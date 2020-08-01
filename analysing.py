
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

#Folder variables
naxis1 = 1040
naxis2 = 1296
testing_dir = '//merger.anu.edu.au/mbirch/data/'
folder_path = lambda test: testing_dir + test + "/" 
frame_path = folder_path('camera_control/frame_time_studies')
int_path = folder_path('camera_control/int_time_studies')
frame_int_space_path = folder_path('camera_control/frame_int_space_testing')
saturation_path = folder_path('saturation_testing')
read_path = folder_path('read_testing')


def analyse_read():
    img_path = read_path + 'master_read_' \
                  + 'test1_1000'+ '.fits'
    hdu_img = fits.open(img_path)
    img = hdu_img[0].data
    header = hdu_img[0].header
    fig, axs = plt.subplots(1, 3, tight_layout=True)
    axs[0].hist(img.flatten(), bins=3000)
    axs[0].set_xlim(1100,1700)
    axs[0].set_title('Raw  $\sigma$={}ADUs'.format(int(np.std(img))))
    axs[0].set_xlabel('ADUs')
    axs[1].imshow(img,vmax=2000)
    noise_img = (img - np.mean(img))*3.07
    axs[2].hist(noise_img.flatten(), bins=3000)
    axs[2].set_xlim(-900,900)
    axs[2].set_title('Mean-subtracted/Gain Adjusted  $\sigma={}$ electrons'.format(int(np.std(noise_img))))
    axs[2].set_xlabel('$e^{-}$')
    axs[1].set_title('Master Bias Frame, DIT=$33\mu s$ (520REFCLKS), NDIT=1000')
    plt.show()

def master_sky_plot():
    bias_data = cam.get_master_bias(-40)
    squished = cam.median_stack(i,folder)
    reduced = squished - bias_data #Subtract master bias
    #Clip to 3 sigmas the squished image to remove recurring
    #dead/hot pixels
    reduce_clipped, _ , _ = sigmaclip(reduced,3,3)
    reduce_clipped_s = reduce_clipped/2
    fig, (ax1, ax2, ax3) = plt.subplots(figsize=(13, 3), ncols=3)
    
    sky = ax1.imshow(squished,vmin=1900,vmax=2600)
    ax1.set_title('Median-stacked(DIT=2s,NDIT=8)')
    fig.colorbar(sky, ax=ax1)
    
    bias_img = ax2.imshow(bias_data,vmin=1400,vmax=2000)
    ax2.set_title('Master Bias(-40C)')
    fig.colorbar(bias_img, ax=ax2)

    sky_reduced = ax3.imshow(reduced,vmin=400,vmax=700)
    ax3.set_title('Bias-subtracted')
    fig.colorbar(sky_reduced, ax=ax3)
    
    plt.suptitle('Sky background (no filter) at airmass 2: {}ADUs/pixel/s'.format(round(np.mean(reduce_clipped_s),2)))
    plt.show()

def bias_temp_analysis():

    #Base Mode
    b_1 = cam.get_master_bias(-40)
    b_2 = cam.get_master_bias(-20)
    b_3 = cam.get_master_bias(0)
    b_4 = cam.get_master_bias(20)

    #Full Mode
    f_1 = cam.get_master_bias(-42)
    f_2 = cam.get_master_bias(-22)
    f_3 = cam.get_master_bias(2)
    f_4 = cam.get_master_bias(22)

    temps = [-40,-20,0,20]
    medians_b = [np.median(b_1),np.median(b_2),np.median(b_3),np.median(b_4)]
    medians_f = [np.median(f_1),np.median(f_2),np.median(f_3),np.median(f_4)]



    plt.plot(temps,medians_b,marker='o',linestyle='--',c='g',label='Base Mode (1-tap)')
    plt.plot(temps,medians_f,marker='x',c='r',label='Full Mode (4-taps)')
    plt.ylabel('Median (ADUs)')
    plt.xlabel('Temperature ($^{\circ}$C)')
    
    plt.legend(loc='best')
    plt.title(r'$\mathrm{Bias\ results\ against\ Temperature}$')
    plt.grid(True)
    plt.show()

def read_diff(files):
    hdu1 = fits.open(imgs[0])
    hdu2 = fits.open(imgs[1])
    img1 = hdu1[0].data
    img2 = hdu2[0].data
    diff = img1 - img2 #Create difference image
    header = hdu1[0].header
    img, _, _ = sigmaclip(diff,5,5)
    sig = np.std(img)
    RN = int(sig/np.sqrt(2))
    fig, axs = plt.subplots(1, 2, tight_layout=True)
    axs[0].imshow(diff,vmax=25)
    axs[1].hist(img, bins=1000,label='$\sigma={}$ electrons'.format(int(sig)))
    axs[1].set_title('Mean-subtracted/Gain Adjusted'.format(int(np.std(img))))
    axs[1].set_xlabel('$e^{-}$')
    axs[1].set_title('Difference of Master Bias Frames, DIT=$33\mu s$ (520REFCLKS), NDIT=1000, RN={}'.format(RN))
    plt.show()
    plt.legend(loc='best')

