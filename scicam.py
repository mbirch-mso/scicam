
import crcmod
import ctypes
import binascii
import struct
import subprocess
import sys
import os
import datetime
import numpy as np
import glob
from astropy.io import fits
from ftplib import FTP
import xml.etree.ElementTree as ET
from astral import moon
import matplotlib.pyplot as plt

#Convert to little endianness
def little(s):
        t = bytearray.fromhex(s)
        t.reverse()
        return ''.join(format(x,'02x') for x in t).upper()

#Hex to float w/ little endianness
def hex_to_float(s):
    s_little = little(s) #Switches Endianness
    s_bytes = bytes.fromhex(s_little)
    float_val = struct.unpack('!f', s_bytes)[0]
    return float_val        # dereference the pointer, get the float 

#Hex to integer w/ little endianness
def hex_to_int(s):
    s_little = little(s) #Switches Endianness
    int_val = int(s_little, 16)
    return int_val

#Hex to string (incl. null char)
def hex_to_str(s):
    return bytearray.fromhex(s).decode()

#Convert string to hex (adds null char)
def str_to_hex(s):
    s = s.encode('utf-8')
    hex_val = s.hex()
    hex_val = hex_val + '00' #Add null char
    return hex_val
 
#Convert int to hex (removes 0x format)
def int_to_hex(s):
    hex_val = hex(s)[2:]
    if len(hex_val)%2 == 0:
        pass
    else:
        hex_val = '0'+ hex_val #Pad 0 if hex cant be sorted into bytes
    return hex_val

#Outputs CRC bytes for command in hex
def crc16(cmd):
    cmd = bytearray.fromhex('00ff'+cmd)
    crc16_func = crcmod.mkCrcFun(0x1755B,initCrc=0,rev=False,xorOut=0xFFFF)
    crc = hex(crc16_func(cmd))
    if len(crc) != 6:
        return 'crc_err'
    else:
        return crc[2:6]

#Format hex string into octets
def byte_format(s):
    result = [s[i:i+2] for i in range(0, len(s), 2)]
    s = " ".join(result)
    return s

#Send command and data packet to camera in hex
#Output is raw response string and requires
#command function for proper use
def send_to_cam(cmd,data="no_data"):
    #Create path string for batch file
    dir_path = os.path.dirname(os.path.realpath(__file__))
    file_path = dir_path + r'\serial_command.bat'
    #Append data to packet if required
    if data != 'no_data': 
        packet = cmd + data      
    else:
        packet = cmd
    crc = crc16(packet) #Generate CRC-16 bytes
    if crc == 'crc_err': #CRC error checking
        print('Invalid data (cannot compute CRC)...trying again')
        return 'crc_err'
    else:
        packet = "3e00ff" + packet + crc + "3e" #Construct packet
        packet = byte_format(packet) #Format into spaced bytes
        #Run batch file and collect output through python
        p = subprocess.Popen([file_path, packet], stdout=subprocess.PIPE, shell=True)
        (output, err) = p.communicate()
        return output, err, packet


