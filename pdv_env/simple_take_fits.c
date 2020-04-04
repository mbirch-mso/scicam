/**
 * @file
 * An example program to show usage of EDT PCI DV library to acquire and
 * optionally save single or multiple images from devices connected to EDT
 * high speed digital imaging interface such as the PCI DV C-Link or PCI DV
 * FOX / RCX.
 * 
 * Provided as a starting point example for adding digital image acquisition
 * to a user application.  Includes optimization strategies that take
 * advantage of the EDT ring buffer library subroutines for pipelining image
 * acquisition and subsequent processing. This allows you to achieve higher
 * performance than would normally be possible through a basic acquire/process
 * scheme.
 *
 * The name is somewhat misleading -- because of the parallel aspect,
 * it really isn't the simplest way to do image acquisition.  For a
 * stone-simple example, see simplest_take.c.
 * 
 * For more more complex operations, including error detection, diagnostics,
 * changing camera exposure times, and tuning the acquisition in various
 * ways, refer to the take.c utility. For serial, see serial_cmd.c.
 * 
 * For a sample Windows GUI application code, see wintake.
 * 
 * (C) 1997-2007 Engineering Design Team, Inc.
 */


#include "edtinc.h"
#include "fitsio.h"
#include <string.h>
#include <stdio.h>
#include <limits.h> 
#include "longnam.h"

static void usage(char *progname, char *errmsg);
static void 
save_image(u_char *image_p, int width, int height, int depth,
        char *basename, int count);

/*
 * Main module. NO_MAIN is typically only defined when compiling for vxworks; if you
 * want to use this code outside of a main module in any other OS, just copy the code
 * and modify it to work as a standalone subroutine, including adding parameters in
 * place of the command line arguments
 */

#ifdef NO_MAIN
#include "opt_util.h"
char *argument ;
int option ;
    int
simple_take(char *command_line)
#else
    int
main(argc, argv)
    int     argc;
    char  **argv;
#endif
{
    int     i;
    int     unit = 0;
    int     overrun, overruns=0;
    int     timeouts, last_timeouts = 0;
    int     recovering_timeout = FALSE;
    char   *progname ;
    char   *cameratype;
    char    fitsfname[128];
    int     IS_SUCCESS;
    int     started;
    u_char *image_p; /*Initialise pointer*/
    PdvDev *pdv_p;  
    char    errstr[64];
    int     loops = 1;
    int     width, height, depth, gain;
    char    edt_devname[128];
    int     channel = 0;
    u_char **buff_add;
    int     xferred_width;
#ifdef NO_MAIN
    char **argv  = 0 ;
    int argc = 0 ;
    opt_create_argv("simple_take",command_line,&argc,&argv);
#endif

    progname = argv[0];

    edt_devname[0] = '\0';
    *fitsfname = '\0';

    /*
     * process command line arguments
     */
    --argc;
    ++argv;
    while (argc && ((argv[0][0] == '-') || (argv[0][0] == '/')))
    {
        switch (argv[0][1])
        {
            case 'u':		/* device unit number */
                ++argv;
                --argc;
                if (argc < 1) 
                    usage(progname, "Error: option 'u' requires an argument\n");
                if  ((argv[0][0] >= '0') && (argv[0][0] <= '9'))
                    unit = atoi(argv[0]);
                else strncpy(edt_devname, argv[0], sizeof(edt_devname) - 1);
                break;

            case 'c':		/* device channel number */
                ++argv;
                --argc;
                if (argc < 1) 
                {
                    usage(progname, "Error: option 'c' requires a numeric argument\n");
                }
                if ((argv[0][0] >= '0') && (argv[0][0] <= '9'))
                {
                    channel = atoi(argv[0]);
                }
                else 
                {
                    usage(progname, "Error: option 'c' requires a numeric argument\n");
                }
                break;

            case 'f':		/* FITS save filename */
                ++argv;
                --argc;
                strcpy(fitsfname, argv[0]);
                break;


            case 'l':
                ++argv;
                --argc;
                if (argc < 1) 
                {
                    usage(progname, "Error: option 'l' requires a numeric argument\n");
                }
                if ((argv[0][0] >= '0') && (argv[0][0] <= '9'))
                {
                    loops = atoi(argv[0]);
                }
                else 
                {
                    usage(progname, "Error: option 'l' requires a numeric argument\n");
                }
                break;

            case '-':
                if (strcmp(argv[0], "--help") == 0) {
                    usage(progname, "");
                    exit(0);
                } else {
                    fprintf(stderr, "unknown option: %s\n", argv[0]);
                    usage(progname, "");
                    exit(1);
                }
                break;


            default:
                fprintf(stderr, "unknown flag -'%c'\n", argv[0][1]);
            case '?':
            case 'h':
                usage(progname, "");
                exit(0);
        }
        argc--;
        argv++;
    }

    /*
     * open the interface
     * 
     * EDT_INTERFACE is defined in edtdef.h (included via edtinc.h)
     *
     * edt_parse_unit_channel and pdv_open_channel) are equivalent to
     * edt_parse_unit and pdv_open except for the extra channel arg and
     * would normally be 0 unless there's another camera (or simulator)
     * on the second channel (camera link) or daisy-chained RCI (PCI FOI)
     */
    if (edt_devname[0])
    {
        unit = edt_parse_unit_channel(edt_devname, edt_devname, EDT_INTERFACE, &channel);
    }
    else
    {
        strcpy(edt_devname, EDT_INTERFACE);
    }

    if ((pdv_p = pdv_open_channel(edt_devname, unit, channel)) == NULL)
    {
        sprintf(errstr, "pdv_open_channel(%s%d_%d)", edt_devname, unit, channel);
        pdv_perror(errstr);
        return (1);
    }

    pdv_flush_fifo(pdv_p);

    /*
     * get image size and name for display, save, printfs, etc.
     */
    width = pdv_get_width(pdv_p);
    height = pdv_get_height(pdv_p);
    depth = pdv_get_depth(pdv_p);
    cameratype = pdv_get_cameratype(pdv_p);
    gain = pdv_get_gain(pdv_p);
    int bytes_per_line = pdv_bytes_per_line(width,depth);

    printf("reading %d image%s from '%s'\nWidth: %d, Height: %d, Depth: %d\n",
            loops, loops == 1 ? "" : "s",cameratype,width,height,depth);
    
    pdv_multibuf(pdv_p, 1);
    pdv_start_image(pdv_p);
    started = 1;

    for (i = 0; i < loops; i++)
    {

        printf("image %d\r", i + 1);
        fflush(stdout);
        image_p = pdv_wait_image(pdv_p);

        if ((overrun = (edt_reg_read(pdv_p, PDV_STAT) & PDV_OVERRUN)))
            ++overruns;

        if (i < loops - started)
        {
            pdv_start_image(pdv_p);
        }
        timeouts = pdv_timeouts(pdv_p);

        if (timeouts > last_timeouts)
        {

            pdv_timeout_restart(pdv_p, TRUE);
            last_timeouts = timeouts;
            recovering_timeout = TRUE;
            printf("\ntimeout....\n");
        } else if (recovering_timeout)
        {
            pdv_timeout_restart(pdv_p, TRUE);
            recovering_timeout = FALSE;
            printf("\nrestarted....\n");
        }
        
        if (*fitsfname)
            
            save_image(image_p, width, height, depth, fitsfname, (loops > 1?i:-1));

    }
    puts("");
    printf("%d images %d timeouts %d overruns\n", loops, last_timeouts, overruns);
    

    if (last_timeouts)
        printf("check camera and connections\n");
    
    pdv_close(pdv_p);

    if (overruns || timeouts)
        exit(2);
    exit(0);
}

