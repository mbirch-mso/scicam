import numpy as np
import matplotlib.pyplot as plt
import scicam as cam
import scipy.ndimage
from scipy.stats import sigmaclip,norm
from scipy import signal
from skimage import exposure
import matplotlib.mlab as mlab
import glob
import os
import astropy.io as fits
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D
from astropy.io import fits

#Folder variables
testing_dir = '//merger.anu.edu.au/mbirch/data/'
img_dir = '//merger.anu.edu.au/mbirch/images'
local_img_dir = 'C:/images'

#Data paths
folder_path = lambda test: testing_dir + test + "/" 
saturation_path = folder_path('saturation_testing')
read_path = folder_path('read_testing')
master_biases = folder_path('master_bias')
master_darks = folder_path('master_dark')
master_skys = folder_path('master_sky')
unsorted_img = img_dir + '/image_unsorted.fits'
local_unsorted = local_img_dir + '/image_unsorted.fits'

#Camera specifications
routine = 'capture'
naxis1 = 1040
naxis2 = 1296


def master_bias(n,tag,T):
    '''
    Enter docstring here
    '''
    cam.set_int_time(0.033)
    cam.set_frame_time(100.033)
    cam.printProgressBar(0, n, prefix = 'Progress:', suffix = 'Complete', length = 50)
    
    stack = np.zeros((naxis1,naxis2),dtype=np.uint16)
    for j in range(n):
        cap, _ = cam.img_cap(routine,img_dir,'f')
        hdu_img = fits.open(unsorted_img)
        fits_img = hdu_img[0]
        data = fits_img.data
        hdu_img.close() #Close image so it can be sorted

        stack = np.dstack((stack,data))

        cam.printProgressBar(j,n, prefix = 'Progress:', \
            suffix = 'Complete', length = 50)

        if j == n-1: #On final frame grab header
            bias_header = fits.getheader(unsorted_img)

        os.remove(unsorted_img) #Delete image after data retrieval 

    bias_header.append(('NDIT',n,'Number of integrations'))
    bias_header.append(('TYPE','MASTER_BIAS','0s exposure frame'))
    bias_header.append(('FPATEMP',T,'Temperature of detector'))

    #Median Stack
    stack = stack[:,:,1:] #Slice off base layer
    master_bias = np.median(stack, axis=2)
    master_bias = master_bias.astype(np.uint16)
    #Write master frame to fits
    master_path = read_path + 'master_bias_' \
                + tag + '.fits'
    fits.writeto(master_path,master_bias,bias_header)
    print('PROGRAM HAS COMPLETED')

def master_dark(i,n,T,tag=''):
    '''
    DIT and NDIT are inputs
    Function can also take tag for sorting individual frames onto local drive
    T is the FPA temperature used to record temperature of FPA for this dark
    which is written to file name and FITS header
    Program also outputs a .npy binary file containing 3D datacube of central (100,100)
    window for studying temporal variance over stack
    '''
    cam.set_int_time(i)
    cam.set_frame_time(i+20)
    bias = cam.get_master_bias(T)

    cam.printProgressBar(0, n , prefix = 'Progress:', suffix = 'Complete', length = 50)

    stack = np.zeros((naxis1,naxis2),dtype=np.uint16)
    window = np.zeros((100,100),dtype=np.uint16)
    for j in range(n):
        _ , _ = cam.img_cap(routine,img_dir,'f')
        hdu_img = fits.open(unsorted_img)
        data = hdu_img[0].data
        hdu_img.close() #Close image so it can be sorted

        data = data - bias
        stack = np.dstack((stack,data))

        data_window = cam.window(data,100)
        window = np.dstack((window,data_window))

        cam.printProgressBar(j,n, prefix = 'Progress:', \
            suffix = 'Complete', length = 50)
        
        if j == n-1: #On final frame grab header
            dark_header = fits.getheader(unsorted_img)

        
        #Save single frame to local drive 
        cam.file_sorting(local_img_dir,i,i+20,tag=tag)
    
    #Median stack
    stack = stack[:,:,1:] #Slice off base layer
    master_dark = np.median(stack, axis=2)

    #Prepare window for temporal analysis
    window = window[:,:,1:] #Slice off base layer
    temp_var = np.median(np.var(stack, axis=2))
    temp_path = master_darks + 'dark_cube' \
                + str(i/1000) + '_' +str(T) +'C.npy'
    np.save(temp_path,window)

    dark_header.append(('NDIT',n,'Number of integrations'))
    dark_header.append(('TYPE','MASTER_DARK','Median stack of dark frames'))
    dark_header.append(('FPATEMP',T,'Temperature of detector'))
    dark_header.append(('TEMPVAR',temp_var,'Median temporal variance of central (100,100) window'))


    #Output master frame to fits
    master_path = master_darks + 'master_dark_' \
                + str(i/1000) + '_' +str(T) +'C.fits'
    fits.writeto(master_path,master_dark,dark_header)
    print('PROGRAM HAS COMPLETED')