#Calls send_to_cam with hex input of command and data
#Retrieves response from raw string and decodes it
def command(cmd,data="no_data",verbose=False):
    #Check if data packet is required
    if data != 'no_data':
        try:
            output, _ , packet= send_to_cam(cmd, data)
            #Extract serial response from verbose terminal readout
            #Actual response is wrapped in <,>
            strt = output.find(b'<',0,len(output))
            end = output.find(b'>',strt,len(output))
            reply = output.decode('utf-8') #Convert to string
            reply = reply[strt+1:end] #+1 to cut off '<' symbol
            reply = reply.replace(" ", "") #Strip whitespace from string
            #Breaks the code if standard error message is returned
            if reply == '3ea0bc893e':
                print('Raw Response: {}'.format(reply))
                print("Error with Serial Packet Sent (std error code)")
                return 'serial_err'
            else:
                #Strip fluff bytes
                resp = reply.replace("3e00ff" + cmd,"") #Cut out flag/ack/cmd bytes
                resp = resp[0:-6] #Cut off CRC and end-flag bytes
                resp = byte_format(resp) #Format into spaced bytes
                #Print responses from different stages if required
                if verbose == True:
                    print('Packet sent: {}'.format(packet))
                    print('Captured output is: {}'.format(output))
                    print('Raw Response: {}'.format(reply))
                    print('Hex Response: {}'.format(resp))
                return resp
        except ValueError:
            return 'crc_err'
    else:
        output, _, packet = send_to_cam(cmd)
        #Extract serial response from verbose terminal readout
        #Actual response is wrapped in <,>
        strt = output.find(b'<',0,len(output))
        end = output.find(b'>',strt,len(output))
        reply = output.decode('utf-8') #Convert to string
        reply = reply[strt+1:end] #+1 to cut off '<' symbol
        reply = reply.replace(" ", "") #Strip whitespace from string
        #Breaks the code if standard error message is returned
        if reply == '3ea0bc893e':
            print('Raw Response: {}'.format(reply))
            print("Error with Serial Packet Sent (std error code)")
            #Error reporting code here
            return 'serial_err'
        else:
            #Strip fluff bytes
            resp = reply.replace("3e00ff" + cmd,"") #Cut out flag/ack/cmd bytes
            resp = resp[0:-6] #Cut off CRC and end-flag bytes
            resp = byte_format(resp) #Format into spaced bytes
            #Print responses from different stages if required
            if verbose == True:
                print('Packet sent: {}'.format(packet))
                print('Captured output is: {}'.format(output))
                print('Raw Response: {}'.format(reply))
                print('Hex Response: {}'.format(resp))
            return resp

#Convert seconds of integration time to ref clocks in hexadecimal
def secs_to_refclks(t):
    clks = t * 16E6 #Integer no. of clocks@16MHz
    clks_hex = int_to_hex(int(clks))
    #Pad out data input
    while len(clks_hex) != 8:
        clks_hex = '00' + clks_hex
    return little(clks_hex)

#Set integration time in milliseconds
def set_int_time(t,verbose=False):
    t = t / 1000
    clks_hex = secs_to_refclks(t)
    #Send command to write integrtion time
    if verbose == True:
        check_clks_hex = command('106c',clks_hex,verbose=True)
        while ((check_clks_hex == 'crc_err') or (check_clks_hex == 'serial_err')):
            t = t + 0.001 #Create new time by adding 0.1ms
            print("Calculating for new time: {}ms".format(t*1000))
            clks_hex = secs_to_refclks(t) #Add ms if CRC not working
            check_clks_hex = command('106c',clks_hex,verbose=True)
    else: 
        check_clks_hex = command('106c',clks_hex)
        while ((check_clks_hex == 'crc_err') or (check_clks_hex == 'serial_err')):
            t = t + 0.001 #Create new time by adding 0.1ms
            print("Calculating for new time: {}ms".format(t*1000))
            clks_hex = secs_to_refclks(t) #Add ms if CRC not working
            check_clks_hex = command('106c',clks_hex)

    #Convert hex response into clocks then milliseconds
    try:
        check_clks = hex_to_int(check_clks_hex)
    except ValueError:
        t = t + 0.001 #Create new time by adding 1ms
        print("Unknown Error\nCalculating for new time: {}ms".format(t*1000))
        clks_hex = secs_to_refclks(t) #Add ms if CRC not working
        check_clks_hex = command('106c',clks_hex)
        while ((check_clks_hex == 'crc_err') or (check_clks_hex == 'serial_err')):
            t = t + 0.001 #Create new time by adding 0.1ms
            print("Calculating for new time: {}ms".format(t*1000))
            clks_hex = secs_to_refclks(t) #Add ms if CRC not working
            check_clks_hex = command('106c',clks_hex)
        check_clks = hex_to_int(check_clks_hex)
    
    check_time = check_clks / 16E6
    check_time = round((check_time * 1000),4)
    print('Integration Time is now: {}ms'.format(check_time))
    return check_time

def read_int_time(verbose=False):
    if verbose == True:
        check_clks_hex = command('106d',verbose=True)
    else: 
        check_clks_hex = command('106d')
    #Convert hex response into clocks then milliseconds
    check_clks = hex_to_int(check_clks_hex)
    check_time = check_clks / 16E6 
    check_time = round((check_time * 1000),0)
    return check_time

