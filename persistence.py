
import subprocess
import numpy as np
import glob
from astropy.io import fits
import matplotlib.pyplot as plt
from skimage import exposure
import os
import time
import scicam as cam
import datetime

#Standard params
img_dir = '//merger.anu.edu.au/mbirch/images'
unsorted_img = img_dir +'/image_unsorted.fits'
persist_dir = img_dir +'/persistence_images'
routine = 'capture'

def persist_routine(dit,offset,end_t,tag):
    

    sorting_dir = persist_dir + '/' + tag
    os.mkdir(sorting_dir)
    img_name = sorting_dir + '/img_' + str(dit) + '_'


    #Run sld_on.exe
    subprocess.call(["C:\EDT\pdv\sld_on.exe"])
    print("SOAK BEGIN")

    cam.set_int_time(dit)
    cam.set_frame_time(dit+offset)

    fr = cam.read_frame_time()
    it = cam.read_int_time()
    print(fr,it)
    
    #Take throwaway image to open up cam
    cam.img_cap(routine,img_dir,'f')
    os.remove(unsorted_img)

    #Run soak.exe
    subprocess.call(["C:\EDT\pdv\soak.exe"])
    t0 = time.time() #Start Timer (time since soak)
    t = 0
    print("SOAK END")

    while t < end_t:
        cam.img_cap(routine,img_dir,'f')
        t1 = time.time()
        t = t1-t0
        t_s = round(t,2)
        print("Image taken: {}".format(t_s))
        file_name = img_name + str(t_s) + '_.fits'
        os.rename(unsorted_img, file_name)
 

    print("PROGRAM COMPLETE")


# res = [50,100,200,300,400,500,600]
# for i in res:
#     name = 'magnified_' + str(i) + '_100s_test'
#     persist_routine(i,5,100,name)

def persist_analyse(folder):

    os.chdir(persist_dir + '/'+ folder)
    img_list = glob.glob('*.fits*')
    times, amps = [],[]
    splitty = folder.split('_')
    for i in img_list:
        str_detail = i.split('_')
        t = float(str_detail[-2])
        times.append(round(t,2))

        hdu = fits.open(i)
        data = hdu[0].data
        frame = data.astype(np.int32)
        masked = cam.roi_circle(frame)

        amps.append(np.median(masked))

    amps_normed = amps/np.max(amps)
    plt.scatter(times,amps_normed,label='Frame Time = {}ms'.format(splitty[1]))


def persist_map(folder):
    os.chdir(persist_dir + '/'+ folder)
    img_list = glob.glob('*.fits*')
    img_list.sort()

    print(img_list[-1])
    high = img_list[-1]
    hdu_h = fits.open(high)
    frame_h = hdu_h[0].data
    frame_h = frame_h.astype(np.int32)

    print(img_list[0])
    low = img_list[0]
    hdu_l = fits.open(low)
    frame_l = hdu_l[0].data
    frame_l = frame_l.astype(np.int32)

    p_map = frame_h - frame_l
    p_map_eq = exposure.equalize_hist(p_map)
    plt.imshow(p_map_eq)
    plt.axis('off')
    plt.title('Persistence Map (min-max charge down difference)')
    plt.colorbar()
    plt.show()


persist_analyse('magnified_50_100s_test')
persist_analyse('magnified_100_100s_test')
persist_analyse('magnified_200_100s_test')
persist_analyse('magnified_300_100s_test')
persist_analyse('magnified_400_100s_test')
persist_analyse('magnified_500_100s_test')
persist_analyse('magnified_600_100s_test')
plt.grid(True)
plt.xlabel('Time since soak (s)')
plt.ylabel('Normalised Pixel Intensity')
plt.title('Persistence charge-down (Saturation time ~1ms)')
plt.legend(loc='best')
plt.show()