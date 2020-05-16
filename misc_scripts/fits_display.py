
from astropy.io import fits
import argparse
import os
import datetime
import numpy as np
import matplotlib.pyplot as plt
import glob
import scicam as cam

p = argparse.ArgumentParser(prog='capture Image', description='Captures image using specified routine')
p.add_argument('-a', type=str, help='Type of Analysis (view or hist)')
p.add_argument('-p', type=str, help='Routine to view',default = 'capture')
p.add_argument('-i', type=str, help='Integration Time',default = '')
p.add_argument('-t', type=str, help='Frame Time',default = '')
p.add_argument('-g', type=str, help='Naming Tag',default = '')
args = p.parse_args()

if args.t != '':
    files = cam.grab_from_args(args.p,args.i,(args.i+25),args.g)
else:
    files = cam.grab_from_args(args.p,args.i,args.t,args.g)
print('Number of images retrieved:{}'.format(len(files)))

x = fits.open(files[0])[0]
print(x.header)

if len(files) == 1:
    fig, axs = plt.subplots(1, 2, tight_layout=True)
    img = x.data
    axs[0].hist(img.flatten(), bins=1000)
    axs[0].set_xlim(np.min(img),np.max(img))
    axs[0].set_title('Pixel distribution $\mu={0}\:\:\sigma={1}$'.format(int(np.std(img)),int(np.mean(img))))
    axs[0].set_xlabel('ADUs')
    axs[1].imshow(img)
    axs[1].set_title('NDIT:{0},DIT={1}'.format(x.header['NDIT'],x.header['DITMET']))
    plt.show()
else:
    if args.a == 'hist':
        cam.group_hist(files)
    elif args.a == 'view':
        cam.group_display(files)
    else:
        print('Must specify display for multi-image mode')




