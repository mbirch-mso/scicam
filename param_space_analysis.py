
import numpy as np
import matplotlib.pyplot as plt

int_t = 6 #seconds
n_imgs = 10
img_loc = '//merger.anu.edu.au/mbirch/images'
test_tag = 'frame_time_study_1'
date = '17-04-2020'
folder_path = img_loc + '/' + 'images' + date + '/' + test_tag + '/'

means_1 = np.loadtxt(folder_path  + 'means.txt')
medians_1 = np.loadtxt(folder_path  + 'medians.txt')
buf_times_1 = np.loadtxt(folder_path  + 'buf_times.txt')
frames_1 = np.loadtxt(folder_path  + 'frames.txt')


plt.plot(frames_1,means_1)
plt.axvline(6000,c='r',ls = '--')
plt.title('Mean ADUs for 10 coadded integrations ({}s)'.format(int_t))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Mean ADUs/pixel ({} exposures)'.format(n_imgs))
plt.show()

plt.plot(frames_1,medians_1)
plt.axvline(6000,c='r',ls = '--')
plt.title('Median ADUs for 10 coadded integrations ({}s)'.format(int_t))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Median ADUs/pixel ({} exposures)'.format(n_imgs))
plt.show()

plt.plot(frames_1,buf_times_1)
plt.axvline(6000,c='r',ls = '--')
plt.title('Mean time to fill buffer for 10 coadded integrations ({}s)'.format(int_t))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Buffer Time (ms)')
plt.show()

