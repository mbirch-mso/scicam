
import numpy as np
import math as m
import matplotlib.pyplot as plt
import scicam as cam
import scipy.ndimage
from scipy.stats import sigmaclip,norm,linregress
from scipy import signal,optimize
from matplotlib import rc
from skimage import exposure
import pandas as pd
import seaborn as sns
import matplotlib.mlab as mlab
import glob
import os
import astropy.io as fits
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D
from astropy.io import fits
from astropy.constants import h,c

#Folder variables
naxis1 = 1040
naxis2 = 1296
testing_dir = '//merger.anu.edu.au/mbirch/data/'
folder_path = lambda test: testing_dir + test + "/" 
sky_dir = testing_dir + 'master_sky/'
obs_dir = testing_dir + 'sky_background_mso/'

def filters():
    filters = obs_dir + 'filters'
    os.chdir(filters)
    fel1000 = pd.read_csv('FEL1000.csv')
    fel1050 = pd.read_csv('FEL1050.csv')
    fel1150 = pd.read_csv('FEL1150.csv')
    fel1350 = pd.read_csv('FEL1350.csv')
    fel1500 = pd.read_csv('FEL1500.csv')


    w_fel = fel1000['Wavelength'].to_numpy()
    T_1000 = fel1000['Transmission'].to_numpy()
    T_1050 = fel1050['Transmission'].to_numpy()
    T_1150 = fel1150['Transmission'].to_numpy()
    T_1350 = fel1350['Transmission'].to_numpy()
    T_1500 = fel1500['Transmission'].to_numpy()

    h = pd.read_csv('hmko.txt',sep="\t")
    j = pd.read_csv('jmko.txt',sep="\t")
    w_J = j['WL'].to_numpy()
    w_H = h['WL'].to_numpy()
    T_J = j['Transmission'].to_numpy()
    T_H = h['Transmission'].to_numpy()

    sky_transmit = np.loadtxt('MKO_transmission_am1_pwv5mm.txt',usecols=(0,1))
    w_t = sky_transmit[:,0]
    sky_T = sky_transmit[:,1]
    sky_T = signal.savgol_filter(sky_T, 251, 3) # window size 51, polynomial order 3

    J_approx = T_1150 - T_1350 
    Y_approx = T_1000 - T_1050

    plt.plot(w_t*1E3,sky_T*100,c='b',label='MKO Sky Transmission (AM=1,PWV=5mm)')
    plt.fill_between(w_t*1E3,sky_T*100,1,facecolor='blue', alpha=0.3)

    plt.plot(w_fel,J_approx,linewidth = 3,label = 'J Approx (1350-1150)')
    #plt.fill_between(w_fel, J_approx,1,facecolor='green', alpha=0.5)

    plt.plot(w_J*1E3,T_J,linewidth = 3,label = 'J MKO')
   # plt.fill_between(w_J, T_J,1,facecolor='blue', alpha=0.5)
    
    plt.plot(w_H*1E3,T_H,linewidth = 3,label = 'H MKO')
   # plt.fill_between(w_H, T_H,1,facecolor='purple', alpha=0.5)
    
    plt.plot(w_fel,Y_approx,linewidth = 3,label = 'Y Approx (1050-1000)')
   # plt.fill_between(w_fel, Y_approx,1,facecolor='yellow', alpha=0.5)

    plt.plot(w_fel,T_1500,linewidth = 3,  label = 'H Approx (Cut-off - 1500')
   # plt.fill_between(w_fel, T_1500,1,facecolor='red', alpha=0.5)

    plt.xlim(900,1680)
    plt.grid(True)
    plt.xlabel('Wavelength (nm)')
    plt.ylabel('Tranmission (%)')
    plt.title('NIR Filter Bands with Sky Transmission Profile')
    plt.legend(loc='best')
    plt.show()


def brightness_estimate(file,rows,cols):
    hdul = fits.open(file)
    img = hdul[0].data
    dit = hdul[0].header['DITSER']
    roi = img[rows[0]:rows[1],cols[0]:cols[1]]

    counts = np.median(roi) #[ADUs/pixel]
    counts /= (dit/1000) # Time [ADUs/s/pixel]
    counts *= 4.17 # Gain [e/s/pixel] UNCERTAIN 
    counts /= 1.24 # Plate-scale [e/s/arcsecond]
    alpha_bintel = 0.7 #optical throughput of BinTel [2x aluminium mirrors]
    counts /= alpha_bintel #Divide by throughput to get incident photons
    
    photons = 0.8*counts # QE [photons/s/arcsecond] UNCERTAIN

    
    lambda_c = 1.3E-6 #central wavelength UNCERTAIN
    e_phot = (h.value*c.value)/lambda_c
    I = photons/e_phot #[W/arcsecond]

    nu_1 = c.value/1E-6
    nu_2 = c.value/1.6E-6
    delta_nu = nu_1-nu_2

    f_nu = I/delta_nu#[W/Hz/arcsecond]
    aperture = m.pi * ((0.25/2)**2) #M1 aperture size [cm^2]
    f_nu /= aperture #[W/m^2/Hz/arcsecond]

    f_nu /= 10E-26 # [Jy/arcsecond]
    #Output ab_mag/arcsecond
    mag_ab = -2.5*m.log10(f_nu/3631)#takes [Jy/arcsecond]

    
    dreams_counts = counts * 2.48 #DREAMS plate-scale [e/s/DREAMSpixel]
    alpha_dreams = 0.4
    dreams_counts *= alpha_dreams #Adjust by optical throughput of DREAMS
    #Insert optical throughput of DREAMS

    print("Sky Brightness: {} mag/arcsecond or {} e/s/DREAMSpixel".\
        format(round(mag_ab,1),int(dreams_counts)))

def sky_hists(folder):
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    sns.set(color_codes=True)
    for i in img_list:
        hdu = fits.open(i)
        dit = (hdu[0].header['DITSER'])/1000 #DIT in seconds
        img = (hdu[0].data)/dit #convert into rate
        clipped = cam.roi_circle(img) #remove vignetting
        clipped *= 4.16 #gain adjust
        clipped ,_,_ = sigmaclip(clipped,3,3)
        sns.distplot(clipped,label='DIT={}s,$\mu={}$$e^-$/s'.format(dit,int(np.mean(clipped))))
    plt.xlabel('$e^-$/s')
    plt.ylabel('Density $\%$')
    plt.title('Sky background distribution (NDIT=10, Airmass=1)')
    plt.legend(loc='best')
    plt.show()


filters()

# img_1 = sky_dir + 'mastersky_2.0s_10stack_am1.fits'
# img_2 = sky_dir + 'mastersky_5.0s_10stack_am1.fits'
# img_3 = sky_dir + 'mastersky_10.0s_11stack_am1.fits'
# img_4 = sky_dir + 'mastersky_20.0s_10stack_am1.fits'
# brightness_estimate(img_1,rows=(385,500),cols=(650,800))
# brightness_estimate(img_2,rows=(700,876),cols=(584,840))
#brightness_estimate(img_3,rows=(385,500),cols=(650,800))
#brightness_estimate(img_4,rows=(385,500),cols=(650,800))