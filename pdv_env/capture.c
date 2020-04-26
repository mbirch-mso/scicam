

/*
Add description of this capture routine that will be used for all generic
framegrabber purposes
Capture.c outputs the file as fits and can only take 1 command line argument:
Example: capture -f "location"
The FITS header is filled with the following important keys
BITPIX / 16 / number of bits per data pixel (2-bit padding)
NAXIS1 / 1280 / width of detector
NAXIS2 / 1024 / height of detector
DEPTH / 14 / bits per pixel (FPA)
BUFTIM / seconds for buffer to fill
PROGNM / "capture.c" / EDT framegrabber routine used for image capture
TIME / AEST date and time of capture
*/


#include <string.h>
#include <stdio.h>
#include <limits.h> 
#include <time.h>
#include "edtinc.h"
#include "longnam.h"
#include "fitsio.h"

    int
savefitsfile(u_char *datatosave, int numberofdimensions, 
        long *dimensionvalues, char *fitsfilename, double buf_time, char *progname, int depth, char *time_str);

    int
check_frame_s(u_char *metdata);


    
    int
main( int argc, char *argv[] )
{
    int     width, height, depth;
    char   *cameratype, *progname;
    char    time_str[64];
    char    fitsfname[128];
    u_char *image_p; /*Image data pointer*/
    u_char metadata[2560]; 
    PdvDev *pdv_p;  /*Device pointer */

    /* Retrieve local date/time for FITS header*/
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(time_str, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, 
            tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);


    /*Open camera and flush FIFO memory */
    pdv_p = pdv_open_channel(EDT_INTERFACE, 0, 0);
    pdv_flush_fifo(pdv_p);
    /* Read specifications from config file */
    width = pdv_get_width(pdv_p);
    height = pdv_get_height(pdv_p);
    depth = pdv_get_depth(pdv_p);
    cameratype = pdv_get_cameratype(pdv_p);
    long naxes[2] = { width , (height - 1) }; /* Dimensions for FITS output without metadata row*/
    printf("reading 1 image from '%s'\nWidth: %d, Height: %d, Depth: %d\n", cameratype,width,height,depth);


    /* Image capture section START*/
    pdv_multibuf(pdv_p, 1); /*  Allocate Buffer */
    edt_start_buffers(pdv_p, 1); /*Open buffer for data*/
    double start_time = edt_dtime(); /*Start timing*/
    fflush(stdout);
    image_p = pdv_wait_image(pdv_p); /*Wait until buffer is full then output to array*/
    double end_time = edt_dtime(); /*Finish timing*/
    /* Image capture section END*/

    if (pdv_timeouts(pdv_p) > 0){
        printf("got timeout, check camera and cables\n");
    }
    
    int row_size = width * sizeof(short);
    memcpy(metadata, image_p, (row_size - 1)); /* Move image pointer to second row*/
    image_p = &image_p[row_size]; /* Move image pointer to second row*/

    /*Frame time ticks */
    u_char FT0 = metadata[146];
    u_char FT1 = metadata[147];
    u_char FT2 = metadata[148];
    u_char FT3 = metadata[149];

    /*Int time ticks */
    u_char IT0 = metadata[142];
    u_char IT1 = metadata[143];
    u_char IT2 = metadata[144];
    u_char IT3 = metadata[145];
    char frame_clks[] = { FT3, FT2, FT1, FT0};
    char int_clks[] = { IT3, IT2, IT1, IT0};
    uint32_t frame_clks_int = *(uint32_t *)frame_clks;
    uint32_t myInt1 = (frame_clks[0] << 24) + (frame_clks[1] << 16) 
                        + (frame_clks[2] << 8) + frame_clks[3];

    /*Offset pixels*/
    u_char COFF0 = metadata[130];
    u_char COFF1 = metadata[131];
    u_char CWS0 = metadata[132];
    u_char CWS1 = metadata[133];
    u_char HB0 = metadata[134];
    u_char HB1 = metadata[134];
    u_char ROFF0 = metadata[135];
    u_char ROFF1 = metadata[136];
    u_char RWS0 = metadata[137];
    u_char RWS1 = metadata[138];

    /* FPA TEMP */
    u_char temp1 = metadata[476];
    u_char temp2 = metadata[477];
    u_char temp3 = metadata[478];
    u_char temp4 = metadata[479];
    u_char temp5 = metadata[480];
    u_char temp6 = metadata[481];
    u_char temp7 = metadata[482];
    u_char temp8 = metadata[483];
    u_char temp9 = metadata[484];
    u_char temp10 = metadata[485];
    u_char temp11 = metadata[486];
    u_char temp12 = metadata[487];

    /*Cooler runtime in minutes */
    u_char cooler1 = metadata[488];
    u_char cooler2 = metadata[489];
    u_char cooler3 = metadata[490];
    u_char cooler4 = metadata[491];

    printf("TEMP--byte1: %x,byte2: %x,byte3: %x,byte4: %x\n",temp1,temp2,temp3,temp4);
    printf("TEMP2--byte1: %x,byte2: %x,byte3: %x,byte4: %x\n",temp5,temp6,temp7,temp8);
    printf("TEMP3--byte1: %x,byte2: %x,byte3: %x,byte4: %x\n",temp9,temp10,temp11,temp12);
    printf("COOLER--byte1: %x,byte2: %x,byte3: %x,byte4: %x\n",cooler1,cooler2,cooler3,cooler4);
    printf("FT--byte1: %x,byte2: %x,byte3: %x,byte4: %x\n",FT0,FT1,FT2,FT3);
    printf("IT--byte1: %x,byte2: %x,byte3: %x,byte4: %x\n",IT0,IT1,IT2,IT3);
    printf("checking reg130: COFF0: %x",COFF0);
    progname = argv[0];
    char ctype[10] = { ".c"};
    strcat(progname, ctype);
    strcpy(fitsfname, argv[2]);

    savefitsfile(image_p, 2, naxes, fitsfname, end_time, progname, depth, time_str); /*Output to FITS file*/
    printf("Image written to %s\n",fitsfname);
    
    pdv_close(pdv_p);

    exit(0);
}

    int
