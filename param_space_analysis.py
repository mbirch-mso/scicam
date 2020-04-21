
import numpy as np
import matplotlib.pyplot as plt

int_t = 6 #seconds
n_imgs_1 = 10
n_imgs_2 = 10
n_imgs_2 = 10

def folder(num,day):
    return '//merger.anu.edu.au/mbirch/images/images' + str(day) \
        + '-04-2020/frame_time_study_' + str(num) + '/'

means_1 = np.loadtxt(folder(1,17) + 'means.txt')
medians_1 = np.loadtxt(folder(1,17) + 'medians.txt')
buf_times_1 = np.loadtxt(folder(1,17) + 'buf_times.txt')
frames_1 = np.loadtxt(folder(1,17) + 'frames.txt')

means_2 = np.loadtxt(folder(2,18) + 'means.txt')
medians_2 = np.loadtxt(folder(2,18) + 'medians.txt')
buf_times_2 = np.loadtxt(folder(2,18) + 'buf_times.txt')
frames_2 = np.loadtxt(folder(2,18) + 'frames.txt')

means_3 = np.loadtxt(folder(3,18) + 'means.txt')
medians_3 = np.loadtxt(folder(3,18) + 'medians.txt')
buf_times_3 = np.loadtxt(folder(3,18) + 'buf_times.txt')
frames_3 = np.loadtxt(folder(3,18) + 'frames.txt')

means_4 = np.loadtxt(folder(4,18) + 'means.txt')
medians_4 = np.loadtxt(folder(4,18) + 'medians.txt')
buf_times_4 = np.loadtxt(folder(4,18) + 'buf_times.txt')
frames_4 = np.loadtxt(folder(4,18) + 'frames.txt')

means_5 = np.loadtxt(folder(5,19) + 'means.txt')
medians_5 = np.loadtxt(folder(5,19) + 'medians.txt')
buf_times_5 = np.loadtxt(folder(5,19)  + 'buf_times.txt')
frames_5 = np.loadtxt(folder(5,19) + 'frames.txt')

means_6 = np.loadtxt(folder(6,19) + 'means.txt')
medians_6 = np.loadtxt(folder(6,19) + 'medians.txt')
buf_times_6 = np.loadtxt(folder(6,19) + 'buf_times.txt')
variances_6 = np.loadtxt(folder(6,19) + 'variances.txt')
frames_6 = np.loadtxt(folder(6,19) + 'frames.txt')

means_7 = np.loadtxt(folder(7,19) + 'means.txt')
medians_7 = np.loadtxt(folder(7,19) + 'medians.txt')
buf_times_7 = np.loadtxt(folder(7,19) + 'buf_times.txt')
variances_7 = np.loadtxt(folder(7,19) + 'variances.txt')
frames_7 = np.loadtxt(folder(7,19) + 'frames.txt')

means_8 = np.loadtxt(folder(8,19) + 'means.txt')
medians_8 = np.loadtxt(folder(8,19) + 'medians.txt')
buf_times_8 = np.loadtxt(folder(8,19) + 'buf_times.txt')
variances_8 = np.loadtxt(folder(8,19) + 'variances.txt')
frames_8 = np.loadtxt(folder(8,19) + 'frames.txt')

#Frame_Study_1

# plt.plot(frames_1/1000,means_1,label='Data')
# plt.axvline(6,c='r',ls = '--',label ='Exposure')
# plt.title('Mean ADUs/pixel for 10 coadded integrations ({}s)'.format(int_t))
# plt.xlabel('Frame Time (s)')
# plt.ylabel('Mean ADUs/pixel ({} exposures)'.format(n_imgs_1))
# plt.legend(loc='best')
# plt.show()

# plt.plot(frames_1/1000,medians_1,label='Data')
# plt.axvline(6,c='r',ls = '--',label ='Exposure')
# plt.title('Median ADUs/pixel for 10 coadded integrations ({}s)'.format(int_t))
# plt.xlabel('Frame Time (s)')
# plt.ylabel('Median ADUs/pixel ({} exposures)'.format(n_imgs_1))
# plt.legend(loc='best')
# plt.show()

# x = frames_1/1000
# y = x

