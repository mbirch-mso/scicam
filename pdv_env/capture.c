

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
        long *dimensionvalues, char *fitsfilename, double buf_time, char *progname, int depth, char *time_str,
        float int_time, float frame_time);

    float
read_int_time(u_char *metadata);

    float
read_frame_time(u_char *metadata);

    
    int
main( int argc, char *argv[] )
{
    int     width, height, depth;
    char   *cameratype, *progname;
    char    time_str[64];
    char    fitsfname[128];
    u_char *image_p; /*Image data pointer*/

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
    u_char metadata[2560]; 
    memcpy(metadata, image_p, (row_size - 1)); /* Move image pointer to second row*/
    image_p = &image_p[row_size]; /* Move image pointer to second row*/

    progname = argv[0];
    char ctype[10] = { ".c"};
    strcat(progname, ctype);
    strcpy(fitsfname, argv[2]);

    float int_time = read_int_time(metadata);
    float frame_time = read_frame_time(metadata);

    savefitsfile(image_p, 2, naxes, fitsfname, end_time, progname, depth, time_str,int_time, frame_time); /*Output to FITS file*/
    printf("Image written to %s\n",fitsfname);
    
    pdv_close(pdv_p);

    exit(0);
}

    int
savefitsfile(u_char *datatosave, int numberofdimensions, 
long *dimensionvalues, char *fitsfilename, double buf_time, char *progname, int depth, char *time_str,float int_time, float frame_time) 
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

    /* Write number of bits per pixel*/
    const char *depth_ptr = "DEPTH";
    const char *depth_com = "bits per pixel";
    fits_write_key(fitsfilepointer,TINT,depth_ptr,&depth,depth_com,&status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    /* Write number of bits per pixel*/
    const char *int_ptr = "INTT";
    const char *int_com = "Integration time in ms";
    fits_write_key(fitsfilepointer,TFLOAT,int_ptr,&int_time,int_com,&status);
    if (status != 0) {
        fits_report_error(stderr, status);
        return status;
    }

    /* Write frame time in milliseconds*/
    const char *frame_ptr = "FRAMET";
    const char *frame_com = "Frame time in ms";
    fits_write_key(fitsfilepointer,TFLOAT,frame_ptr,&frame_time,frame_com,&status);
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

        /* Write AEST date and time */
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

  float
read_int_time(u_char *metadata)
{
    u_char IT0=metadata[142], IT1=metadata[143], IT2=metadata[144], IT3=metadata[145];
    uint32_t int_refclks = (IT3 << 24) + (IT2 << 16) 
                        + (IT1 << 8) + IT0; 
    float int_time = (int_refclks/16E6)*1000;
    return int_time;
}

    float
read_frame_time(u_char *metadata)
{
    u_char FT0=metadata[146], FT1=metadata[147], FT2=metadata[148], FT3=metadata[149];
    uint32_t frame_refclks = (FT3 << 24) + (FT2 << 16) 
                        + (FT1 << 8) + FT0; 
    float frame_time = (frame_refclks/16E6)*1000;
    return frame_time;
}
