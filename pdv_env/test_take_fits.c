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
        char *basename, uint regstatus, int fv_counts, double buf_time);

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
    u_char *image_p; /*Initialise pointer*/
    PdvDev *pdv_p;  
    char    errstr[64];
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

    /* process command line arguments*/
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
    pdv_cl_reset_fv_counter(pdv_p);
    
    width = pdv_get_width(pdv_p);
    height = pdv_get_height(pdv_p);
    depth = pdv_get_depth(pdv_p);
    cameratype = pdv_get_cameratype(pdv_p);

    /* Initalise Buffer */
    pdv_multibuf(pdv_p, 1);
    
    /* Image Capture Section */
    double start_time = edt_dtime();
    edt_start_buffers(pdv_p, 1);

    fflush(stdout);
    uint mid_status = edt_reg_read(pdv_p, PDV_STAT);

    image_p = pdv_wait_image(pdv_p); /* Bypass processing and collect from DMA */
    double end_time = edt_dtime();
    double slop_time = 0.000014; /* Time taken to do fflush and reg_read commands */
    /* Image Capture Section */
    double buf_time = end_time - slop_time;

    if ((overrun = (edt_reg_read(pdv_p, PDV_STAT) & PDV_OVERRUN)))
        ++overruns;
    timeouts = pdv_timeouts(pdv_p);
    if (timeouts > last_timeouts){
        pdv_timeout_restart(pdv_p, TRUE);
        last_timeouts = timeouts;
        recovering_timeout = TRUE;
        printf("\ntimeout....\n");
    } 
    else if (recovering_timeout){
        pdv_timeout_restart(pdv_p, TRUE);
        recovering_timeout = FALSE;
        printf("\nrestarted....\n");
    }
    int fv_counts = pdv_cl_get_fv_counter(pdv_p);

    printf("%d Timeouts %d Overruns\n", last_timeouts, overruns);
    printf("Number of Frame Valids Seen:%d\n",fv_counts);
    printf("Time to fill buffs: %f seconds\n",buf_time);
    printf("STATUS after buffer opened:%x\n",mid_status);
    
    if (*fitsfname){
        save_image(image_p, width, height, depth, fitsfname,mid_status, fv_counts, buf_time);
    }
    
    if (last_timeouts)
        printf("check camera and connections\n");

    pdv_close(pdv_p);

    if (overruns || timeouts)
        exit(2);
    exit(0);
}

int savefitsfile_double(u_char *datatosave, int numberofdimensions, 
long *dimensionvalues, char *fitsfilename,uint regstatus, int fv_counts, double buf_time) 
{
    fitsfile *fitsfilepointer = NULL;
    int status = 0, nkeys, IS_SUCCESS;
    char card[FLEN_CARD];
    LONGLONG nelements;
    long fpixel[2];
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

    fits_create_img(fitsfilepointer, USHORT_IMG, numberofdimensions, dimensionvalues, &status);
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

    /* Write registry read of 8bit 0x01 STATUS Reg ot FITS */
    const char *stat_ptr = "STATUS";
    const char *stat_com = "Reg read of 0x01 Status";
    fits_write_key(fitsfilepointer,TUINT,stat_ptr,&regstatus,stat_com,&status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }
    
    /* Write number of frame valid signals counted */
    const char *fvsc_ptr = "FVSNUM";
    const char *fvsc_com = "Number of frame valid signals counted";
    fits_write_key(fitsfilepointer,TUINT,fvsc_ptr,&fv_counts,fvsc_com,&status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    /* Write time to fill data buffer in seconds */
    const char *buf_time_ptr = "BUFTIM";
    const char *buf_time_com = "Seconds for buffer to fill";
    fits_write_key(fitsfilepointer,TDOUBLE,buf_time_ptr,&buf_time,buf_time_com,&status);
    if (status != 0) {
        fits_report_error(stderr, status);
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
save_image(u_char *image_p, int s_width, int s_height, int s_depth, char *tmpname,uint regstatus, int fv_counts, double buf_time)
{
    int     s_db = bits2bytes(s_depth);
    int     success;
    long    naxes[2] = { 0, 0 };
    long    dimensions = 2;
    naxes[0] = s_width;
    naxes[1] = s_height;
    success = savefitsfile_double(image_p,2,naxes,tmpname,regstatus,fv_counts,buf_time);
    printf("Writing %dx%dx%d FITS file to %s\n",
                    s_width, s_height, s_depth, tmpname);
}
    static void
usage(char *progname, char *errmsg)
{
    puts(errmsg);
    printf("%s:Program that acquires images from an\n", progname);
    printf("EDT interface board and saves to fits file\n");
    puts("");
    printf("usage: %s [-b fname] [-l loops] [-N numbufs] [-u unit] [-c channel]\n", progname);
    printf("  -f fname        output to FITS file\n");
    printf("  -u unit         %s unit number (default 0)\n", EDT_INTERFACE);
    printf("  -c channel      %s channel number (default 0)\n", EDT_INTERFACE);
    printf("  -h              this help message\n");
    exit(1);
}