def gain_calc():
    testing_dir = '//merger.anu.edu.au/mbirch/data/'
    folder_path = lambda test: testing_dir + test + "/" 
    gain_path = folder_path('gain_testing')
    struct = np.loadtxt(gain_path + 'study_2_results.txt')
    x = struct[0,:]
    y = struct[1,:]
    y = y[np.argwhere(y<700)]
    x = x[np.argwhere(y<700)]
    x = x[:,0]
    y = y[:,0]
    y = y[x<3000]
    x = x[x<3000]


    slope, intercept, r_value, _ ,_ = linregress(x,y)
    gain = round(slope,3)
    fit = slope*x + intercept
    slope = round(slope,2)  
    rsqr = round((r_value**2),4)
    plt.scatter(x,y,c='red',label = 'Data')
    plt.plot(x,fit,'g--',label = 'Linear Fit, $r^2$ = {}'.format(rsqr))
    plt.xlabel('Median Intensity (ADUs)')
    plt.ylabel('$\sigma^2$ (ADUs)')
    plt.legend(loc = 'best')
    plt.grid(True)
    plt.title('Variance vs Intensity Gain Study, g = {0}ADUs/$e^-$ ($n=100$)'.format(gain))
    plt.show()

def cooling_test():
    os.chdir('C:')
    os.chdir('C:/nstf/images/images19-06-2020/cooling_test_2')
    files = glob.glob('*.fits*')
    bias = cam.get_master_bias(-60)
    temps = []
    vals = []
    sigma = []
    for i in files:
        hdu = fits.open(i)
        ambtemp = hdu[0].header['AMBTEMP']
        temps.append(ambtemp)
        data = hdu[0].data
        data = data - bias
        clipped,_,_ = sigmaclip(data,5,5) #clip hot/dead pixels
        vals.append(np.median(clipped))
        sigma.append(np.std(clipped))
    

    temps = np.array(temps)
    vals = np.array(vals)
    vals = vals[temps<10]
    temps = temps[temps<10]


    vals = 3.23*vals
    vals = vals/2

    slope, intercept, r_value, _ ,_ = linregress(temps,vals)
    fit = slope*temps + intercept
    slope = int(slope)  
    rsqr = round((r_value**2),4)

    plt.scatter(temps,vals,label = 'Data',color='black')
    plt.plot(temps,fit,'g--',label = 'Linear Fit (m={0}e/s/$^\circ$C, $r^2$={1})'\
        .format(slope,rsqr))
    plt.grid(True)
    plt.legend(loc='best')
 #   plt.axvspan(26, 8, color='r', alpha=0.5, lw=0)
  #  plt.axvspan(7.9, -11, color='g', alpha=0.5, lw=0)
   # plt.text(20,180,r'$FPA\ Unstable$')
  #  plt.text(0,180,r'$FPA\ Stable$')
  #  plt.gca().invert_xaxis()
    plt.xlabel('Temperature ($^\circ$C)')
    plt.ylabel('Median $e^-$/s/pix')
    plt.title('Detector Cooling Test (FPA:-60$^\circ$C, DIT=2s)')
    plt.show()

def dark_current(folder):
    os.chdir(folder)
    files = glob.glob('*.fits*')
   # bias = cam.get_master_bias(-60)
    vals,temps,times = [] , [], []
    for i in files:
        hdu = fits.open(i)
        temps.append(hdu[0].header['TEMPAMB'])
        times.append(hdu[0].header['DITSER'])
        data = hdu[0].data
       # data = data - bias
       # data[data > 60000] = 0 #Avoid unsigned integer overflow
        clipped,_,_ = sigmaclip(data,3,3)
        vals.append(np.median(clipped))
    
    #test_temp = round(np.mean(temps),1)
    times = np.array(times)/1000
    vals = 3.22*np.array(vals)

    slope, intercept, r_value, _ ,_ = linregress(times,vals)
    fit = slope*times + intercept
    i_dark = int(round(slope))  
    rsqr = round((r_value**2),4)

    plt.scatter(times,vals/1000,c='black',label='Data ($n={}$)'.format(len(files)))
    plt.plot(times,fit/1000,c='g',linestyle='dashed',label = 'Linear Fit, $r^2$ = {}'.format(rsqr))
    plt.grid(True)
    plt.xlabel('Integration Time (s)')
    plt.ylabel('Median Pixel Value ($ke^-$)')
    plt.legend(loc='best')
    plt.title('Dark Current, {0}$e^-$/s (FPA: $0^\circ$, Shutter: 20$^\circ$C)'\
        .format(i_dark))
    plt.show()

def dark_temp_analysis():
    temp_aitc = [-60,-40,-20,0]
    dark_aitc = [51,419,2199,7100,]
    temp_sull = [-50,-40,-30,-20,-10,0]
    dark_sull = [163,500,1000,3000,10000,30000]
    dark_pirt = [300,28000]
    temp_pirt = [-60,20]
    dark_fpa = [160]
    temp_fpa = [-50]
    def exp(x,a,b):
        return a*np.exp(b*(x))
    temps = np.linspace(-60,20,5000)
    popt_1, pcov_1 = optimize.curve_fit(exp, temp_aitc, dark_aitc,p0=[30000,1])
    popt_2, pcov_2 = optimize.curve_fit(exp, temp_sull, dark_sull,p0=[30000,1])
    k_aitc = round(popt_1[1],2)
    k_sull = round(popt_2[1],2)
    a_aitc = int(popt_1[0])
    a_sull = int(popt_2[0])

    perr_1 = np.sqrt(np.diag(pcov_1))
    perr_2 = np.sqrt(np.diag(pcov_2))
    a_aitc_err = int(perr_1[0])
    a_sull_err = int(perr_2[0])

    props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)

    def targ_temp(I,A,k):
        return (m.log(I/A))/k
    return_temp = targ_temp(100,popt_1[0],popt_1[1])
    print('Temp is {}'.format(return_temp))

    temps_fine = np.linspace(-80,20,5000)

    plt.plot(temps_fine, (exp(temps_fine, *popt_1)),'r--',label="$A: {0}\pm{1},\:k: {2}\pm0.03$"\
        .format(a_aitc,a_aitc_err,k_aitc))
    plt.plot(temps_fine, (exp(temps_fine, *popt_2)),'g--',label="$A: {0}\pm{1},\:k: {2}\pm0.01$"\
        .format(a_sull,a_sull_err,k_sull))
    

    plt.scatter(temp_pirt,dark_pirt,c= 'k',marker='*',label = 'PIRT 1280SCICAM (Vendor Sheet)')
    plt.scatter(temp_aitc,dark_aitc,c= 'red',marker='s',label = 'PIRT 1280SCICAM')
    plt.scatter(temp_sull,dark_sull,c = 'green',marker = "D",label='FLIR AP1121 (Sullivan et. al. 2014)')
    plt.scatter(temp_fpa,dark_fpa,c= 'm',marker='o',label = 'PIRT 1280A1-12 (FPA)')
    plt.text(-10,100,r'$I_{dark}\approx Ae^{kT}$',size=20,bbox=props)
    plt.grid(True)
    plt.xlabel('FPA Temperature [$^\circ$C]')
    plt.ylabel('Dark Current [$e^-$/s/pix]')
    plt.yscale('log')
    plt.legend(loc='best')
    plt.title('Dark Current against Temperature for InGaAs FPAs')
    plt.show()