# plt.plot(frames_1/1000,buf_times_1,label='Data')
# plt.axvline(6,c='r',ls = '--',label ='Exposure')
# plt.plot(x,y,label='Linear buffer to frame time')
# plt.title('Mean time to fill buffer for 10 coadded integrations ({}s)'.format(int_t))
# plt.xlabel('Frame Time (s)')
# plt.ylabel('Buffer Time (s)')
# plt.legend(loc='best')
# plt.show()


# #Frame_Study_2

# plt.plot(frames_2,means_2,label='Data')
# plt.axvline(6000,c='r',ls = '--',label ='Exposure')
# plt.title('Mean ADUs/pixel for 10 coadded integrations ({}s)'.format(int_t))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Mean ADUs/pixel ({} exposures)'.format(n_imgs_1))
# plt.legend(loc='best')
# plt.show()

# plt.plot(frames_2,medians_2,label='Data')
# plt.axvline(6000,c='r',ls = '--',label ='Exposure')
# plt.title('Median ADUs/pixel for 10 coadded integrations ({}s)'.format(int_t))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Median ADUs/pixel ({} exposures)'.format(n_imgs_1))
# plt.legend(loc='best')
# plt.show()

# plt.plot(frames_2,buf_times_2*1000,label='Data')
# plt.axvline(6000,c='r',ls = '--',label ='Exposure')
# plt.title('Mean time to fill buffer for 10 coadded integrations ({}s)'.format(int_t))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Buffer Time (ms)')
# plt.legend(loc='best')
# plt.show()


# #Frame_Study_3

# plt.plot(frames_3,means_3,label='Data')
# plt.axvline(6000,c='r',ls = '--',label ='Exposure')
# plt.title('Mean ADUs/pixel for 10 coadded integrations ({}s)'.format(int_t))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Mean ADUs/pixel ({} exposures)'.format(n_imgs_1))
# plt.legend(loc='best')
# plt.show()

# plt.plot(frames_3,medians_3,label='Data')
# plt.axvline(6000,c='r',ls = '--',label ='Exposure')
# plt.title('Median ADUs/pixel for 10 coadded integrations ({}s)'.format(int_t))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Median ADUs/pixel ({} exposures)'.format(n_imgs_1))
# plt.legend(loc='best')
# plt.show()

# plt.plot(frames_3,buf_times_3*1000,label='Data')
# plt.axvline(6000,c='r',ls = '--',label ='Exposure')
# plt.title('Mean time to fill buffer for 10 coadded integrations ({}s)'.format(int_t))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Buffer Time (ms)')
# plt.legend(loc='best')
# plt.show()


# #Frame_Study_4

# plt.plot(frames_4,means_4,label='Data')
# plt.axvline(1000,c='r',ls = '--',label ='Exposure')
# plt.title('Mean ADUs/pixel for 25 coadded integrations ({}s)'.format(1))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Mean ADUs/pixel ({} exposures)'.format(n_imgs_1))
# plt.legend(loc='best')
# plt.show()

# plt.plot(frames_4,medians_4,label='Data')
# plt.axvline(1000,c='r',ls = '--',label ='Exposure')
# plt.title('Median ADUs/pixel for 25 coadded integrations ({}s)'.format(1))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Median ADUs/pixel ({} exposures)'.format(n_imgs_1))
# plt.legend(loc='best')
# plt.show()

# plt.plot(frames_4,buf_times_4*1000,label='Data')
# plt.axvline(1000,c='r',ls = '--',label ='Exposure')
# plt.title('Mean time to fill buffer for 25 coadded integrations ({}s)'.format(1))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Buffer Time (ms)')
# plt.legend(loc='best')
# plt.show()


# #Frame_Study_5

# plt.plot(frames_5,means_5,label='Data')
# plt.axvline(1000,c='r',ls = '--',label ='Exposure')
# plt.title('Mean ADUs/pixel for 10 coadded integrations ({}s)'.format(1))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Mean ADUs/pixel ({} exposures)'.format(n_imgs_1))
# plt.legend(loc='best')
# plt.show()

# plt.plot(frames_5,medians_5,label='Data')
# plt.axvline(1000,c='r',ls = '--',label ='Exposure')
# plt.title('Median ADUs/pixel for 10 coadded integrations ({}s)'.format(1))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Median ADUs/pixel ({} exposures)'.format(n_imgs_1))
# plt.legend(loc='best')
# plt.show()

