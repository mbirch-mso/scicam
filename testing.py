import numpy as np
import math as m
import matplotlib.pyplot as plt
from scipy import stats,optimize
import os
import scicam as cam
from astropy.io import fits

#Input Parameters
testing_dir = '//merger.anu.edu.au/mbirch/data/'
img_dir = '//merger.anu.edu.au/mbirch/images'
local_img_dir = 'C:/images'
unsorted_img = img_dir +'/image_unsorted.fits'
local_unsorted = local_img_dir +'/image_unsorted.fits'
routine = 'capture'
naxis1 = 1040
naxis2 = 1296


    
def saturation_ramp(loops,n,tag):
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

def read_ramp(n):
    int_times = np.round(np.linspace(0.033,0.5,n),3)
    RNs = []
    bias_level = []
    cam.printProgressBar(0, n)
    y=0
    for j in int_times:
        int_t = cam.set_int_time(j)
        if int_t < (j+1):
            cam.set_frame_time(20.33)
            
            bias_1,_ = cam.simple_cap()
            bias_2,_ = cam.simple_cap()
            
            bias_1 = np.asarray(bias_1, dtype=np.int32)
            bias_2 = np.asarray(bias_2, dtype=np.int32)
            bias_dif = bias_2 - bias_1
            
            dif_clipped = bias_dif.flatten()
            RNs.append(np.std(dif_clipped)/np.sqrt(2))

            bias_level.append(np.median(bias_1))
        else:
            RNs.append(RNs[-1])
            bias_level.append(bias_level[-1])

        y += 1
        cam.printProgressBar(y,n)
    RNs = 3.22*np.array(RNs)
    bias_level = 3.22*np.array(bias_level)

    int_times *= 1E3

    fig, ax1 = plt.subplots()

    color = 'tab:red'
    ax1.set_xlabel('Integration Time ($\mu$s)')
    ax1.set_ylabel('Median $e^-$/pixel', color=color)
    ax1.scatter(int_times, bias_level, color=color)
    ax1.tick_params(axis='y', labelcolor=color)

    ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

    color = 'tab:blue'
    ax2.set_ylabel('$\sigma$', color=color)  # we already handled the x-label with ax1
    ax2.scatter(int_times, RNs, color=color)
    ax2.tick_params(axis='y', labelcolor=color)

    fig.tight_layout()  # otherwise the right y-label is slightly clipped
    plt.grid(True)
    plt.title('Read-noise/Bias as function of Integration Time')
    plt.show()

def read_noise_estimate(n):

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
    RNs = []

    for j in range(n):
        bias_1,_ = cam.simple_cap()
        y += 1
        cam.printProgressBar(y,2*n)
        bias_2,_ = cam.simple_cap()
        y += 1
        cam.printProgressBar(y,2*n)

        bias_1 = np.asarray(bias_1, dtype=np.int32)
        bias_2 = np.asarray(bias_2, dtype=np.int32)
        #save max mean dif, max absolute dif
        bias_dif = bias_2 - bias_1

        dif_clipped = bias_dif.flatten()
        RNs.append(np.std(dif_clipped)/np.sqrt(2))
    dev = np.std(dif_clipped)
    RNs = np.array(RNs)
    RN = round(np.median(RNs),3)
    uncert = round(3*np.std(RNs),2)
    sample_hist,_,_ = stats.sigmaclip(dif_clipped,5,5)
    N, bins, _ = plt.hist(sample_hist,bins = 265,facecolor='blue', alpha=0.75,\
                label = 'Bias Difference Image')

    def fit_function(x, B, sigma):
        return (B * np.exp(-1.0 * (x**2) / (2 * sigma**2)))
    popt, _ = optimize.curve_fit(fit_function, xdata=bins[0:-1]+0.5, \
                ydata=N, p0=[0, dev])
    xspace = np.linspace(bins[0], bins[-1], 100000)
    fit_dev = round(popt[1],3)
    delta_sig = round(abs(fit_dev - dev),2)
    plt.plot(xspace+0.5, fit_function(xspace, *popt), color='darkorange', \
        linewidth=2.5, label='Gaussian fit, $\Delta\sigma$:{}'.format(delta_sig))
    
    plt.ylabel('No. of Pixels')
    plt.xlabel('ADUs')
    plt.title('Read Noise Estimate:${}\pm{}$ ADUs ($n={}$, FPA:$-40^\circ$C)'.format(RN,uncert,n))
    plt.legend(loc='best')
    plt.show()
    print('PROGRAM HAS COMPLETED')

