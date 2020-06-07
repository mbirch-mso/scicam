import numpy as np
import matplotlib.pyplot as plt
from scipy import stats
import os
import scicam as cam
from astropy.io import fits

#Input Parameters
testing_dir = '//merger.anu.edu.au/mbirch/data/'
img_dir = '//merger.anu.edu.au/mbirch/images'
unsorted_img = img_dir +'/image_unsorted.fits'
routine = 'capture'
naxis1 = 1040
naxis2 = 1296

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
    
def saturation_ramp(loops,points,tag):
    int_times = np.round(np.linspace(1001,25000,points),0)
    means, medians, buf_times, variances, modes, maxs = map(np.zeros,[points]*6)
    cam.printProgressBar(0, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
    y=0
    for j in range(points):
        cam.set_int_time(int_times[j])
        cam.set_frame_time(int_times[j]+20)
        empty_array = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
        buf_time_per = np.zeros(shape = loops) #Array for averaging buffer time
        for l in range(loops):
            cap, _ = cam.img_cap(routine,img_dir,'f')
            hdu_img = fits.open(unsorted_img)
            fits_img = hdu_img[0]
            buf_time_per[l] = fits_img.header['BUFTIM'] #Grab time to fill buffer
            empty_array += fits_img.data
            hdu_img.close() #Close image so it can be sorted
            os.remove(unsorted_img) #Delete image after data retrieval 
            y = y + 1
            cam.printProgressBar(y,loops*points, prefix = 'Progress:', \
                suffix = 'Complete', length = 50)
        empty_array = empty_array / loops
        buf_times[j] = np.mean(buf_time_per)
        means[j] = np.mean(empty_array)
        medians[j] = np.median(empty_array)
        variances[j] = np.var(empty_array)
        mode_entry, _ = stats.mode(empty_array,axis=None)
        modes[j] = mode_entry[0]
        maxs[j] = np.max(empty_array)
    #Code to save all data to numpy array in txt
    results_path = testing_dir +'saturation_testing/' + \
                    tag + '_results.txt'
    np.savetxt(results_path,(means,medians,buf_times,variances,modes,maxs,int_times))
    print('PROGRAM HAS COMPLETED')

def read_ramp(points,loops,tag):
    int_times = np.round(np.linspace(0.033,25,points),4)
    #int_times[169] = 0.846
    means, medians, variances, stddevs = map(np.zeros,[points]*4)
    cam.printProgressBar(0, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
    y=0
    for j in range(points):
        int_t = cam.set_int_time(int_times[j])
        if int_t >= (int_times[j]+1): #Ignore this data point
            means[j] = means[j-1]
            medians[j] = medians[j-1]
            variances[j] = variances[j-1]
            stddevs[j] = stddevs[j-1]
        else:
            frame_t = cam.set_frame_time(int_times[j]+20)
            empty_array = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
            for l in range(loops):
                cap, _ = cam.img_cap(routine,img_dir,'f')
                hdu_img = fits.open(unsorted_img)
                fits_img = hdu_img[0]
                empty_array = empty_array + fits_img.data
                hdu_img.close() #Close image so it can be sorted
                os.remove(unsorted_img) #Delete image after data retrieval 
                y = y + 1
                cam.printProgressBar(y,loops*points, prefix = 'Progress:', \
                    suffix = 'Complete', length = 50)
            means[j] = np.mean(empty_array)
            medians[j] = np.median(empty_array)
            variances[j] = np.var(empty_array)
            stddevs[j] = np.std(empty_array)
    #Code to save all data to numpy array in txt
    results_path = testing_dir +'read_testing/' + \
                    tag + '_results.txt'
    np.savetxt(results_path,(means,medians,variances,stddevs,int_times))
    print('PROGRAM HAS COMPLETED')

def read_noise_estimate(frames,n,tag):

    '''
    Capture n pairs of bias frames (520REFCLKS)
    Produce difference image from each pair and store
    read noise estimate from sigma/sqrt(2) of difference
    Output histogram of final pair with RN estimate as average
    of all pairs
    '''
    int_t = cam.set_int_time(0.033)
    frame_t = cam.set_frame_time(20.033)
    cam.printProgressBar(0, 2*n, prefix = 'Progress:', suffix = 'Complete', length = 50)
    y = 0
    RN = 0

    for j in range(n):
        bias_1,_ = cam.simple_cap()
        y += 1
        cam.printProgressBar(y,frames, prefix = 'Progress:', \
            suffix = 'Complete', length = 50)
        
        bias_2,_ = cam.simple_cap()
        y += 1
        cam.printProgressBar(y,frames, prefix = 'Progress:', \
            suffix = 'Complete', length = 50)

        bias_dif = bias_2 - bias_1
        dif_clipped,_,_ = sigmaclip(bias_dif,5,5)
        RN += np.std(dif_clipped)/np.sqrt(2)


    RN = RN / 500

    n, bins, _ = plt.hist(dif_clipped,bins = 200,density = 1,facecolor='blue', alpha=0.75,\
                label = 'Bias Difference Frame, $\sigma$:{}'.format(round(np.std(dif_clipped))))
    
    def fit_function(x, B, sigma):
        return (B * np.exp(-1.0 * (x**2) / (2 * sigma**2)))
    popt, _ = curve_fit(fit_function, xdata=bins[0:-1], \
                ydata=n, p0=[0.1, dev])
    xspace = np.linspace(bins[0], bins[-1], 100000)
    fit_dev = round(popt[1],3)
    plt.plot(xspace+0.5, fit_function(xspace, *popt), color='darkorange', \
        linewidth=2.5, label='Gaussian fit, $\sigma$:{}'.format(fit_dev))
    
    plt.ylabel('Density ($\%$)')
    plt.xlabel('ADUs')
    plt.title('Read Noise Estimate:{}'.format(RN))
    plt.legend(loc='best')
    plt.show()
    print('PROGRAM HAS COMPLETED')

def gain_estimate():
    bias = cam.get_master_bias(-60)
    int_times = np.round(np.linspace(1001,25000,points),0)
    intensities,  variances = [],[]

    for j in int_times:
        cam.set_int_time(j)
        cam.set_frame_time(j+20)

        #Take pair of images
        first,_ = cam.simple_capture()
        second,_ = cam.simple_capture()
        
        diff_img = (first-bias) - (second-bias)
        variances.append(np.var(diff_img))
        intensities.append(np.mean(first_bias))
    
    variances = np.array(variances)/2 #Adjust for difference theorem
    intensities = np.array(intensities)

    #Linear fit
    slope, intercept, r_value, _ ,_ = stats.linregress(intensities,variances)

    #Figure out parameters
    gain = round((1 / slope),2)
    read_noise = int(np.sqrt((gain**2)-intercept))

    print(slope,intercept,r_value)
    fit = slope*intensities + intercept
    slope = round(slope,2)  
    intercept = int(intercept)
    rsqr = round((r_value**2),4)
    plt.scatter(intensities,variances,c='red',label = 'Data')
    plt.plot(intensities,fit,'g--',label = 'Linear Fit: (m = {0}, b = {1}, $r^2$ = {2})'\
        .format(slope,intercept,rsqr))
    plt.ylabel('$\sigma^2$ (ADUs)')
    plt.xlabel('Intensity (ADUs)')
    plt.grid(True)
    plt.legend(loc='best')
    plt.title('Pairwise Variance vs Intensity (g = {0}, RN = {1}$e^-$)'\
        .format(gain,read_noise))
    plt.show()
    print('PROGRAM HAS COMPLETED')