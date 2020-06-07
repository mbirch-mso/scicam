
import numpy as np
import matplotlib.pyplot as plt
import scicam as cam
import scipy.ndimage
from scipy.stats import sigmaclip,norm,linregress
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

def plot2d(param,x,y):
    plt.plot(x,y,label='Data')
    if param == 'means':
        #plt.axvline(held_val,c='r',ls = '--',label ='Exposure')
        plt.title('Mean ADUs/pixel')
        plt.xlabel('Frame Time (ms)')
        plt.ylabel('Mean ADUs/pixel')
    if param == 'medians':
       # plt.axvline(held_val,c='r',ls = '--',label ='Exposure')
        plt.title('Median ADUs/pixel')
        plt.xlabel('Frame Time (ms)')
        plt.ylabel('Median ADUs/pixel')
    if param == 'buf_times':
      #  plt.axvline(held_val,c='r',ls = '--',label ='Exposure')
        plt.title('Mean time to fill buffer')
        plt.xlabel('Frame Time (ms)')
        plt.ylabel('Buffer Time (ms)')
    if param == 'variances':
      #  plt.axvline(held_val,c='r',ls = '--',label ='Exposure')
        plt.title('Variance')
        plt.xlabel('Frame Time (ms)')
        plt.ylabel('Variance (ADUs)')
    
    plt.legend(loc='best')
    plt.show()

def display_2dparam(param,value,tag):
    if value == "frames":
        struct = np.loadtxt(frame_path + tag + '_results.txt')
        x = struct[4,:]
    if value == "ints":
        struct = np.loadtxt(int_path + tag + '_results.txt')
        x = struct[4,:]
    if value == "saturation":
        struct = np.loadtxt(saturation_path + tag + '_results.txt')
        x = struct[4,:]
    if value == "read":
        struct = np.loadtxt(saturation_path + tag + '_results.txt')
        x = struct[4,:]
    
    if param == 'means':
        y = struct[0,:]
        print(struct)
        print(x)
        print(y)
        plot2d('means',x,y)
    if param == 'medians':
        y = struct[1,:]
        plot2d('medians',x,y)
    if param == 'buf_times':
        y = struct[2,:]
        plot2d('buf_times',x,y)
    if param == 'variances':
        y = struct[3,:]
        plot2d('variances',x,y)

