import scicam as cam
import numpy as np
import os
from astropy.io import fits
import argparse

parser = argparse.ArgumentParser(prog='Expose and save to FITS', description='Expose for a given integration time and number of integrations')
parser.add_argument('i', type=float, help='Integration time in milliseconds (DIT)')
parser.add_argument('-l', type=int, help='Number of loops/images to save',default=1)
parser.add_argument('-g', type=str, help='Tag for placing in folder',default = '')
parser.add_argument('-c', type=str, help='Commment for FITS Header',default = '')
args = parser.parse_args()


#Input Parameters
testing_dir = '//merger.anu.edu.au/mbirch/data/'
img_dir = '//merger.anu.edu.au/mbirch/images'
unsorted_img = img_dir +'/image_unsorted.fits'
routine = 'capture'
naxis1 = 1040
naxis2 = 1296


def expose(i,tag=''):
    int_t = cam.set_int_time(i)
    frame_t = cam.set_frame_time(i+250)
    frame , int_header = cam.simple_cap()
    if args.c != '':
        int_header.append(('COMMENT',args.c,'User-defined comment'))
  #  os.remove(unsorted_img)
    fits.writeto(unsorted_img,frame,int_header,overwrite=True)
    cam.weather_to_fits(unsorted_img)
    cam.file_sorting(img_dir,int_t,frame_t,tag=tag)
    print('EXPOSE COMPLETE')
    

for i in range(args.l):
        expose(args.i,args.g)