int savefitsfile_double(u_char *datatosave, int numberofdimensions, long *dimensionvalues, char *fitsfilename) 
{
    fitsfile *fitsfilepointer = NULL;
    int status = 0;
    LONGLONG nelements;
    long fpixel[2];
    int IS_SUCCESS;
    /*Prepend Exclamation Point at beginning to overwrite file*/
    char exclamantionpoint[2] = "!";
    char filename[200] = { NULL };
    char extension[10] = { ".fits" };
    strcat(filename, exclamantionpoint);
    strcat(filename, fitsfilename);
    strcat(filename, extension);
    fpixel[0] = fpixel[1] = 1;
    nelements = dimensionvalues[0] * dimensionvalues[1];

    fits_create_file(&fitsfilepointer, filename, &status);
    if (status != 0) {
        fits_report_error(stderr, status);
        printf("Occured at create file");
        return status;
    }

    fits_create_img(fitsfilepointer, SHORT_IMG, numberofdimensions, dimensionvalues, &status);
    if (status != 0) {
        fits_report_error(stderr, status);
        printf("Occured at create");
        return status;
    }

    fits_write_pix(fitsfilepointer, TUSHORT, fpixel, nelements, datatosave, &status);
    if (status != 0) {
        fits_report_error(stderr, status);
        printf("Occured at write");
        return status;
    }

    fits_close_file(fitsfilepointer, &status);
    if (status != 0) {
        fits_report_error(stderr, status);
        printf("Occured at close");
        return status;
    }

    return IS_SUCCESS;
}

    static void
save_image(u_char *image_p, int s_width, int s_height, int s_depth, char *tmpname, int count)
{
    int     s_db = bits2bytes(s_depth);
    char    fname[256];
    int     success;
    u_char *bbuf = NULL;
    long    naxes[2] = { 0, 0 };
    long    dimensions = 2;
    naxes[0] = s_width;
    naxes[1] = s_height;
    if ((strcmp(&tmpname[strlen(tmpname) - 4], ".fits") == 0)
            || (strcmp(&tmpname[strlen(tmpname) - 4], ".fits") == 0))
        tmpname[strlen(tmpname) - 4] = '\0';
    if (count >= 0)
        sprintf(fname, "%s_%03d.fits", tmpname, count);
    else sprintf(fname, "%s.fits", tmpname);
    success = savefitsfile_double(image_p,2,naxes,tmpname);
    printf("%d writing %dx%dx%d FITS file to %s\n",
                   success, s_width, s_height, s_depth, fname);
}

    static void
usage(char *progname, char *errmsg)
{
    puts(errmsg);
    printf("%s:Program that acquires images testtest from an\n", progname);
    printf("EDT interface board and saves to fits file\n");
    puts("");
    printf("usage: %s [-b fname] [-l loops] [-N numbufs] [-u unit] [-c channel]\n", progname);
    printf("  -f fname        output to FITS file\n");
    printf("  -l loops        number of loops (images to take)\n");
    printf("  -u unit         %s unit number (default 0)\n", EDT_INTERFACE);
    printf("  -c channel      %s channel number (default 0)\n", EDT_INTERFACE);
    printf("  -h              this help message\n");
    exit(1);
}