def shutter_temp_analysis():

    temps = [-5,-10.6,-13]
    I = [54,51,50]
   # def exp(x,a,b,c):
    #    return c + a*np.exp(b*(x+273.15))
   # popt, _ = optimize.curve_fit(exp, temps, fluxes,p0=[1E-6,1E-6,1E-6])
    plt.scatter(temps,I)
    plt.show()

def master_dark_hist():
    
    os.chdir('//merger.anu.edu.au/mbirch/data/master_dark')
    files = glob.glob('*-10.6C.fits*')
    sns.set(color_codes=True)
    for i in files:
        hdu = fits.open(i)
        int_t = (hdu[0].header['DITSER'])/1000
        img = (hdu[0].data)/int_t
        clipped,_,_ = sigmaclip(img,5,5)
        clipped = clipped*3.23
        val = round(np.median(clipped))
        sns.distplot(clipped,label='DIT={}s,$\mu={}$'.format(int_t,val))

    plt.xlabel('ADUs/s')
    plt.ylabel('Density $\%$')
    plt.title('Master Dark Pixel Distributions (NDIT=20, FPA:-60$^\circ$C, Shutter:-10.6$^\circ$C)')
    plt.legend(loc='best')
    plt.show()

def dark_bad_pix_var(folder,i):
    bias = cam.get_master_bias(-60)
    os.chdir(folder)

    i = i*1000 #convert to ms
    
    img_list = glob.glob('*.fits*')
    img_list_split = [i.split('_') for i in img_list]
    low_map = np.zeros((naxis1,naxis2))
    very_low_map = np.zeros((naxis1,naxis2))
    hot_map = np.zeros((naxis1,naxis2))
    very_hot_map = np.zeros((naxis1,naxis2))

    c_bias = 1951*np.ones((naxis1,naxis2))
    n = 0
    for k in range(len(img_list)):
        if img_list_split[k][1] == str(i):
            hdu = fits.open(img_list[k])
            data = hdu[0].data
            dark = data - bias
            np.asarray(dark,dtype=np.float64)
            print(np.min(dark))

           # hot_dark = np.median(dark)+6*np.std(dark)
            low_map += ((dark<300) & (dark>0))*1
            very_low_map += (dark<0)*1
            hot_map += ((dark>2000) & (dark<5000))*1
            very_hot_map += (dark>5000)*1
            print(len((very_low_map[very_low_map!=0])))
            n+= 1

    hot_dist = (hot_map[hot_map!=0])/n
    very_hot_dist = (very_hot_map[very_hot_map!=0])/n
    low_dist = (low_map[low_map!=0])/n
    very_low_dist = (very_low_map[very_low_map!=0])/n

    print(very_low_dist)
    #plt.hist(hot_dist,facecolor='g',hatch='/', edgecolor='k',fill=True, alpha=0.5,\
    #    label=r'High $I_{dark}$')
    #plt.hist(very_hot_dist,facecolor='y',hatch='|', edgecolor='k',fill=True, alpha=0.5,\
     #   label=r'Very High $I_{dark}$')
    #plt.hist(low_dist,hatch='*', facecolor='c',edgecolor='k',fill=True,alpha=0.5,\
     #   label=r'Low $I_{dark}$')
    plt.hist(very_low_dist,hatch='o', facecolor='r',edgecolor='k',fill=True,alpha=0.5,\
        label=r'Very Low $I_{dark}$')
    plt.xlabel('Recurrance Rate (%)')
    plt.ylabel('# of Pixels')
    plt.legend(loc='best')
    plt.title('Recurrance of bad pixels on dark frames(n={},DIT=40s)'.format(n))
    plt.show()
    print('Program Complete')

