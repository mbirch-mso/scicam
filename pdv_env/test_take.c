#include "edtinc.h"
main(int argc, char **argv)
{
    int     unit = 0, channel = 0;
    char    edt_devname[128];
    char    errstr[64];
    u_char *image_p;
    PdvDev *pdv_p;
    char osn[32];

    if (argc > 3)
    {
        printf("usage: %s -u [unit]\n", argv[0]);
        exit(1);
    }
    if ((argc > 1) && ((strcmp(argv[1], "-u") == 0)))
    {
        unit = atoi(argv[2]);
    }
    if ((pdv_p = pdv_open_channel(EDT_INTERFACE, unit, channel)) == NULL)
    {
        sprintf(errstr, "pdv_open_channel(%s%d_%d)", edt_devname, unit, channel);
        pdv_perror(errstr);
        return (1);
    }
    image_p = pdv_image(pdv_p);
    if (pdv_timeouts(pdv_p) > 0)
        printf("got timeout, check camera and cables\n");
    else printf("got one image\n");
    pdv_close(pdv_p);

    exit(0);
}


