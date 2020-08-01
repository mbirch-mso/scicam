
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import datetime

from scipy import integrate,optimize
from astropy.constants import c,h,k_B

def thermal_emission_temp():
    
    temps = np.linspace(-50,50,10)
    w = np.linspace(1.2E-6,1.67E-6,10000)

    fluxes = []
    for i in temps:
        a = 2.0*h.value*(c.value**2)
        b = (h.value*c.value)/(w*k_B.value*(i+273.15))
        flux = (a/(w**5)) * (1/(np.exp(b) - 1))
        total = np.trapz(flux,w)
        fluxes.append(total)
    
    dark_floor = np.mean(fluxes)
    fluxes = fluxes+dark_floor

    def exp(x,a,b,c):
        return c + a*np.exp(b*(x+273.15))
    def exp2(x,a,b):
        return a*np.exp(b*(x+273.15))
    popt, _ = optimize.curve_fit(exp, temps, fluxes,p0=[1E-6,1E-6,1E-6])
    popt2, _ = optimize.curve_fit(exp2, temps, fluxes,p0=[1E-6,1E-6])

    

    plt.plot(temps, (exp(temps, *popt)),'r',label="Fitted Curve")
    plt.plot(temps, (exp2(temps, *popt2)),'r',label="Fitted Curve (no floor)")
    plt.scatter(temps,np.array(fluxes),label='Measured Emission ($I_d$:{}'.format(5*np.mean(fluxes)))
    plt.ylabel('W (arbitrary)')
    plt.xlabel('Temperature ($^\circ$C)')
    plt.title('NIR (1.2$\mu$m-1.67$\mu$m) Blackbody Emission ($I_d$ estimate: {}'.format(popt[2]))
    plt.grid(True)
    plt.legend(loc='Best')
    plt.show()

def frame_times(i,loops,points,min_t,max_t,tag):
    cam.set_int_time(i)
    frame_times = np.linspace(min_t,max_t,points)
    frame_times = np.round(frame_times,2) #Round so no errors
    means, medians, buf_times, variances = map(np.zeros,[points]*4)
    cam.printProgressBar(0, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
    y=0
    for k in range(len(frame_times)):
        cam.set_frame_time(frame_times[k])
        empty_array = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
        buf_time_per = np.zeros(shape = loops) #Array for averaging buffer time
        for j in range(0,loops): #Take 50 images per data point
            cap, _ = cam.img_cap(routine,img_dir,'f')
            hdu_img = fits.open(unsorted_img)
            fits_img = hdu_img[0]
            buf_time_per[j] = fits_img.header['BUFTIM'] #Grab time to fill buffer
            empty_array = empty_array + fits_img.data
            hdu_img.close() #Close image so it can be sorted
            os.remove(unsorted_img) #Delete image after data retrieval
            y = y + 1
            cam.printProgressBar(y, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
        #Write to arrays
        buf_times[k] = np.mean(buf_time_per)
        means[k] = np.mean(empty_array)
        medians[k] = np.median(empty_array)
        variances[k] = np.var(empty_array)

    #Code to save all data to numpy array in txt
    results_path = testing_dir +'camera_control/frame_time_studies/' + \
                   tag + '_results.txt'
    np.savetxt(results_path,(means,medians,buf_times,variances,frame_times),header=str(i))
    print('PROGRAM HAS COMPLETED')

def int_times(t,loops,points,min_i,max_i,tag):
    cam.set_frame_time(t)
    int_times = np.linspace(min_i,max_i,points)
    int_times = np.round(int_times,2) #Round so no errors
    means, medians, buf_times, variances = map(np.zeros,[points]*4)
    cam.printProgressBar(0, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
    y=0
    #Iterate over frame time
    for k in range(len(int_times)):
        cam.set_int_time(int_times[k])
        empty_array = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
        buf_time_per = np.zeros(shape = loops) #Array for averaging buffer time
        for j in range(0,loops): #Take 50 images per data point
            cap, _ = cam.img_cap(routine,img_dir,'f')
            hdu_img = fits.open(unsorted_img)
            fits_img = hdu_img[0]
            buf_time_per[j] = fits_img.header['BUFTIM'] #Grab time to fill buffer
            empty_array = empty_array + fits_img.data
            hdu_img.close() #Close image so it can be sorted
            os.remove(unsorted_img) #Delete image after data retrieval
            y = y + 1
            cam.printProgressBar(y, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
        #Write to arrays
        buf_times[k] = np.mean(buf_time_per)
        means[k] = np.mean(empty_array)
        medians[k] = np.median(empty_array)
        variances[k] = np.var(empty_array)

    #Code to save all data to numpy array in txt
    results_path = testing_dir +'camera_control/int_time_studies/' + \
                   tag + '_results.txt'
    np.savetxt(results_path,(means,medians,buf_times,variances,int_times),header=str(t))
    print('PROGRAM HAS COMPLETED')

def frame_int_times(loops,points,min_val,max_val,tag):
    int_times = np.round(np.linspace(min_val,max_val,points),2)
    int_times[31] = 6327.62
    frame_times = np.round(np.linspace(min_val,max_val,points),2)
    frame_times[31] = 6327.62
    means = np.zeros(shape = (points,points))
    buf_times = np.zeros(shape = (points,points))
    variances = np.zeros(shape = (points,points))
    medians = np.zeros(shape = (points,points))
    cam.printProgressBar(0,(loops*(points**2)), prefix = 'Progress:', suffix = 'Complete', length = 50)
    y=0
    for j in range(points):
        cam.set_int_time(int_times[j])
        for k in range(points):
            cam.set_frame_time(frame_times[k])
            empty_array = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
            buf_time_per = np.zeros(shape = loops) #Array for averaging buffer time
            for l in range(loops):
                cap, _ = cam.img_cap(routine,img_dir,'f')
                hdu_img = fits.open(unsorted_img)
                fits_img = hdu_img[0]
                buf_time_per[l] = fits_img.header['BUFTIM'] #Grab time to fill buffer
                empty_array = empty_array + fits_img.data
                hdu_img.close() #Close image so it can be sorted
                os.remove(unsorted_img) #Delete image after data retrieval 
                y = y + 1
                cam.printProgressBar(y,(loops*(points**2)), prefix = 'Progress:', \
                    suffix = 'Complete', length = 50)
            #Write to arrays
            buf_times[j,k] = np.mean(buf_time_per)
            means[j,k] = np.mean(empty_array)
            medians[j,k] = np.median(empty_array)
            variances[j,k] = np.var(empty_array)
    
    #Code to save all data to numpy array in txt
    results_path = testing_dir +'camera_control/frame_int_space_testing/' + \
                   tag
    np.savetxt(results_path + '_means.txt',means)
    np.savetxt(results_path + '_medians.txt',medians)
    np.savetxt(results_path + '_bufs.txt',buf_times)
    np.savetxt(results_path + '_variances.txt',variances)
    np.savetxt(results_path + '_ints.txt',int_times)
    np.savetxt(results_path + '_frames.txt',frame_times)
    print('PROGRAM HAS COMPLETED')

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