def bias_bad_pix_var(folder):
    
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    
    low_map = np.zeros((naxis1,naxis2))
    very_low_map = np.zeros((naxis1,naxis2))
    hot_map = np.zeros((naxis1,naxis2))
    very_hot_map = np.zeros((naxis1,naxis2))
    flag_map = np.zeros((naxis1,naxis2))
    dead_map = np.zeros((naxis1,naxis2))
    n = len(img_list)
    
    for k in img_list:
        hdu = fits.open(k)
        data = hdu[0].data
        l = np.median(data)-5*np.std(data)
        h = np.median(data)+5*np.std(data)

        dead_map += (data==15)*1
        very_low_map += ((data>15) & (data<700))*1
        low_map += ((data>700) & (data<l))*1
        hot_map += ((data>h) & (data<6000))*1
        very_hot_map += ((data>6000) & (data<16383))*1
        flag_map += (data==16383)*1
        print(len(very_low_map[very_low_map!=0]),k)
    
    print(flag_map)

    hot_dist = (hot_map[hot_map!=0])
    very_hot_dist = (very_hot_map[very_hot_map!=0])
    low_dist = (low_map[low_map!=0])
    very_low_dist = (very_low_map[very_low_map!=0])
    dead_dist = (dead_map[dead_map!=0])
    flag_dist = (flag_map[flag_map!=0])
    print(flag_dist)
    print(dead_dist)
    print(flag_map)

    # plt.hist(hot_dist,bins=100,facecolor='g',hatch='/', edgecolor='k',fill=True, alpha=0.5,\
    #     label=r'High Bias')
    # plt.hist(very_hot_dist,bins=100,facecolor='y',hatch='|', edgecolor='k',fill=True, alpha=0.5,\
    #     label=r'Very High Bias')
    # plt.hist(low_dist,bins=100,hatch='*', facecolor='c',edgecolor='k',fill=True,alpha=0.5,\
    #     label=r'Low Bias')
    # plt.hist(very_low_dist,bins=100,hatch='+', facecolor='r',edgecolor='k',fill=True,alpha=0.5,\
    #     label=r'Very Low Bias')
    # plt.hist(flag_dist,bins=100,hatch='.', facecolor='m',edgecolor='k',fill=True,alpha=0.5,\
    #     label=r'Flag')
    # plt.hist(dead_dist,bins=100,hatch='o', facecolor='k',edgecolor='k',fill=True,alpha=0.5,\
    #     label=r'Dead')
    # plt.xlabel('Recurrance Rate (%)')
    # plt.ylabel('# of Pixels')
    # plt.legend(loc='best')
    # plt.title('Recurrance of bad pixels on bias frames(n={},DIT=40s)'.format(n))
    # plt.show()
    print('Program Complete')

def bias_temp_var(folder):

    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    stack = np.zeros((naxis1,naxis2),dtype=np.uint16)

    for i in img_list:
        hdu_img = fits.open(i)
        data = hdu_img[0].data
        stack = np.dstack((stack,data))

    stack = stack[:,:,1:] #Slice off base layer
    temp_stack = np.var(stack, axis=2)
    plt.imshow(temp_stack)
    plt.show()

def full_well_hist(folder):
    
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40)
    
    stack = np.zeros((naxis1,naxis2),dtype=np.uint16)
    
    for i in img_list:   
        hdu_img = fits.open(i)
        data = hdu_img[0].data
        data = data - bias
        data[data > 60000] = 0 #Avoid unsigned integer overflow
        stack = np.dstack((stack,data))
    
    stack = stack[:,:,1:] #Remove 0 array it is stacked on
    collapsed = np.median(stack, axis=2)
    clipped,_,_ = sigmaclip(collapsed,8,8)
    #clipped *= 3.22 #gain-adjust
    median = np.median(clipped)
    sigma = np.std(clipped)
    early_line = median-3*sigma #3sigma away from distribution full-well begins
    
    fig,axs = plt.subplots(nrows=1,ncols=2)
    
    axs[0].axvline(early_line,linestyle='--',c='r',label='$3\sigma$ Full Well: {}ADUs'.format(int(early_line)))
    axs[0].axvline(median,linestyle='--',c='blue',label='Median: {}ADUs'.format(int(median)))
    
    axs[0].hist(clipped,bins=110,facecolor='g',edgecolor='k',fill=True)
    axs[0].set_yscale('log')
    axs[0].set_ylabel('No. of Pixels')
    axs[0].set_xlabel('ADUs')
    axs[0].grid(True)
    axs[0].legend(loc='best')
    
    img_eq = exposure.equalize_hist(data)
    axs[1].imshow(img_eq)
    plt.suptitle('Full-well Study of Oversaturated Frames (DIT={}s,NDIT={})'.format(20,70))
    plt.show()

def var_time(folder):
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40)
    bias = np.asarray(bias, dtype=np.int32)
    times = []
    for k in img_list:
        hdu = fits.open(k)
        times.append(hdu[0].header['DITSER'])
    times = np.unique(times)
    #sort times if order is needed
    sets = [[] for _ in times]
    for j in img_list:
        hdu = fits.open(j)
        dit = hdu[0].header['DITSER']
        ind = np.argwhere(times==dit)
        sets[ind[0,0]].append(hdu[0].data)
    
    var = []
    for i in sets:
        first = i[1].astype(np.int32)
        second = i[0].astype(np.int32)

        diff_img = first - second
        roi_diff = diff_img[400:800,400:800]

        var.append(np.var(roi_diff))

    var = np.array(var)/2

    plt.scatter(times,var,c='blue',label = 'Data')

    plt.ylabel('$\sigma^2$ (ADUs)')
    plt.xlabel('Integration Time (ms)')
    plt.grid(True)
    plt.title('Variance vs Integration Time')
    plt.legend(loc='best')
    plt.show()
    print('PROGRAM HAS COMPLETED')

