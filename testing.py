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

# Print progress
def printProgressBar (iteration, total, prefix = '', suffix = '', decimals = 1, length = 100, fill = '█', printEnd = "\r"):
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    print('\r%s |%s| %s%% %s' % (prefix, bar, percent, suffix), end = printEnd)
    if iteration == total: 
        print()


def frame_times(i,loops,points,min_t,max_t,tag):
    cam.set_int_time(i)
    frame_times = np.linspace(min_t,max_t,points)
    frame_times = np.round(frame_times,2) #Round so no errors
    means, medians, buf_times, variances = map(np.zeros,[points]*4)
    printProgressBar(0, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
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
            printProgressBar(y, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
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
    printProgressBar(0, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
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
            printProgressBar(y, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
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


def frame_int_space(loops,points,min_val,max_val,tag):
    int_times = np.round(np.linspace(min_val,max_val,points),2)
    int_times[31] = 6327.62
    frame_times = np.round(np.linspace(min_val,max_val,points),2)
    frame_times[31] = 6327.62
    means = np.zeros(shape = (points,points))
    buf_times = np.zeros(shape = (points,points))
    variances = np.zeros(shape = (points,points))
    medians = np.zeros(shape = (points,points))
    printProgressBar(0,(loops*(points**2)), prefix = 'Progress:', suffix = 'Complete', length = 50)
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
                printProgressBar(y,(loops*(points**2)), prefix = 'Progress:', \
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
    

def saturation_testing(loops,points,tag):
    int_times = np.round(np.linspace(1001,25000,points),0)
    means, medians, buf_times, variances, modes, maxs = map(np.zeros,[points]*6)
    printProgressBar(0, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
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
            printProgressBar(y,loops*points, prefix = 'Progress:', \
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


def read_ramp_testing(points,loops,tag):
    int_times = np.round(np.linspace(0.033,25,points),4)
    #int_times[169] = 0.846
    means, medians, variances, stddevs = map(np.zeros,[points]*4)
    printProgressBar(0, loops*points, prefix = 'Progress:', suffix = 'Complete', length = 50)
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
                printProgressBar(y,loops*points, prefix = 'Progress:', \
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


def create_master_read(frames,tag):
    int_t = cam.set_int_time(0.033)
    frame_t = cam.set_frame_time(20.033)
    printProgressBar(0, frames, prefix = 'Progress:', suffix = 'Complete', length = 50)
    y=0
    master_read = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
    for j in range(frames):
        cap, _ = cam.img_cap(routine,img_dir,'f')
        hdu_img = fits.open(unsorted_img)
        fits_img = hdu_img[0]
        master_read += fits_img.data
        hdu_img.close() #Close image so it can be sorted

        y = y + 1
        printProgressBar(y,frames, prefix = 'Progress:', \
            suffix = 'Complete', length = 50)
        if j == frames-1: #On final frame grab header
            frame_header = fits.getheader(unsorted_img)
            frame_header.append(('NDIT',frames,'Number of integrations'))
            sky_header.append(('TYPE','MASTER_READ','Read noise esteimate from min exposure'))
        os.remove(unsorted_img) #Delete image after data retrieval 
    
    master_read = master_read / frames #Divide by NDIT
    master_path = testing_dir + 'read_testing/' + 'master_read_' \
                  + tag + '.fits'
    fits.writeto(master_path,master_read,frame_header)
    results_path = testing_dir +'read_testing/' + \
                    tag + '_masterdata.txt'
    np.savetxt(results_path,master_read)
    print('PROGRAM HAS COMPLETED')


def expose(i,n,tag=''):
    int_t = cam.set_int_time(i)
    frame_t = cam.set_frame_time(i+25)
    printProgressBar(0, n, prefix = 'Progress:', suffix = 'Complete', length = 50)
    y=0
    array = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
    for j in range(n):
        cap, _ = cam.img_cap(routine,img_dir,'f')
        hdu_img = fits.open(unsorted_img)
        fits_img = hdu_img[0]
        array += fits_img.data
        hdu_img.close() #Close image so it can be sorted
        y = y + 1
        printProgressBar(y,n, prefix = 'Progress:', \
            suffix = 'Complete', length = 50)
        if j == n-1: #On final frame grab header
            int_header = fits.getheader(unsorted_img)
            int_header.append(('NDIT',n,'Number of integrations'))
        os.remove(unsorted_img) #Delete image after data retrieval 
    fits.writeto(unsorted_img,array,int_header)
    cam.weather_to_fits(unsorted_img)
    cam.file_sorting(img_dir,int_t,frame_t,tag=tag)
    print('EXPOSE COMPLETE')


#create_master_read(1000,'test1_1000')
#read_ramp_testing(200,200,'200on200_test1_transition')
saturation_testing(20,50,'satuation_20loops_50pnts_1')
#frame_times(1002,3,3,20,2000,'wide_100point_study_8')
#int_times(1000,3,10,20,2000,'wide__int_100point_study_4')
#frame_int_space(1,50,21,10000,'param_space_study_1')