#Similar to set_int_time but calls command for frame time
def set_frame_time(t,verbose=False,rate=False):
    #Takes Hz or ms as input
    if rate == True:
        t = 1 / t
    else:
        t = t / 1000
    clks_hex = secs_to_refclks(t)
    #Send command to write frame time
    if verbose == True:
        check_clks_hex = command('106e',clks_hex,verbose=True)
        while ((check_clks_hex == 'crc_err') or (check_clks_hex == 'serial_err')):
            t = t + 0.001 #Create new time by adding 0.1ms
            print("Calculating for new time: {}ms".format(t*1000))
            clks_hex = secs_to_refclks(t) #Add ms if CRC not working
            check_clks_hex = command('106e',clks_hex,verbose=True)
    else: 
        check_clks_hex = command('106e',clks_hex)
        while ((check_clks_hex == 'crc_err') or (check_clks_hex == 'serial_err')):
            t = t + 0.001 #Create new time by adding 0.1ms
            print("Calculating for new time: {}ms".format(t*1000))
            clks_hex = secs_to_refclks(t) #Add ms if CRC not working
            check_clks_hex = command('106e',clks_hex)

    #Convert hex response into clocks then milliseconds
    try:
        check_clks = hex_to_int(check_clks_hex)
    except ValueError:
        t = t + 0.001 #Create new time by adding 1ms
        print("Unknown Error\nCalculating for new time: {}ms".format(t*1000))
        clks_hex = secs_to_refclks(t) #Add ms if CRC not working
        check_clks_hex = command('106e',clks_hex,verbose=True)
        while ((check_clks_hex == 'crc_err') or (check_clks_hex == 'serial_err')):
            t = t + 0.001 #Create new time by adding 0.1ms
            print("Calculating for new time: {}ms".format(t*1000))
            clks_hex = secs_to_refclks(t) #Add ms if CRC not working
            check_clks_hex = command('106e',clks_hex)
        check_clks = hex_to_int(check_clks_hex)
    
    check_time = check_clks / 16E6
    check_time = round((check_time * 1000),4)
    print('Frame Time is now: {}ms'.format(check_time))
    return check_time

def read_frame_time(verbose=False):
    if verbose == True:
        check_clks_hex = command('106f',verbose=True)
    else: 
        check_clks_hex = command('106f')
    #Convert hex response into clocks then milliseconds
    check_clks = hex_to_int(check_clks_hex)
    check_time = check_clks / 16E6 
    check_time = round((check_time * 1000),0)
    return check_time

#Sorts file into appropriate folder and renames appropriately
def file_sorting(loc,i,t,ext='.fits',routine='capture',tag=''):
    #Define name of folder(s) file is to be sorted into
    now = datetime.datetime.now()
    today = now.strftime("%d-%m-%Y")
    new_fold = 'images' + today
    if tag != '':
        new_fold = new_fold + '/' + tag

    folder_name = loc + '/' + new_fold
    k = 0
    image_name = '/' + routine + '_' + str(i) + '_' + str(t) + '_' + str(k) + ext
    if os.path.exists(folder_name) == True:
        while os.path.exists(folder_name + image_name) == True:
            k = k + 1
            image_name = '/' + routine + '_' + str(i) + '_' + str(t) + '_' + str(k) + ext
    else:
        os.chdir(loc)
        try:
            os.mkdir('images' + today)
            if tag != '':
                os.chdir('images' + today)
                os.mkdir(tag)
        except FileExistsError:
            os.chdir('images' + today)
            os.mkdir(tag)
        os.chdir(os.path.dirname(os.path.realpath(__file__))) #Reset to script path
        print(os.getcwd())
        while os.path.exists(folder_name + image_name) == True:
            k = k + 1
            image_name = '/' + routine + '_' + str(i) + '_' + str(t) + '_' + str(k) + ext
 
    
    os.rename(loc + '/image_unsorted' + ext, folder_name + image_name)
    
    with fits.open((folder_name+image_name),mode='update') as hdu:
        head = hdu[0].header
        head.append(('DITSER',i,'Integration time in ms (serial)')) 
    
    print('Successfully saved image to {}'.format(folder_name + image_name))
    
    return folder_name+image_name