def ramp_plot(folder):
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40)
    bias = np.asarray(bias, dtype=np.int32)
    times,amp = [],[]

    for i in img_list:
        hdu = fits.open(i)
        data = hdu[0].data
        data = np.asarray(data, dtype=np.int32)
       # data = data - bias

        times.append(hdu[0].header['DITSER'])

        #Dark Region
        roi_single_d = data[400:800,400:800]
        amp.append(np.median(roi_single_d))

    amp = np.array(amp)

    plt.scatter(times,amp,c='blue',label = 'Data')
    plt.xlabel('Integration Time (ms)')
    plt.ylabel('Intensity (ADUs)')
  #  plt.xscale('log')
  #  plt.yscale('log')
    plt.grid(True)
    plt.title('Integration Ramp')
    plt.legend(loc='best')
    plt.show()
    print('PROGRAM HAS COMPLETED')

def pixel_population_ramp(folder):
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40)
    bias = np.asarray(bias, dtype=np.int32)
    times,pixels = [],[]
    stack = np.zeros((10,10),dtype=np.int32)
    for i in img_list:
        hdu = fits.open(i)
        data = hdu[0].data
        data = np.asarray(data, dtype=np.int32)
        data = data - bias

        times.append(hdu[0].header['DITSER'])
        #Bright Region
        roi = data[790:800,790:800]
        stack = np.dstack((stack,roi))
    
    stack = stack[:,:,1:] #Slice off base layer
    
    for i in range(stack.shape[0]):
        for j in range(stack.shape[1]):
            pix_array = stack[i,j,:]
            plt.scatter(times,pix_array)

    plt.xlabel('Integration Time (ms)')
    plt.ylabel('Intensity (ADUs)')
    plt.grid(True)
    plt.title('Integration Ramp for 100 pixel population')
    plt.show()
    print('PROGRAM HAS COMPLETED')

def ptc_gain(folder):
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40)
    bias = np.asarray(bias, dtype=np.int32)
    times = []
    for k in img_list:
        hdu = fits.open(k)
        times.append(hdu[0].header['DITSER'])
    times = np.unique(times)
    print('times retrieved')
    #sort times if order is needed
    sets = [[] for _ in times]
    for j in img_list:
        hdu = fits.open(j)
        dit = hdu[0].header['DITSER']
        ind = np.argwhere(times==dit)
        sets[ind[0,0]].append(hdu[0].data)
    print('sets constructed')
    amp,var = [],[]
    
    for i in sets:
        first = i[1].astype(np.int32)
        second = i[0].astype(np.int32)
        diff_img = first - second
        first = first - bias
        roi_diff = diff_img[400:800,400:800]
        roi_single = first[400:800,400:800]
        var.append(np.var(roi_diff))
        amp.append(np.median(roi_single))
    print('arrays finished')
    var = np.array(var)/2
    amp = np.array(amp)

    plt.scatter(amp,var)#,label = 'Data')
    
    # slope, intercept, r_value, _ ,_ = linregress(x,y)
    # fit = slope*x + intercept
    # plt.plot(x,fit)
    
   # rsqr = round((r_value**2),4)

 #   plt.plot(amp,fit,'g',label = 'Linear Fit, $r^2$ = {}'.format(rsqr))

    # def fixed(x,b):
    #     return 0.31*x+b
    # popt1, _ = optimize.curve_fit(fixed,x,y)
    # plt.plot(amp, fixed(amp, *popt1), 'r-',label='Pre-determined gain slope fit (m=0.31)')

    plt.ylabel('$\sigma^2$ (ADUs)')
    plt.xlabel('Median Pixel Value (ADUs)')
    plt.grid(True)
    plt.title('Variance vs Intensity')# Gain Study 2 (SLD), g = {}ADUs/$e^-$ (n = {})'.format(round(slope,2),len(x)))
   # plt.legend(loc='best')

def median_ptc(folder): 
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40)
    bias = np.asarray(bias, dtype=np.int32)
    times = []
    for k in img_list:
        hdu = fits.open(k)
        times.append(hdu[0].header['DITSER'])
    times = np.unique(times)
    sets = [[] for _ in times]
    for j in img_list:
        hdu = fits.open(j)
        dit = hdu[0].header['DITSER']
        ind = np.argwhere(times==dit)
        sets[ind[0,0]].append(hdu[0].data)
    amp,var = [],[]
    
    for i in sets:
        print(len(i))
        med,var_ele = [],[]
        for j in range(0,len(i),2): #select every second array
            first = i[j].astype(np.int32)
            second = i[j+1].astype(np.int32)
            diff_img = first - second
            roi_diff = diff_img[400:800,400:800]
            roi_single = first[400:800,400:800]
            med.append(np.median(roi_single))
            var_ele.append(np.var(roi_diff))

        var.append(np.mean(var_ele))
        amp.append(np.mean(med))
    
    var = np.array(var)/2
    amp = np.array(amp)

    plt.scatter(amp,var,marker='d',label='Median Results (n=20)')
    plt.ylabel('$\sigma^2$ (ADUs)')
    plt.xlabel('Median Pixel Value (ADUs)')
    plt.grid(True)
    plt.legend(loc='best')
    plt.title('Variance vs Intensity')
    plt.show()

