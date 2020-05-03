
import numpy as np
import matplotlib.pyplot as plt
import scipy.ndimage
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
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
    x = x#*1000
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
    maxs = struct[5,:][0]
    x = struct[6,:]
    #plt.subplot(122)
    plt.plot(x,means,label = 'Mean')
    plt.axhline(15634,c='r',ls = '--',label ='Effective Saturation Value: {}'.format(15635))
    #plt.axhline(maxs[0],c='b',ls = '--',label ='Bad pixel value: {}'.format(16385))
    plt.xlabel('Integration time (ms)')
    plt.ylabel('ADUs')
    plt.title('Illuminated')
    plt.legend(loc = 'best')
    plt.suptitle("Detector Saturation vs Integration Time", fontsize=14)
    plt.show()
    plt.axhline(14400,c='r',ls = '--',label ='Effective Saturation Value: {}'.format(14400))
    # plt.axhline(maxs[0],c='b',ls = '--',label ='Bad pixel value: {}'.format(16385))
    # plt.xlabel('Integration time (ms)')
    # plt.ylabel('ADUs')
    # ax1.title('ADUs vs Integration Time')
    # fig.legend(loc = 'best')


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
    axs[1].set_title('Master Read Frame, DIT=$33\mu s$ (520REFCLKS), NDIT=1000')
    plt.show()

analyse_read()

#saturation_plot()
#read_ramp()