def import_fits(routine='capture',date='today',i=False,t=False,\
    loc='//merger.anu.edu.au/mbirch/images/images',tag=False):
    if date == 'today':
        now = datetime.datetime.now()
        date = now.strftime("%d-%m-%Y")
    folder_name = loc + date + '/'
    
    if tag != False:
        folder_name = folder_name + tag + '/'

    if i != False:
        if t != False:
            file_name  =  "*" + routine + "_" +\
                 str(float(i)) + "_"+ str(float(t)) + "*"
            files = glob.glob(folder_name+file_name)
        else:
            file_name = "*" + routine + "_" +\
                 str(float(i)) + "*"
            files = glob.glob(folder_name+file_name)
    else:
        if t != False:
            file_name = "*" + routine + "*"
            ext_t = "*"+ str(float(t)) + "*"
            files = [item for sublist in [glob.glob(folder_name + ext)\
                    for ext in [file_name, ext_t]] \
                    for item in sublist]
        else:
            files = glob.glob(folder_name + "*" + routine + "*")
    return files


#Function to grab files for any given number of specific args
def grab_from_args(p='',i='',t='',tag=''):
    if p != '':
        if i != '':
            if t != '':
                if tag != '':
                    files = import_fits(routine = p,i=i,t=t,tag=tag)
                else:
                    files = import_fits(routine = p,i=i,t=t)
            else:
                if tag != '':
                    files = import_fits(routine = p,i=i,tag=tag)
                else:
                    files = import_fits(routine = p,i=i)
        else:
            if t != '':
                if tag != '':
                    files = import_fits(routine = p,t=t,tag=tag)
                else:
                    files = import_fits(routine = p,t=t)
            else:
                if tag != '':
                    files = import_fits(routine = p,tag=tag)
                else:
                    files = import_fits(routine = p)
    else:
        if i != '':
            if t != '':
                if tag != '':
                    files = import_fits(i=i,t=t,tag=tag)
                else:
                    files = import_fits(i=i,t=t)
            else:
                if tag != '':
                    files = import_fits(i=i,tag=tag)
                else:
                    files = import_fits(i=i)
        else:
            if t != '':
                if tag != '':
                    files = import_fits(t=t,tag=tag)
                else:
                    files = import_fits(t=t)
            else:
                if tag != '':
                    files = import_fits(tag=tag)
                else:
                    files = import_fits()
    return files

#Function to display list of fits files as overlayed histograms
def group_hist(files):
    for k in range(len(files)):
        img = fits.open(files[k])[0]
        img_data = img.data
        plt.hist(img_data.flatten(), bins=400)
    plt.title('{}'.format(img.header['ROUTINE']))
    plt.xlabel('Counts (ADUs)')
    plt.ylabel('# of Pixels')
    plt.show()

#Function to display list of fits files as grid
def group_display(files):
    #Code for the case of a single image
    #otherwise construct grid geometry
    #Display header in format (int_time,frame_time,Reg_status 0x01,FVSC)
    Tot = len(files)
    try:
        Cols = Tot//2
        Rows = Tot // Cols 
        Rows += Tot % Cols
    except ZeroDivisionError:
        Cols = 1
        Rows = 1
    Position = range(1,Tot + 1)
    fig = plt.figure(1)
    for k in range(Tot):
        img = fits.open(files[k])[0]
        print(img.header)
        ax = fig.add_subplot(Rows,Cols,Position[k])
        ax.imshow(img.data)
        ax.set_xticks([])
        ax.set_yticks([])
    plt.show()   

#Generic capture command that runs capture.bat 
#with given routine and leaves unsorted
def img_cap(routine, loc = False, form = 'f'):
    #Create path string for batch file
    if loc != False:
        loc = loc +'/image_unsorted'
    else:
        loc = os.path.dirname(os.path.realpath(__file__))
        #import code for file naming convention
        loc = loc + '/image_unsorted'
    path = os.path.dirname(os.path.realpath(__file__))
    file_path = path + r'\capture.bat'
    #Run batch file and collect output through python
    p = subprocess.Popen([file_path, routine, loc, form], stdout=subprocess.PIPE, shell=True)
    (output, err) = p.communicate()
    return output, err

