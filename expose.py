import scicam as cam
import os
import numpy as np
from astropy.io import fits
import argparse

parser = argparse.ArgumentParser(prog='Expose and save to FITS', description='Expose for a given integration time and number of integrations')
parser.add_argument('i', type=float, help='Integration time in milliseconds (DIT)')
parser.add_argument('-n', type=int, help='Number of integrations to co-add (NDIT)')
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

def expose(i,n,tag=''):
    int_t = cam.set_int_time(i)
    frame_t = cam.set_frame_time(i+25)
    cam.printProgressBar(0, n, prefix = 'Progress:', suffix = 'Complete', length = 50)
    y=0
    array = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
    for j in range(n):
        _ , _ = cam.img_cap(routine,img_dir,'f')
        hdu_img = fits.open(unsorted_img)
        fits_img = hdu_img[0]
        array += fits_img.data
        hdu_img.close() #Close image so it can be sorted
        y = y + 1
        cam.printProgressBar(y,n, prefix = 'Progress:', \
            suffix = 'Complete', length = 50)
        if j == n-1: #On final frame grab header
            int_header = fits.getheader(unsorted_img)
            int_header.append(('NDIT',n,'Number of integrations'))
        os.remove(unsorted_img) #Delete image after data retrieval 
    
    if args.c != '':
        int_header.append(('COMMENT',args.c,'User-defined comment'))
    
    fits.writeto(unsorted_img,array,int_header)
    cam.weather_to_fits(unsorted_img)
    cam.file_sorting(img_dir,int_t,frame_t,tag=tag)
    print('EXPOSE COMPLETE')

for i in range(args.l-1):
    if args.n:
        expose(args.i,args.n,args.g)
    else:
        expose(args.i,1,args.g)
