
from astropy.io import fits
import argparse
import os
import datetime
import numpy as np
import matplotlib.pyplot as plt
import glob
import scicam as cam

p = argparse.ArgumentParser(prog='capture Image', description='Captures image using specified routine')
p.add_argument('a', type=str, help='Type of Analysis (view or hist)')
p.add_argument('-p', type=str, help='Routine to view',default = '')
p.add_argument('-i', type=str, help='Integration Time',default = '')
p.add_argument('-t', type=str, help='Frame Time',default = '')
args = p.parse_args()

files = cam.grab_from_args(args.p,args.i,args.t)
x = fits.open(files[0])[0]
print(x.header)
if args.a == 'hist':
    cam.group_hist(files)
elif args.a == 'view':
    cam.group_display(files)
else:
    pass