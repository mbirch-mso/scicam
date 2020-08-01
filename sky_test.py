import scicam as cam
import numpy as np
import time
from astropy.io import fits
import argparse

parser = argparse.ArgumentParser(prog='Take 10 frames at given exposure times')
parser.add_argument('-l', type=int, help='Number of loops/images to save',default=1)
parser.add_argument('-g', type=str, help='Tag for placing in folder',default = '')
parser.add_argument('-c', type=str, help='Commment for FITS Header',default = '')
args = parser.parse_args()

start = time.time()

#Input Parameters
local_img_dir = 'C:/mso/images'
unsorted_img = local_img_dir +'/image_unsorted.fits'
routine = 'capture'
naxis1 = 1040
naxis2 = 1296

def expose(i,tag=''):
    int_t = cam.set_int_time(i)
    frame_t = cam.set_frame_time(i+100)
    frame , int_header = cam.simple_cap()
    if args.c != '':
        int_header.append(('COMMENT',args.c,'User-defined comment'))
    fits.writeto(unsorted_img,frame,int_header)
    cam.weather_to_fits(unsorted_img)
    cam.file_sorting(local_img_dir,int_t,frame_t,tag=tag)
    
#Fixed exposure times of interest for testing
times = [500,1000.5,2000,3000,4000,5000,10000]

for k in times:
    for j in range(args.l):
        expose(k,tag=args.g)

end = time.time()
print("Image capture complete, time elapsed: {}".format(int(end-start)))