def gain_estimate(n):

    bias = cam.get_master_bias(-40)
    int_times = np.round(np.linspace(500,5000,n),0)
    medians,  sigmasqrs = [],[]

    for j in int_times:
        cam.set_int_time(j)
        cam.set_frame_time(j+20)

        #Take pair of images
        first,_ = cam.simple_cap()
        second,_ = cam.simple_cap()
        
        first = np.asarray(first, dtype=np.int32)
        second = np.asarray(second, dtype=np.int32)

        first = first - bias
        second = second - bias

        diff_img = first - second #Difference of bias-subtracted frames
        clipped_diff_img = cam.roi_clip(diff_img)
        clipped_img = cam.roi_clip(first)
        
        sigmasqrs.append(np.var(clipped_diff_img)) #variance from difference
        medians.append(np.median(clipped_img)) #intensity from single frame
    
    sigmasqrs = np.array(sigmasqrs)/2 #Adjust for difference theorem
    medians = np.array(medians)

    #Linear fit
    slope, intercept, r_value, _ ,_ = stats.linregress(medians,sigmasqrs)
    print(slope,intercept,r_value)
    #Figure out parameters
    gain = round((1/slope),2)
    #read_noise = int(np.sqrt((gain**2)*intercept))

    results_path = testing_dir +'gain_testing/' + \
                    'study_2_results.txt'
    np.savetxt(results_path,(medians,sigmasqrs,int_times))
    fit = slope*medians + intercept
    slope = round(slope,2)  
    intercept = int(intercept)
    rsqr = round((r_value**2),4)
    plt.scatter(medians,sigmasqrs,c='red',label = 'Data')
    plt.plot(medians,fit,'g--',label = 'Linear Fit: (m = {0}, b = {1}, $r^2$ = {2})'\
        .format(slope,intercept,rsqr))
    plt.ylabel('$\sigma^2$ (ADUs)')
    plt.xlabel('Intensity (ADUs)')
    plt.grid(True)
    plt.legend(loc='best')
    plt.title('Pairwise Variance vs Intensity Gain Study, g = {0}'.format(gain))
    plt.show()
    print('PROGRAM HAS COMPLETED')

def bias_temp(n,loops):
    bias = cam.get_master_bias(-40) #Reference
    median_residuals = []
    times = []
    #Stamp starting time here

    int_t = cam.set_int_time(0.033)
    frame_t = cam.set_frame_time(20.033)
    for i in range(loops):
        img = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
        for i in range(n):
            frame,_ = cam.simple_cap()
            img += frame
        img = img / n
        #Stamp time of this frame here
        #Append time to list of times

        residual_img = img - bias 
        median_residuals.append(np.median(residual_img)) #Append median of difference
    
    fig, ax1 = plt.subplots()
    colour = 'tab:blue'
    ax1 = sns.pointplot(x=temps,y=means,color=colour)
    ax1.set_ylabel('Median Residual', color=colour)
    ax1.tick_params(axis='y', labelcolor=colour)
    ax1.set_xlabel('Time (s)')
    
    plt.title(r'$\mathrm{Thermal\ emission\ of\ shutter\ study\(bias\ frames)}$')
    plt.show()
    
def dark_current(n,T,tag='',amb_temp=''):
    int_times = np.round(np.linspace(1001,40000,n),0) 
    
    cam.printProgressBar(0, sum(int_times))
    y=0
    
    for j in int_times:
        
        cam.set_int_time(j)
        cam.set_frame_time(j+20)
        
        cap, _ = cam.img_cap(routine,img_dir,'f')
        hdu_img = fits.open(unsorted_img)
        data = hdu_img[0].data
        
        dark_header = fits.getheader(unsorted_img)
        dark_header.append(('FPATEMP',T,'Temperature of detector'))
        dark_header.append(('TEMPAMB',amb_temp,'Ambient Temperature'))
        hdu_img.close()
        
        os.remove(unsorted_img) #Delete image after data retrieval 
        fits.writeto(unsorted_img,data,dark_header)
        cam.file_sorting(img_dir,j,j+20,tag=tag)

        y += j
        cam.printProgressBar(y,sum(int_times))
    print('PROGRAM HAS COMPLETED')

dark_current(60,-40,'dark_current_-40_20',20)