#Function adds BOM Canberr Airport weather statistics
#and moon phase to FITS header
def weather_to_fits(target):
    #Access BOM FTP service for NSW/ACT Forecast
    stat_id = 'IDN60920.xml' #CANBERRA AIRPORT SUMMARY
    ftp = FTP('ftp2.bom.gov.au')
    ftp.login()
    ftp.cwd('anon/gen/fwo')
    with open(stat_id, 'wb') as fp:
        ftp.retrbinary('RETR '+ stat_id, fp.write)
    ftp.quit()
    
    #Retrieve forecast for Canberra
    tree = ET.parse(stat_id)
    root = tree.getroot()
    stat_weather = root.findall(".//observations/station/[@stn-name='CANBERRA AIRPORT']/period/level/")
    keys = ['CLOUD','CLOUDOKT','DELTA_T','GUST','AIR_TEMP',\
            'PRESSURE','HUMID','WIND_DIR','WIND_SPD','PRECIP']
    choice = ['cloud','cloud_oktas',\
        'delta_t','gust_kmh','air_temperature','pres','rel-humidity',\
            'wind_dir_deg','wind_spd_kmh','rainfall']
    
    with fits.open(target,mode='update') as hdu:
        target_head = hdu[0].header
        k = 0
        for i in range(len(stat_weather)):
            if stat_weather[i].attrib['type'] in choice:
                try:
                    comment = stat_weather[i].attrib['type']\
                        + '  ' + stat_weather[i].attrib['units']
                except KeyError:
                    comment = stat_weather[i].attrib['type']
                try: 
                    entry = round(float(stat_weather[i].text),2)
                except ValueError:
                    entry = stat_weather[i].text
                target_head.append((keys[k],entry,comment))
                k += 1
        
        phase = round(moon.phase(datetime.date.today()),2)
        
        target_head.append(('MOON',phase,'phase of moon (0-28)'))
        print(target_head)

# Print progress
def printProgressBar (iteration, total, prefix = '', suffix = '', decimals = 1, length = 100, fill = '█', printEnd = "\r"):
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    print('\r%s |%s| %s%% %s' % (prefix, bar, percent, suffix), end = printEnd)
    if iteration == total: 
        print()

#Stack frames and perform z-wise median collapse
def median_stack(i,folder):
    os.chdir(folder)
    img_list = glob.glob('*.fits*')
    img_list_split = [i.split('_') for i in img_list]
    stack = np.zeros((1040,1296))
    for k in range(len(img_list)):
        if img_list_split[k][1] == str(float(i)):
            hdu = fits.open(img_list[k])
            data = hdu[0].data
            stack = np.dstack((stack,data))
    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    #Collapse multi-dimensional array along depth axis by median
    return np.median(stack, axis=2)

def get_master_bias(temp):
    bias_dir = '//merger.anu.edu.au/mbirch/data/master_bias/'
    bias_file = bias_dir + str(temp) + 'C.fits'
    master_bias = fits.open(bias_file)[0]
    return master_bias.data

def simple_capture():
    '''
    Take single image and return numpy array and header
    '''
    img_dir = '//merger.anu.edu.au/mbirch/images'
    cap, _ = cam.img_cap('capture',img_dir,'f')
    hdu_img = fits.open(unsorted_img)
    data = hdu_img[0].data
    header = hdu_img[0].header
    hdu_img.close() #Close image so it can be sorted
    os.remove(unsorted_img) #Delete image after data retrieval 
    return data, header

def expose(i,n,path=False,tag=''):
    '''
    Capture image for given NDIT/DIT
    Can write user-defined comment to header
    Adds recent Canberra BOM weather to header
    Can save to custom file path or auto-sort
    '''
    int_t = set_int_time(i)
    frame_t = set_frame_time(i+25)
    array = np.zeros(shape = (naxis1,naxis2)) #Initiate array for coadding
    for j in range(n):
        cap, _ = cam.img_cap(routine,img_dir,'f')
        hdu_img = fits.open(unsorted_img)
        fits_img = hdu_img[0]
        array += fits_img.data
        hdu_img.close() #Close image so it can be sorted

        if j == n-1: #On final frame grab header
            img_header = fits.getheader(unsorted_img)
            img_header.append(('NDIT',n,'Number of integrations'))
            if tag != '':
                img_header.append(('COMMENT',tag,'User-defined comment'))
        
        os.remove(unsorted_img) #Delete image after data retrieval 
    
    if path:
        fits.writeto(path,array,img_header)
        weather_to_fits(path)
    else:
        fits.writeto(unsorted_img,array,img_header)
        weather_to_fits(unsorted_img)
        file_sorting(img_dir,int_t,frame_t,tag=tag)
    print('EXPOSE COMPLETE')
