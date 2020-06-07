import numpy as np
import matplotlib.pyplot as plt
import scicam as cam
import scipy.ndimage
from scipy.stats import sigmaclip,norm
from scipy import signal
from skimage import exposure
import seaborn as sns
import matplotlib.mlab as mlab
import glob
import os
import astropy.io as fits
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D
from astropy.io import fits

#Folder variables
testing_dir = '//merger.anu.edu.au/mbirch/data/'
folder_path = lambda test: testing_dir + test + "/" 
frame_path = folder_path('camera_control/frame_time_studies')
int_path = folder_path('camera_control/int_time_studies')
frame_int_space_path = folder_path('camera_control/frame_int_space_testing')
saturation_path = folder_path('saturation_testing')
read_path = folder_path('read_testing')
master_biases = folder_path('master_bias')

naxis1 = 1040
naxis2 = 1296




def master_bias(frames,tag):
    int_t = cam.set_int_time(0.033)
    frame_t = cam.set_frame_time(20.033)
    printProgressBar(0, frames*2, prefix = 'Progress:', suffix = 'Complete', length = 50)
    y=0
    master_bias = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
    for j in range(frames):
        cap, _ = cam.img_cap(routine,img_dir,'f')
        hdu_img = fits.open(unsorted_img)
        fits_img = hdu_img[0]
        master_bias += fits_img.data
        hdu_img.close() #Close image so it can be sorted

        y = y + 1
        printProgressBar(y,frames, prefix = 'Progress:', \
            suffix = 'Complete', length = 50)
        if j == frames-1: #On final frame grab header
            frame_header = fits.getheader(unsorted_img)
            frame_header.append(('NDIT',frames,'Number of integrations'))
            sky_header.append(('TYPE','MASTER_BIAS','0s exposure frame'))
        os.remove(unsorted_img) #Delete image after data retrieval 
    
        master_bias = master_bias / frames #Divide by NDIT
        master_path = testing_dir + 'read_testing/' + 'master_bias_' \
                + tag + '.fits'
        fits.writeto(master_path,master_bias,frame_header)

    print('PROGRAM HAS COMPLETED')

def master_flat(int_time,folder):
    dark = fits.open(dark_path)
    bias = cam.get_master_bias('-60')
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    img_list_split = [i.split('_') for i in img_list]
    stack = np.zeros((1040,1296))
    for j in files:
        if img_list_split[k][1] == str(float(int_time)):
            hdu = fits.open(j)
            img = hdu[0].data
            img = img - bias - dark #Subtract bias and dark from each flat
            stack = np.dstack((stack,data))
    stack = stack[:,:,1:] #Remove 0 array it is stacked on
    flat_collapsed = np.median(stack, axis=2)
    flat_normed = flat_collapsed/np.max(flat_collapsed) #Normalise flat

    flat_header = hdu[0].header
    flat_header.append(('NSTACK',stack.shape[2],'Number of exposures stacked'))
    flat_header.append(('TYPE','MASTER_FLAT','Normalised median stack of flat fields'))

    master_name = 'mastersky_'+str(i/1000)+"s_" + str(stack.shape[2]) \
                    + 'stack_am' + str(am) + '.fits'
    
    #Write to FITS file
    fits.writeto(master_name,sky_reduced,sky_header)
    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    return background_est

def master_sky(i,folder,am):
    '''
    Takes integration time in ms, folder containing sky backgrounds
    an airmass and camera temperature
    '''
    os.chdir(folder)
    bias = cam.get_master_bias(-60) #Retrieve bias
    img_list = glob.glob('*.fits*')
    img_list_split = [i.split('_') for i in img_list]
    stack = np.zeros((1040,1296))
    for k in range(len(img_list)):
        if img_list_split[k][1] == str(float(i)):
            hdu = fits.open(img_list[k])
            data = hdu[0].data
            data = data - bias #Bias subtract
            stack = np.dstack((stack,data))
    stack = stack[:,:,1:] #Remove 0 array it is stacked on
    #Collapse multi-dimensional array along depth axis by median
    sky_collapsed = np.median(stack, axis=2)
    #Create single estimate in adus/pixel/s
    sky_reduced_clipped, _ , _ = sigmaclip(sky_reduced,3,3) #3 sigma clip of outliers
    sky_reduced_clipped = sky_reduced_clipped/(i/1000) #Divide by seconds
    background_est = round(np.mean(sky_reduced_clipped),2)

    #Append neccesary info to header
    sky_header = hdu[0].header
    sky_header.append(('NSTACK',stack.shape[2],'Number of exposures stacked'))
    sky_header.append(('TYPE','MASTER_SKY','Median stack of sky backgrounds'))
    sky_header.append(('AIRMASS',am,'Airmass of exposures'))
    sky_header.append(('COUNTEST',background_est,'Estimate of background in ADUs/pixel/s'))
    master_name = 'mastersky_'+str(i/1000)+"s_" + str(stack.shape[2]) \
                    + 'stack_am' + str(am) + '.fits'
    
    #Write to FITS file
    fits.writeto(master_name,sky_reduced,sky_header)
    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    return background_est