def ptc(folder):
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40)
    bias = np.asarray(bias, dtype=np.int32)
    times = []
    for k in img_list:
        hdu = fits.open(k)
        times.append(hdu[0].header['DITSER'])
    times = np.unique(times)
    #sort times if order is needed
    sets = [[] for _ in times]
    for j in img_list:
        hdu = fits.open(j)
        dit = hdu[0].header['DITSER']
        ind = np.argwhere(times==dit)
        sets[ind[0,0]].append(hdu[0].data)
    amp,  noise= [],[]
    for i in sets:
        first = i[1].astype(np.int32)
        second = i[0].astype(np.int32)

        diff_img = first - second
        single = first - bias

        roi_diff = diff_img[200:900,300:1000]
        roi_single = single[200:900,300:1000]

        noise.append(np.std(roi_diff))
        amp.append(np.median(roi_single))

    noise = np.array(noise)/m.sqrt(2)
    amp = np.array(amp)
    amp = np.log10(amp)
    noise = np.log10(noise)

    plt.scatter(amp,noise,c='b',label = 'Data')
    x1 = amp[(amp<4) & (amp>3.4)] 
    y1 = noise[(amp<4) & (amp>3.4)] 
    x = np.linspace(-1,5,500)

    x2 = amp[(amp<4) & (amp>3.8)] 
    y2 = noise[(amp<4) & (amp>3.8)]

    def fixed(x,b):
        return 0.5*x+b
    popt1, _ = optimize.curve_fit(fixed,x1,y1)
    plt.plot(x, fixed(x, *popt1), 'r-',label='1/2 Slope Fit 1')
    
 
    popt2, _ = optimize.curve_fit(fixed,x2,y2)
   # plt.plot(x, fixed(x, *popt2), 'g-',label='1/2 Slope Fit 2')
    

    plt.axhline(1.255,c='m',linestyle='--',label='19ADU Read-noise floor')
    plt.ylabel('Noise (log(ADUs))')
    plt.xlabel('Intensity (log(ADUs))')
    plt.grid(True)
    plt.title('Photon Transfer Curve (g=3.34$e^-$/ADU)')
    plt.legend(loc='best')
    plt.show()

def temp_var(folder):

    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40) #Retrieve bias
    bias = bias.astype(np.int32)
    stack = np.zeros((naxis1,naxis2),dtype=np.int32)
    for i in img_list:
            hdu = fits.open(i)
            data = hdu[0].data
            data = data.astype(np.int32)
            data -= bias
            stack = np.dstack((stack,data))

    
    stack = stack[:,:,1:] #Slice off base layer
    var_map = np.var(stack, axis=2)

    dark = cam.get_master_dark(40)
    bias = cam.get_master_bias(-60)
    
    nonlinear_mask = (var_map>50000)*1

   # var_map = var_map[var_map<0.6E6]
    plt.hist(var_map.flatten(),bins=200)
    plt.yscale('log')
    plt.grid(True)
    plt.xlabel('$\sigma^2$ (ADUs)')
    plt.ylabel('No. of pixels')
    plt.title('Distribution of temporal variability (DIT=450ms,NDIT=30)')
    plt.show()
    print('PROGRAM HAS COMPLETED')
    return nonlinear_mask

def stack_hists(folder):
    
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40) #Retrieve bias
    bias = bias.astype(np.int32)
    for i in img_list:
            hdu = fits.open(i)
            data = hdu[0].data
            data = data.astype(np.int32)
            data -= bias
            data = data[400:800,400:800]
            plt.hist(data.flatten(),bins=100)

    plt.yscale('log')
    plt.grid(True)
    plt.xlabel('ADUs')
    plt.ylabel('No. of pixels')
    plt.title('Pixel Distribution Histograms (NDIT=30)')
    plt.show()

def nonlinearity(folder):
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40)
    bias = np.asarray(bias, dtype=np.int32)
    times = []
    for k in img_list:
        hdu = fits.open(k)
        times.append(hdu[0].header['DITSER'])
    times = np.unique(times)
    sets = [[] for _ in times]
    for j in img_list:
        hdu = fits.open(j)
        dit = hdu[0].header['DITSER']
        ind = np.argwhere(times==dit)
        sets[ind[0,0]].append(hdu[0].data)
    amp,var = [],[]
    
    for i in sets:
        first = i[1].astype(np.int32)
        second = i[0].astype(np.int32)
        diff_img = first - second
        first = first - bias
        roi_diff = diff_img[400:800,400:800]
        roi_single = first[400:800,400:800]
        var.append(np.var(roi_diff))
        amp.append(np.median(roi_single))
    var = np.array(var)/2 #Adjust var
    amp = np.array(amp)

    #Linear Region
    lin_y = var[(amp>2000) & (amp<8000)]
    lin_x = amp[(amp>2000) & (amp<8000)]



    slope, intercept, r_value, _ ,_ = linregress(lin_x,lin_y)
    fit = slope*amp + intercept
    rsqr = round(r_value**2,3)
    m = round(slope,2)
    b = int(intercept)

    x = np.linspace(amp[0],amp[-1],5000)
    noise = (20**2) + np.sqrt(x)
    nonlin = 11600
    fwell = 13480
    fig1 = plt.figure(1)
    frame1=fig1.add_axes((.1,.3,.8,.6))
    plt.plot(amp,fit,linestyle='--',c='g',label='Linear fit: $r^2$ = {}, m = {}, b = {}'.format(rsqr,m,b))
    plt.scatter(amp,var,label = 'Data (n={})'.format(len(times)))
    plt.axvline(nonlin,c='m',label = '$4\%$ Non-linearity Point: {}ADUs'.format(nonlin))
    plt.axvline(fwell,c='y',label = 'Full-well: {}ADUs'.format(fwell))
    plt.xlim(11000,13600)
    plt.ylim(0,4000)
    plt.ylabel('$\sigma^2$ (ADUs)')
    plt.title('Linear photon transfer curve with fit residuals')
    plt.legend(loc='best')
    frame1.set_xticklabels([])
    plt.grid()

    #Residual plot
    resids = fit - var
    frame2 = fig1.add_axes((.1,.1,.8,.2))        
    plt.plot(amp,resids,'or')
    plt.fill_between(x, -noise, noise, alpha=0.2,label = 'Shot/Read Noise')
    plt.ylabel('$\sigma^2$ Residuals (ADUs)')
    plt.xlabel('Median Pixel Value (ADUs)')
    plt.axvline(nonlin,c='m',label = '$4\%$ Non-linearity Point: {}ADUs'.format(nonlin))
    plt.axvline(fwell,c='y',label = 'Full-well: {}ADUs'.format(fwell))
    plt.grid()
    plt.ylim(-1000,4000)
    plt.xlim(11000,13600)
    plt.legend(loc='best')
    plt.show()