savefitsfile(u_char *datatosave, int numberofdimensions, 
long *dimensionvalues, char *fitsfilename, double buf_time, char *progname, int depth, char *time_str) 
{
    fitsfile *fitsfilepointer = NULL;
    int status = 0;
    LONGLONG nelements;
    long fpixel[2];
    char exclamantionpoint[2] = "!";
    char filename[200] = { NULL }; /*ERROR IS FLAGGED HERE DUE TO POINTER TYPE CONVERSION */
    char extension[10] = { ".fits" };
    fpixel[0] = fpixel[1] = 1;
    nelements = dimensionvalues[0] * dimensionvalues[1];

    strcat(filename, exclamantionpoint);
    strcat(filename, fitsfilename);
    strcat(filename, extension);

    fits_create_file(&fitsfilepointer, filename, &status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    fits_create_img(fitsfilepointer, USHORT_IMG, numberofdimensions, dimensionvalues, &status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    fits_write_pix(fitsfilepointer, TUSHORT, fpixel, nelements, datatosave, &status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    /* Write time to fill data buffer in seconds */
    const char *depth_ptr = "DEPTH";
    const char *depth_com = "bits per pixel";
    fits_write_key(fitsfilepointer,TINT,depth_ptr,&depth,depth_com,&status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    /* Write time to fill data buffer in seconds */
    const char *buf_time_ptr = "BUFTIM";
    const char *buf_time_com = "seconds for buffer to fill";
    fits_write_key(fitsfilepointer,TDOUBLE,buf_time_ptr,&buf_time,buf_time_com,&status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    /* Write time to fill data buffer in seconds */
    const char *progname_ptr = "PROGNM";
    const char *progname_com = "EDT framegrabber routine used for capture";
    fits_write_key(fitsfilepointer,TSTRING,progname_ptr,progname,progname_com,&status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

        /* Write time to fill data buffer in seconds */
    const char *time_ptr = "TIME";
    const char *time_com = "AEST date and time of image";
    fits_write_key(fitsfilepointer,TSTRING,time_ptr,time_str,time_com,&status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    fits_close_file(fitsfilepointer, &status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    return (0);
}

  /*  int
check_frame_s(u_char *metadata);
*/