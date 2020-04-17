
import numpy as np
import matplotlib.pyplot as plt
import math as m
from fabric import Connection
import subprocess
import os
import scicam as cam
from astropy.io import fits
import datetime

#Input Parameters
saturate_time = 6000
n_loops = 10
n_points = 100
test_tag = 'frame_time_study_2'

rout = 'test_take_fits'
img_loc = '//merger.anu.edu.au/mbirch/images'
unsorted_img = img_loc +'/image_unsorted.fits'
frame_times = np.linspace(5500,6500,n_points)
frame_times = np.round(frame_times,0) #Round so no errors

#Specific fix for recurring number error
#only for current array
#frame_times[19] =  11541 

int_t = cam.set_int_time(saturate_time)

#Initiate arrays for tracked parameters
means = np.zeros(shape=len(frame_times))
medians = np.zeros(shape=len(frame_times))
invalids = np.zeros(shape=len(frame_times))
buf_times = np.zeros(shape=len(frame_times))

#Start by iterating over frame time
for k in range(len(frame_times)):
    frame_t = cam.set_frame_time(frame_times[k])
    empty_array = np.zeros(shape = (1024,1280)) #Initiate array for coadding
    buf_time_per = np.zeros(shape = n_loops) #Array for averaging buffer time
    inval_count = 0
    for j in range(0,n_loops): #Take 50 images per data point
        cap, _ = cam.img_cap(rout,img_loc,'f')
        hdu_img = fits.open(unsorted_img)
        fits_img = hdu_img[0]
        #Code to check if frame is valid
        while fits_img.header['FVSNUM'] != 1:
            hdu_img.close()
            os.remove(unsorted_img)
            image, _ = cam.img_cap(rout,img_loc,'f')
            hdu_img = fits.open(unsorted_img)
            fits_img = hdu_img[0]
            inval_count = inval_count + 1
        #Coadd to total array
        buf_time_per[j] = fits_img.header['BUFTIM'] #Grab time to fill buffer
        empty_array = empty_array + fits_img.data
        hdu_img.close() #Close image so it can be sorted
        cam.file_sorting(img_loc,int_t,frame_t,'.fits',routine=rout,tag=test_tag) #File image away
    
    #Write to arrays
    buf_times[k] = np.mean(buf_time_per)
    means[k] = np.mean(empty_array)
    medians[k] = np.median(empty_array)
    invalids[k] = inval_count


#Code to save all data to numpy array in txt
now = datetime.datetime.now()
today = now.strftime("%d-%m-%Y")
folder_path = img_loc + '/' + 'images' + today+ '/' + test_tag + '/'
np.savetxt(folder_path  + 'means.txt',means)
np.savetxt(folder_path  + 'medians.txt',medians)
np.savetxt(folder_path  + 'invalids.txt', invalids)
np.savetxt(folder_path  + 'buf_times.txt', buf_times)
np.savetxt(folder_path  + 'frames.txt', frame_times)
print('PROGRAM HAS COMPLETED')







