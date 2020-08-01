
import numpy as np
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


def read_ramp():
    struct = np.loadtxt(read_path + '200on200_test1_transition' + '_results.txt')
    sigma = [0.8, 0.8]
    means = struct[0,:]
    means = means/200
    stddevs = struct[3,:]
    stddevs = stddevs/200
    x = struct[4,:]
    x = x
    cubic_coefs = np.polyfit(x, means, 3)
    cubic = np.poly1d(cubic_coefs)
    
    fig, ax1 = plt.subplots()
    
    color = 'tab:red'
    ax1.set_xlabel('Frame Time ($ms$)')
    ax1.set_ylabel('Mean (ADUs)', color=color)
    ax1.plot(x,means, color=color)
    ax1.plot(x,cubic(x),label = 'Cubic Fit',color='green')
    ax1.legend(loc='best')
    ax1.tick_params(axis='y', labelcolor=color)

    ax2 = ax1.twinx()
    color = 'tab:blue'
    ax2.set_ylabel('$\sigma$ (ADUs)', color=color)  # we already handled the x-label with ax1
    ax2.plot(x, stddevs, color=color)
    ax2.tick_params(axis='y', labelcolor=color)
    plt.suptitle("Read to dark noise regime transition ({} frames per data point)".format(len(x)))
    plt.show()

def saturation_plot():
    struct = np.loadtxt(saturation_path + 'saturation_20loops_50pnts_1' + '_results.txt')
    means = struct[0,:]
    maxs = struct[5,:]
    x = struct[6,:]
    plt.plot(x,means,label = 'Mean')
    plt.axhline(means[-1],c='r',ls = '--',label ='Effective Saturation Value: {}'.format(15635))
    plt.axhline(16385,c='b',ls = '--',label ='Bad pixel value: {}'.format(16385))
    plt.xlabel('Integration time (ms)')
    plt.ylabel('ADUs')
    plt.legend(loc = 'best')
    plt.title("Detector Saturation")
    plt.show()

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

def hist_analysis_sky():
    m2 = '//merger.anu.edu.au/mbirch/data/sky_background_offsite/11-5-2020/zenith_first_test_h/mastersky_2.0s_10stack_am1.fits'
    m5 = '//merger.anu.edu.au/mbirch/data/sky_background_offsite/11-5-2020/zenith_first_test_h/mastersky_5.0s_10stack_am1.fits'
    m10 = '//merger.anu.edu.au/mbirch/data/sky_background_offsite/11-5-2020/zenith_first_test_h/mastersky_10.001s_10stack_am1.fits'
    m20 = '//merger.anu.edu.au/mbirch/data/sky_background_offsite/11-5-2020/zenith_first_test_h/mastersky_20.0s_10stack_am1.fits'
    hdu2 = fits.open(m2)
    hdu5 = fits.open(m5)
    hdu10 = fits.open(m10)
    hdu20 = fits.open(m20)
    
    clipped2 = cam.roi_clip(hdu2[0].data)/2 
    clipped5 = cam.roi_clip(hdu5[0].data)/5
    clipped10 = cam.roi_clip(hdu10[0].data)/10
    clipped20 = cam.roi_clip(hdu20[0].data)/20
    sns.set(color_codes=True)
    sns.distplot(clipped2,label='DIT=2s,$\mu={}$'.format(int(np.mean(clipped2))))
    sns.distplot(clipped5,label='DIT=5s,$\mu={}$'.format(int(np.mean(clipped5))))
    sns.distplot(clipped10,label='DIT=10s,$\mu={}$'.format(int(np.mean(clipped10))))
    sns.distplot(clipped20,label='DIT=20s,$\mu={}$'.format(int(np.mean(clipped20))))
    #sns.distplot(clip_avg,label='Average,$\mu={}$'.format(int(np.mean(clip_avg))))
    plt.xlabel('ADUs/s')
    plt.ylabel('Density $\%$')
    plt.title('H-band sky background distribution (NDIT=10, Airmass=1)')
    plt.legend(loc='best')
    plt.show()

