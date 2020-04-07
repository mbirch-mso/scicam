
from astropy.io import fits
import argparse
import os
import datetime
import numpy as np
import matplotlib.pyplot as plt
import glob
import scicam as cam
parser = argparse.ArgumentParser(prog='capture Image', description='Captures image using specified routine')
parser.add_argument('a', type=str, help='Type of Analysis (view or hist)')
parser.add_argument('-p', type=str, help='Routine to view')
parser.add_argument('-i', type=float, help='Integration Time')
parser.add_argument('-t', type=float, help='Frame Time')
args = parser.parse_args()
#Create Histogram of file list 
def group_hist(files):
    for k in range(len(files)):
        img = fits.open(files[k])[0]
        img_data = img.data
        img_int = img.header['INT_T']
        img_frame = img.header['FRAME_T']
        plt.hist(img_data.flatten(), bins=400, \
            label = 'Integration Time (ms):{0} Frame Time (ms):{1}'\
                .format(img_int,img_frame))
    plt.legend(loc='best')
    plt.title('{}'.format(img.header['ROUTINE']))
    plt.xlabel('Counts (ADUs)')
    plt.ylabel('# of Pixels')
    plt.show()
    #Include analysis of hist curves
#Create image gallery of files
def group_display(files):
    Tot = len(files)
    Cols = Tot//2
    Rows = Tot // Cols 
    Rows += Tot % Cols
    Position = range(1,Tot + 1)
    fig = plt.figure(1)
    for k in range(Tot):
        img = fits.open(files[k])[0]
        ax = fig.add_subplot(Rows,Cols,Position[k])
        ax.imshow(img.data)
        ax.set_xticks([])
        ax.set_yticks([])
        ax.set_title('{0},{1}'\
                .format(int(img.header['INT_T']),\
                int(img.header['FRAME_T'])))
    #fig.set_title('{}'.format(hdu.header['ROUTINE']))
    plt.show()


if args.p:
    if args.i:
        if args.t:
            files = cam.import_fits(args.p,i=args.i,t=args.t)
        else:
            files = cam.import_fits(args.p,i=args.i)
    else:
        if args.t:
            files = cam.import_fits(args.p,t=args.t)
        else:
            files = cam.import_fits(args.p)
else:
    if args.i:
        if args.t:
            files = cam.import_fits(i=args.i,t=args.t)
        else:
            files = cam.import_fits(i=args.i)
    else:
        if args.t:
            files = cam.import_fits(t=args.t)
        else:
            files = cam.import_fits()

if args.a == 'hist':
    group_hist(files)
elif args.a == 'view':
    group_display(files)
else:
    pass