def display_3dparam():
    path = frame_int_space_path + '/param_space_study_1/param_space_study_1_'
    

    frames = np.loadtxt(path + 'frames.txt')
    ints = np.loadtxt(path + 'ints.txt')
    means = np.loadtxt(path + 'means.txt')
    medians = np.loadtxt(path + 'medians.txt')
    bufs = np.loadtxt(path + 'bufs.txt')
    variances = np.loadtxt(path + 'variances.txt')
    x, y = np.meshgrid(frames,ints)
    x = x/1000
    y = y/1000
    
    sigma = [0.8, 0.8]
    means_s = scipy.ndimage.filters.gaussian_filter(means, sigma, mode='constant')
    medians_s = scipy.ndimage.filters.gaussian_filter(medians, sigma, mode='constant')
    bufs_s = scipy.ndimage.filters.gaussian_filter(bufs, sigma, mode='constant')
    variances_s = scipy.ndimage.filters.gaussian_filter(variances, sigma, mode='constant')

    fig = plt.figure()

    ax = fig.add_subplot(2, 2, 1, projection='3d')  
    surf = ax.plot_surface(x, y, means_s, cmap=cm.coolwarm, linewidth=0, antialiased=True)
    plt.title('Mean ADUs/pixel')
    plt.xlabel('Frame Times (s)')
    plt.ylabel('Integration Times (s)')

    ax = fig.add_subplot(2, 2, 2, projection='3d')  
    surf = ax.plot_surface(x, y, medians_s, cmap=cm.coolwarm, linewidth=0, antialiased=True)
    plt.title('Median ADUs/pixel')
    plt.xlabel('Frame Times (s)')
    plt.ylabel('Integration Times (s)')

    ax = fig.add_subplot(2, 2, 3, projection='3d')  
    surf = ax.plot_surface(x, y, bufs_s, cmap=cm.coolwarm, linewidth=0, antialiased=True)
    plt.title('Buffer fill time (s)')
    plt.xlabel('Frame Times (s)')
    plt.ylabel('Integration Times (s)')

    ax = fig.add_subplot(2, 2, 4, projection='3d')  
    surf = ax.plot_surface(x, y, variances_s, cmap=cm.coolwarm, linewidth=0, antialiased=True)
    plt.title('Variance (ADUs)')
    plt.xlabel('Frame Times (s)')
    plt.ylabel('Integration Times (s)')
    plt.show()

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
    
    #Create simple mask
    nrows, ncols = (1040,1296)
    row, col = np.ogrid[:nrows, :ncols]
    cnt_row, cnt_col = nrows / 2, ncols / 2
    outer_disk_mask = ((row - cnt_row)**2 + (col - cnt_col)**2 >(nrows / 2.5)**2)

    def format_sky(hdu,i):
        data = (hdu[0].data)/i
        data[outer_disk_mask] = 0
        data_masked = data.flatten()
        data_masked = data_masked[data_masked != 0]
        clipped, _ , _ = sigmaclip(data_masked,4,4) #4 sigma clip of outliers
        return clipped

    clipped2 = format_sky(hdu2,2)
    clipped5 = format_sky(hdu5,5)
    clipped10 = format_sky(hdu10,10)
    clipped20 = format_sky(hdu20,20)
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
    neg_sixty = cam.get_master_bias(-60)
    neg_forty = cam.get_master_bias(-40)
    neg_twenty = cam.get_master_bias(-20)
    zero = cam.get_master_bias(0)
    temps = [-60,-40,-20,0]
    means = [np.mean(neg_sixty),np.mean(neg_forty),np.mean(neg_twenty),np.mean(zero)]
    devs = [np.std(neg_sixty),np.std(neg_forty),np.std(neg_twenty),np.std(zero)]
    
    max_resid = neg_sixty - zero
    img_eq = exposure.equalize_hist(max_resid)
    raveled = max_resid.flatten()
    raveled = raveled[(raveled>650) & (raveled<850)]
    # fig, axs = plt.subplots(1, 2)
    # axs[0].imshow(img_eq)
    # axs[0].set_xticks([])
    # axs[0].set_yticks([])
    # axs[1].hist(raveled,bins=200,label = '$\mu={0}\ \sigma={1}$'\
    #             .format(int(np.mean(raveled)),int(np.std(raveled))))
    # #axs[1].set_xlim(550,608)
    # axs[1].set_xlabel('ADUs')
    # axs[0].set_title('Hist-equalised image')
    # axs[1].set_title('Pixel distribution')
    # plt.suptitle('-60C to 0C Bias Residual')
    # plt.legend(loc='best')
    # plt.show()
    
    fig, ax1 = plt.subplots()
    colour = 'tab:blue'
    ax1 = sns.pointplot(x=temps,y=means,color=colour)
    ax1.set_ylabel('Mean (ADUs)', color=colour)
    ax1.tick_params(axis='y', labelcolor=colour)
    ax1.set_xlabel('Temperature ($^{\circ}$C)')
    
    colour = 'tab:red'
    ax2 = ax1.twinx()
    ax2 = sns.pointplot(x=temps,y=devs,color=colour)
    ax2.set_ylabel('$\sigma$ (ADUs)', color=colour)
    ax2.tick_params(axis='y', labelcolor=colour)
    plt.title(r'$\mathrm{Master\ Bias\ results\ against\ Temperature (33\mu s, 520REFCLKS)}$')
    plt.show()


    # residuals = np.array([[neg_forty-neg_forty,neg_forty-neg_twenty,neg_forty-zero,neg_forty-twenty],\
    #     [neg_twenty-neg_forty,neg_twenty-neg_twenty,neg_twenty-zero,neg_twenty-twenty],\
    #         [zero-neg_forty,zero-neg_twenty,zero-zero,zero-twenty],\
    #             [twenty-neg_forty,twenty-neg_twenty,twenty-zero,twenty-twenty]])
    # residual_matrix = np.zeros(shape=(4,4))
    # for i in range(residuals.shape[0]):
    #     for j in range(residuals.shape[1]):
    #         residual_matrix[i,j] = np.mean(residuals[i,j])
    # residual_matrix.astype(int)
    # sns.set(style="white")
    # fig, axs = plt.subplots(1, 2)#, tight_layout=True)    
    # pos = axs[0].matshow(residual_matrix)#ax1.set_yticklabels(temps)
    # axs[0].set_xticklabels(['']+temps)
    # axs[0].set_yticklabels(['']+temps)
    # axs[0].set_title('Mean Residual Matrix ($^{\circ}$C)')
    # fig.colorbar(pos, ax=axs[0])
    #plt.title(r'$\mathrm{Mean\ of\ Residual\ Matrix}$')

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



def gain_fitting(files):

    pass

    # diff_imgs = []
    # means, variances = [],[]
    # for i in sets:
    #     diff = i[1] - i[0]
    #     intensity, _ , _ = sigmaclip(intensity,5,5)
    #     means.append(np.mean(intensity))
    #     clipped, _ , _ = sigmaclip(diff,5,5)
    #     #means.append(np.mean(clipped))
    #     variances.append(np.var(clipped))
    #     diff_imgs.append(clipped)

    # variances = np.array(variances)/2 #Adjustment for variance of the difference
    
    # slope, intercept, r_value, _ ,_ = stats.linregress(means,variances)

    # #Figure out parameters
    # gain = round((1 / slope),2)
    # read_noise = int(np.sqrt((gain**2)-intercept))

    # print(slope,intercept,r_value)
    # fit = slope * np.array(means) + intercept
    # slope = round(slope,2)  
    # intercept = int(intercept)
    # rsqr = round((r_value**2),4)
    # plt.scatter(means,variances,c='red',label = 'Data')
    # plt.plot(means,fit,'g--',label = 'Linear Fit: (m = {0}, b = {1}, $r^2$ = {2})'\
    #     .format(slope,intercept,rsqr))
    # plt.ylabel('$\sigma^2$ (ADUs)')
    # plt.xlabel('Intensity (ADUs)')
    # plt.grid(True)
    # plt.legend(loc='best')
    # plt.title('Pairwise Variance vs Intensity for Blue Flats (g = {0}, RN = {1}$e^-$)'\
    #     .format(gain,read_noise))
    # plt.show()