# plt.plot(frames_5,buf_times_5*1000,label='Data')
# plt.axvline(1000,c='r',ls = '--',label ='Exposure')
# plt.title('Mean time to fill buffer for 10 coadded integrations ({}s)'.format(1))
# plt.xlabel('Frame Time (ms)')
# plt.ylabel('Buffer Time (ms)')
# plt.legend(loc='best')
# plt.show()

#Frame_Study_6

plt.plot(frames_6,means_6,label='Data')
plt.axvline(1000,c='r',ls = '--',label ='Exposure')
plt.title('Mean ADUs/pixel for 10 coadded integrations ({}s)'.format(1))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Mean ADUs/pixel ({} exposures)'.format(n_imgs_1))
plt.legend(loc='best')
plt.show()

plt.plot(frames_6,medians_6,label='Data')
plt.axvline(1000,c='r',ls = '--',label ='Exposure')
plt.title('Median ADUs/pixel for 10 coadded integrations ({}s)'.format(1))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Median ADUs/pixel ({} exposures)'.format(n_imgs_1))
plt.legend(loc='best')
plt.show()

plt.plot(frames_6,buf_times_6*1000,label='Data')
plt.axvline(1000,c='r',ls = '--',label ='Exposure')
plt.title('Mean time to fill buffer for 10 coadded integrations ({}s)'.format(1))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Buffer Time (ms)')
plt.legend(loc='best')
plt.show()

plt.plot(frames_6,variances_6,label='Data')
plt.axvline(1000,c='r',ls = '--',label ='Exposure')
plt.title('Variance for 10 coadded integrations ({}s)'.format(1))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Variance (ADUs)')
plt.legend(loc='best')
plt.show()


#Frame_Study_7

plt.plot(frames_7,means_7,label='Data')
plt.axvline(6000.001,c='r',ls = '--',label ='Exposure')
plt.title('Mean ADUs/pixel for 25 coadded integrations ({}s)'.format(6))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Mean ADUs/pixel ({} exposures)'.format(n_imgs_1))
plt.legend(loc='best')
plt.show()

plt.plot(frames_7,medians_7,label='Data')
plt.axvline(6000.001,c='r',ls = '--',label ='Exposure')
plt.title('Median ADUs/pixel for 25 coadded integrations ({}s)'.format(6))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Median ADUs/pixel ({} exposures)'.format(n_imgs_1))
plt.legend(loc='best')
plt.show()

plt.plot(frames_7,buf_times_7*1000,label='Data')
plt.axvline(6000.001,c='r',ls = '--',label ='Exposure')
plt.title('Mean time to fill buffer for 25 coadded integrations ({}s)'.format(6))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Buffer Time (ms)')
plt.legend(loc='best')
plt.show()

plt.plot(frames_7,variances_7,label='Data')
plt.axvline(6000.001,c='r',ls = '--',label ='Exposure')
plt.title('Variance for 25 coadded integrations ({}s)'.format(6))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Variance (ADUs)')
plt.legend(loc='best')
plt.show()


#Frame_Study_8

plt.plot(frames_8,means_8,label='Data')
plt.axvline(6000.001,c='r',ls = '--',label ='Exposure')
plt.title('Mean ADUs/pixel for 25 coadded integrations ({}s)'.format(6))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Mean ADUs/pixel ({} exposures)'.format(n_imgs_1))
plt.legend(loc='best')
plt.show()

plt.plot(frames_8,medians_8,label='Data')
plt.axvline(6000.001,c='r',ls = '--',label ='Exposure')
plt.title('Median ADUs/pixel for 25 coadded integrations ({}s)'.format(6))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Median ADUs/pixel ({} exposures)'.format(n_imgs_1))
plt.legend(loc='best')
plt.show()

plt.plot(frames_8,buf_times_8*1000,label='Data')
plt.axvline(6000.001,c='r',ls = '--',label ='Exposure')
plt.title('Mean time to fill buffer for 25 coadded integrations ({}s)'.format(6))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Buffer Time (ms)')
plt.legend(loc='best')
plt.show()

plt.plot(frames_8,variances_8,label='Data')
plt.axvline(6000.001,c='r',ls = '--',label ='Exposure')
plt.title('Variance for 25 coadded integrations ({}s)'.format(6))
plt.xlabel('Frame Time (ms)')
plt.ylabel('Variance (ADUs)')
plt.legend(loc='best')
plt.show()