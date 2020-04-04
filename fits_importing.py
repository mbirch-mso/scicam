
from astropy.io import fits
import os
import datetime
import numpy as np
import matplotlib.pyplot as plt
import glob
import scicam as cam


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
        ax.set_title('Int:{0} Frame:{1}'\
                .format(img.header['INT_T'],img.header['FRAME_T']))
    #fig.set_title('{}'.format(hdu.header['ROUTINE']))
    plt.show()