def master_flat(i,folder):
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

def master_sky(i,folder,am,filter):
    '''
    Takes integration time in ms, folder containing sky backgrounds
    an airmass and camera temperature
    '''
    os.chdir(folder)
    bias = cam.get_master_bias(-60) #Retrieve bias
    bias = bias.astype(np.int32)
    dark = cam.get_master_dark(int(i/2000)) #Retrieve dark
    dark = dark.astype(np.int32)
    img_list = glob.glob('*.fits*')
    img_list_split = [i.split('_') for i in img_list]
    stack = np.zeros((1040,1296),dtype=np.int32)
    for k in range(len(img_list)):
        if img_list_split[k][1] == str(float(i)):
            hdu = fits.open(img_list[k])
            data = hdu[0].data
            data = data.astype(np.int32)
            data -= bias #Bias subtract
            data -= dark #Dark subtract
            stack = np.dstack((stack,data))
    
    stack = stack[:,:,1:] #Remove 0 array it is stacked on
    #Collapse multi-dimensional array along depth axis by median
    sky_collapsed = np.median(stack, axis=2)

    #Append neccesary info to header
    sky_header = hdu[0].header
    sky_header.append(('NDIT',stack.shape[2],'Number of integrations'))
    sky_header.append(('TYPE','MASTER_SKY','Median stack of sky backgrounds'))
    sky_header.append(('AIRMASS',am,'Airmass of exposures'))
    sky_header.append(('BAND',filter,'Bandpass filter'))
    master_name = master_skys + 'mastersky_'+str(i/1000)+"s_" + str(stack.shape[2]) \
                    + 'stack_am' + str(am) + '.fits'
    
    #Write to FITS file
    fits.writeto(master_name,sky_collapsed,sky_header)
    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    print('PROGRAM COMPLETE')

def master_dark_local(folder,i):
    '''
    DIT and NDIT are inputs
    Function can also take tag for sorting individual frames onto local drive
    T is the FPA temperature used to record temperature of FPA for this dark
    which is written to file name and FITS header
    Program also outputs a .npy binary file containing 3D datacube of central (100,100)
    window for studying temporal variance over stack
    Takes folder of single frames
    '''
    os.chdir(folder)

    bias = cam.get_master_bias(-60) #Retrieve bias

    img_list = glob.glob('*.fits*')
    img_list_split = [i.split('_') for i in img_list]
    stack = np.zeros((naxis1,naxis2),dtype=np.uint16)

    for k in range(len(img_list)):
        if img_list_split[k][1] == str(i):
            hdu = fits.open(img_list[k])
            data = hdu[0].data
            data = data - bias #Bias subtract
            data[data > 60000] = 0 #Avoid unsigned integer overflow
            stack = np.dstack((stack,data))

    dark_header = hdu[0].header
    
    stack = stack[:,:,1:] #Slice off base layer
    ndit = stack.shape[2]
    master_dark = np.median(stack, axis=2)
    master_dark = master_dark.astype(np.uint16)

    dark_header.append(('NDIT',ndit,'Number of integrations'))
    dark_header.append(('TYPE','MASTER_DARK','Median stack of dark frames'))
    #Output master frame to fits
    master_path = master_darks + 'master_dark_' \
                + str(i/1000) + '_-10.6C.fits'
    fits.writeto(master_path,master_dark,dark_header)
    print('PROGRAM HAS COMPLETED')

def master_bias_local(folder,T):
    '''
    Enter docstring here
    '''
    os.chdir(folder)
    img_list = glob.glob('*.fits*')

    stack = np.zeros((naxis1,naxis2),dtype=np.uint16)
    for i in img_list:
            hdu = fits.open(i)
            data = hdu[0].data
            stack = np.dstack((stack,data))

    bias_header = hdu[0].header
    
    stack = stack[:,:,1:] #Slice off base layer
    ndit = stack.shape[2]
    master_bias = np.median(stack, axis=2)
    master_bias = master_bias.astype(np.uint16)
    bias_header.append(('NDIT',ndit,'Number of integrations'))
    bias_header.append(('TYPE','MASTER_BIAS','Median stack of dark frames'))
    #Output master frame to fits
    master_path = master_biases + 'master_bias_' \
                + str(T) + '.fits'
    fits.writeto(master_path,master_bias,bias_header)
    print('PROGRAM HAS COMPLETED')

def bad_pix_map():
    dark = cam.get_master_dark(40)
    bias = cam.get_master_bias(-60)
    
    hot_dark = np.median(dark)+5*np.std(dark)
    low_dark = 300
    dead_lim = np.median(bias)-5*np.std(bias)


    flag_mask = (bias==16383)*1
    dead_mask = (bias<dead_lim)*1
    low_mask = (dark<low_dark)*1
    hot_mask = (dark>hot_dark)*1 #*1 converts to integer bool
    bad_mask = dead_mask+hot_mask+flag_mask+low_mask
    return bad_mask