def bias_config_diff():
    #Base Mode
    b_1 = cam.get_master_bias(-40)
    b_2 = cam.get_master_bias(-20)
    b_3 = cam.get_master_bias(0)
    b_4 = cam.get_master_bias(20)

    #Full Mode
    f_1 = cam.get_master_bias(-42)
    f_2 = cam.get_master_bias(-22)
    f_3 = cam.get_master_bias(2)
    f_4 = cam.get_master_bias(22)


    b_1 = b_1.astype(np.int32)
    b_2 = b_2.astype(np.int32)
    b_3 = b_3.astype(np.int32)
    b_4 = b_4.astype(np.int32)

    f_1 = f_1.astype(np.int32)
    f_2 = f_2.astype(np.int32)
    f_3 = f_3.astype(np.int32)
    f_4 = f_4.astype(np.int32)

    img1 = b_1 - f_1
    img2 = b_2 - f_2
    img3 = b_3 - f_3
    img4 = b_4 - f_4

    img_eq1 = exposure.equalize_hist(img1)
    img_eq2 = exposure.equalize_hist(img2)
    img_eq3 = exposure.equalize_hist(img3)
    img_eq4 = exposure.equalize_hist(img4)

    fig, axs = plt.subplots(nrows = 1, ncols= 4)

    axs[0].imshow(img1)
    axs[0].set_title('Temp: {}$^\circ$C, Median Difference: {}'.format(-40,int(np.median(img1))))
    
    axs[1].imshow(img2)
    axs[1].set_title('Temp: {}$^\circ$C, Median Difference: {}'.format(-20,int(np.median(img2))))

    
    axs[2].imshow(img3)
    axs[2].set_title('Temp: {}$^\circ$C, Median Difference: {}'.format(0,int(np.median(img3))))

    
    axs[3].imshow(img4)
    axs[3].set_title('Temp: {}$^\circ$C, Median Difference: {}'.format(20,int(np.median(img4))))

    plt.suptitle('Base-Full Bias Difference Images (DIT=33$\mu$s, NDIT=100)')
    plt.show()
    plt.tight_layout()

def fringing_analysis():

 #   fig,axs = plt.subplots(nrows=1,ncols=3)

    def fringing_stack(folder,T,band):

        os.chdir(folder)
        img_list = glob.glob('*.fits*')

        bias = cam.get_master_bias(T)
        bias = bias.astype(np.int32)

        stack = np.zeros((naxis1,naxis2),dtype=np.int32)
        for i in img_list:
                hdu = fits.open(i)
                frame = hdu[0].data
                frame = frame.astype(np.int32)
                frame = frame - bias
                stack = np.dstack((stack,frame))

        stack = stack[:,:,1:] #Slice off base layer
        img = np.median(stack, axis=2)
        img_save = np.copy(img)
        masked = cam.roi_circle(img)
        norm_masked = masked/np.max(masked)
       # plt.hist(norm_masked,bins=300,label=band)

        return img_save
    
    h_img = fringing_stack('//merger.anu.edu.au/mbirch/images/images22-07-2020/hnarrowband_0.8s_1',-40,'H-band (1650$\pm$10nm)')
   # j_img = fringing_stack('//merger.anu.edu.au/mbirch/images/images22-07-2020/jnarrowband_0.8s_1',-40,'J-band (1250$\pm$10nm)')



    # plt.yscale('log')
    # plt.legend(loc='best')
    # plt.grid(True)
    # plt.xlabel('Normalised Pixel Intensity')
    # plt.ylabel('# of Pixels')
    # plt.title('Pixel distribution with J/H narrowband filters')
    # plt.show()

    # h_norm = h_img/np.max(h_img)
    # j_norm = j_img/np.max(j_img)

    # diff_map = j_norm - h_norm
    # diff_eq = exposure.equalize_hist(diff_map)

    # axs[0].imshow(diff_eq)
    # axs[0].set_title('J/H difference image')
    # axs[0].axis('off')

    diff_h = exposure.equalize_hist(h_img)
    # axs[1].imshow(diff_h)
    # axs[1].set_title('H-band median-stacked image')
    # axs[1].axis('off')

    # diff_j = exposure.equalize_hist(j_img)
    # axs[2].imshow(diff_j)
    # axs[2].set_title('J-band median-stacked image')
    # axs[2].axis('off')

    # plt.suptitle('J/H Narrowpass images')
    # plt.show()

    plt.imshow(diff_h)
    plt.axis('off')
    plt.show()