def bias_temp_analysis():
    b_1_1 = cam.get_master_bias(-60)
    b_1_2 = cam.get_master_bias(-40)
    b_1_3 = cam.get_master_bias(-20)
    b_2_1 = cam.get_master_bias(-62)
    b_2_2 = cam.get_master_bias(-42)
    b_2_3 = cam.get_master_bias(-22)

    temps = [-60,-40,-20]
    medians_1 = [np.median(b_1_1),np.median(b_1_2),np.median(b_1_3)]
    medians_2 = [np.median(b_2_1),np.median(b_2_2),np.median(b_2_3)]
    medians_1 = 3.22*np.array(medians_1)
    medians_2 = 3.22*np.array(medians_2)
    shutter_temps = [-10,25,-10,25,-10,25]
    temps = [-60,-40,-20]
    # vals = [np.median(b_1_1),np.median(b_2_1),np.median(b_1_2),np.median(b_2_2),np.median(b_1_3),np.median(b_2_3)]
    # dat = list(zip(vals,temps,shutter_temps))
    # biases = pd.DataFrame(dat, columns = ['Median Pixel Value' , 'FPA Temperature', 'Shutter Temperature'])

    #sns.set()
    # sns.set(style="darkgrid")
    # ax = sns.pointplot(x="FPA Temperature", y="Median Pixel Value", hue="Shutter Temperature",
    #                data=biases,
    #                markers=["o", "x"],
    #                linestyles=["-", "--"])
    plt.plot(temps,medians_1,marker='o',linestyle='--',c='g',label='Shutter: -10.6$^\circ$C')
    plt.plot(temps,medians_2,marker='x',c='r',label='Shutter: 25$^\circ$C')
    plt.ylabel('Median ($e^-$)')
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


    print(y[0])
    print(len(y),len(x))
    print(x.shape,y.shape)

    slope, intercept, r_value, _ ,_ = linregress(x,y)
    print(slope,intercept,r_value)
    gain = round(slope,3)
    fit = slope*x + intercept
    slope = round(slope,2)  
    intercept = int(intercept)
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

def dark_current():
    os.chdir('//merger.anu.edu.au/mbirch/images/images24-06-2020/dark_current_-40_20')
    files = glob.glob('*.fits*')
    bias = cam.get_master_bias(-40)
    vals,temps,times = [] , [], []
    for i in files:
        hdu = fits.open(i)
        temps.append(hdu[0].header['TEMPAMB'])
        times.append(hdu[0].header['DITSER'])
        data = hdu[0].data
        data = data - bias
        data[data > 60000] = 0 #Avoid unsigned integer overflow
        clipped,_,_ = sigmaclip(data,3,3)
        vals.append(np.median(clipped))
    
    test_temp = round(np.mean(temps),1)
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
    plt.title('Dark Current, {0}$e^-$/s (FPA: $-40^\circ$, Shutter: {1}$^\circ$C)'\
        .format(i_dark,test_temp))
    plt.show()

def dark_temp_analysis():
    temp_aitc = [-60,-40,-20]
    dark_aitc = [51,419,2199]
    temp_sull = [-50,-40,-30,-20,-10,0]
    dark_sull = [163,500,1000,3000,10000,30000]

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

    plt.plot(temps, (exp(temps, *popt_1)),'r--',label="$A: {0}\pm{1},\:k: {2}\pm0.03$"\
        .format(a_aitc,a_aitc_err,k_aitc))
    plt.plot(temps, (exp(temps, *popt_2)),'g--',label="$A: {0}\pm{1},\:k: {2}\pm0.01$"\
        .format(a_sull,a_sull_err,k_sull))
    
    plt.scatter(temp_aitc,dark_aitc,c= 'red',marker='s',label = 'PIRT 1280SCICAM')
    plt.scatter(temp_sull,dark_sull,c = 'green',marker = "D",label='FLIR AP1121 (Sullivan et. al. 2014)')
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



#bias_temp_var('//merger.anu.edu.au/mbirch/images/nstf_images/bias_-60_-10.6_1')
#bias_bad_pix_var('//merger.anu.edu.au/mbirch/images/nstf_images/bias_-60_-10.6_1')
#dark_bad_pix_var('//merger.anu.edu.au/mbirch/images/nstf_images/master_dark_frames_-60_1',40)