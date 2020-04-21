
import numpy as np
import matplotlib.pyplot as plt
import scipy.ndimage
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
from mpl_toolkits.mplot3d import Axes3D



testing_dir = '//merger.anu.edu.au/mbirch/data/camera_control/'

folder_path = lambda test: testing_dir + test + "/" 
frame_path = folder_path('frame_time_studies')
int_path = folder_path('int_time_studies')
frame_int_space_path = folder_path('frame_int_space_testing')
saturation_path = folder_path('saturation_testing')

#Frame_Study_1
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
    
    #fig = plt.figure(figsize=plt.figaspect(0.5))
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
    
#display_3dparam()

#display_2dparam('means','saturation','saturation_test_1')


structl = np.loadtxt(saturation_path + 'saturation_test_source_2' + '_results.txt')
struct = np.loadtxt(saturation_path + 'saturation_test_2' + '_results.txt')
means = struct[0,:]
meansl = structl[0,:]

maxs = struct[5,:]
x = struct[6,:]
xl = structl[6,:]

plt.figure(1)


plt.subplot(121)
plt.plot(x,means,label = 'Mean')
plt.axhline(14400,c='r',ls = '--',label ='Effective Saturation Value: {}'.format(14400))
plt.axhline(maxs[0],c='b',ls = '--',label ='Bad pixel value: {}'.format(16385))
plt.xlabel('Integration time (ms)')
plt.ylabel('ADUs')
plt.title('Dark')
plt.legend(loc = 'best')


plt.subplot(122)
plt.plot(xl,meansl,label = 'Mean')
plt.axhline(15634,c='r',ls = '--',label ='Effective Saturation Value: {}'.format(15635))
plt.axhline(maxs[0],c='b',ls = '--',label ='Bad pixel value: {}'.format(16385))
plt.xlabel('Integration time (ms)')
plt.ylabel('ADUs')
plt.title('Illuminated')
plt.legend(loc = 'best')
plt.suptitle("Detector Saturation vs Integration Time", fontsize=14)
plt.show()