def dark_gain_method(dk_folder,ff_folder):
    
    os.chdir(dk_folder)
    dk_list = glob.glob('*.fits*')

    os.chdir(ff_folder)
    ff_list = glob.glob('*.fits*')
    gains = []
    for i in range(0,len(dk_list),2):
        
        os.chdir(dk_folder)
        dk_hdu1 = fits.open(dk_list[i])
        dk_hdu2 = fits.open(dk_list[i+1])
        
        dk_img1 = dk_hdu1[0].data
        dk_img1 = dk_img1.astype(np.int32)

        
        dk_img2 = dk_hdu2[0].data
        dk_img2 = dk_img2.astype(np.int32)


        os.chdir(ff_folder)
        ff_hdu1 = fits.open(ff_list[i])
        ff_hdu2 = fits.open(ff_list[i+1])
        
        ff_img1 = ff_hdu1[0].data
        ff_img1 = ff_img1.astype(np.int32)


        ff_img2 = ff_hdu2[0].data
        ff_img2 = ff_img2.astype(np.int32)

        dk_img1 = dk_img1[500:700,500:700]
        dk_img2 = dk_img2[500:700,500:700]
        ff_img1 = ff_img1[500:700,500:700]
        ff_img2 = ff_img2[500:700,500:700]

        ff_diff = ff_img2 - ff_img1
        dk_diff = dk_img2 - dk_img1

        ff_var = np.var(ff_diff)
        dk_var = np.var(dk_diff)
        var_denom = (ff_var - dk_var)/2

        ff_med1 = np.median(ff_img1)
        ff_med2 = np.median(ff_img2)
        
        dk_med1 = np.median(dk_img1)
        dk_med2 = np.median(dk_img2)

        ff_med = ff_med1 + ff_med2
        dk_med = dk_med1 + dk_med2
        med_numer = (ff_med - dk_med)/2
        
        k = med_numer/var_denom
        gains.append(k)
    print(gains)
    print("Gain[ADU/e]: {}".format(np.mean(gains)))
    #create flat diff imgs
    #create dark diff imgs
    #subtract variance of diff imgs and divide by 2

    #median of each pair and add for flat
    #median of each pair and add for dark
    #subtract the added dark median from the added flat median and divide by 2

    #divide this by the variance product
    #output is K[ADU/election]

def crosstalk(folder):
    img = '/crosstalk_2.fits'
    img_dir = folder+img
    hdu = fits.open(img_dir)
    frame = hdu[0].data
    bias = cam.get_master_bias(-40)
    bias = bias.astype(np.int32)
    dark = cam.get_master_dark(20,-40)
    dark = dark.astype(np.int32)

    frame = frame - bias - dark
    
    fig,axs = plt.subplots(nrows=1,ncols=2)

    cut_region = range(200,205)
    cuts = frame[:,199]
    for i in cut_region:
        cut = frame[:,i]
        cuts = np.vstack((cuts,cut))

    cut_median = np.median(cuts,axis=0)
    axs[1].plot(cut_median)
    axs[1].set_ylabel('ADUs')
    axs[1].set_xlabel('y-Index')
    axs[0].imshow(frame)

    plt.suptitle('Vertical Profile at x = {}'.format(cut_region[0]))
    plt.show()

def dark_nonuniformity(folder):
    os.chdir(folder)
    dk_list = glob.glob('*.fits*')
    dk_list.sort()
    
    bias_20 = cam.get_master_bias(-20)
    bias_40 = cam.get_master_bias(-40)
    bias_60 = cam.get_master_bias(-60)

    img_20 = cam.fits_extract(dk_list[0])
    img_40 = cam.fits_extract(dk_list[1])
    img_60 = cam.fits_extract(dk_list[2])

    img_20 -= bias_20
    img_40 -= bias_40
    img_60 -= bias_60

    norm_20 = img_20/np.mean(img_20)
    norm_40 = img_40/np.mean(img_40)
    norm_60 = img_60/np.mean(img_60)

    norm_20 = norm_20[100:1000,100:1000]
    norm_40 = norm_40[100:1000,100:1000]
    norm_60 = norm_60[100:1000,100:1000]
    
    xx, yy = np.mgrid[0:norm_20.shape[0], 0:norm_20.shape[1]] #coords for images

    fig = plt.figure()

    ax = fig.add_subplot(1, 3, 1, projection='3d')
    surf = ax.plot_surface(xx,yy,norm_20, cmap='twilight_shifted', linewidth=0.2)
    ax.set_zlim(0.5,1.5)
    ax.axes.xaxis.set_ticks([])
    ax.axes.yaxis.set_ticks([])
    ax.set_title('FPA: -20$^\circ$C, $\sigma$={}'.format(round(np.std(norm_20),1)))
    ax.set_zlabel('Normalised Pixel Intensity')

    ax = fig.add_subplot(1, 3, 2, projection='3d')
    surf = ax.plot_surface(xx,yy,norm_40, cmap='twilight_shifted', linewidth=0.2)
    ax.set_title('FPA: -40$^\circ$C, $\sigma$={}'.format(round(np.std(norm_40),1)))
    ax.axes.xaxis.set_ticks([])
    ax.axes.yaxis.set_ticks([])
    ax.set_zlabel('Normalised Pixel Intensity')

    ax = fig.add_subplot(1, 3, 3, projection='3d')
    surf = ax.plot_surface(xx,yy,norm_60, cmap='twilight_shifted', linewidth=0.2)
    ax.set_title('FPA: -60$^\circ$C, $\sigma$={}'.format(round(np.std(norm_60),1)))
    ax.axes.xaxis.set_ticks([])
    ax.axes.yaxis.set_ticks([])
    ax.set_zlabel('Normalised Pixel Intensity')
    
    plt.tight_layout()
    plt.suptitle('Dark Current Spatial Non-uniformity with FPA Temperature')
    plt.show()



dark_nonuniformity('//merger.anu.edu.au/mbirch/data/dark_current/non_uniformity')
