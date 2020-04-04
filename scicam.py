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
    crc = crc[2:6]
    return crc

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
    if len(crc) != 4:
        print('Invalid data (cannot compute CRC)')
        sys.exit()
    else:
        pass
    packet = "3e00ff" + packet + crc +"3e" #Construct packet
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
        output, _ , packet= send_to_cam(cmd, data)
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
        print("Error with Packet Sent (error code)")
        sys.exit()
    else:
        pass
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
    else: 
        pass

    return resp

#Set integration time in milliseconds
def set_int_time(t,verbose=False):
    t = t / 1000
    #Convert time to number of clocks@16MHz
    clk_speed = 16E6 #Hz
    clks = t * clk_speed #Integer no. of clocks
    clks_hex = int_to_hex(int(clks))
    #Pad out data input
    while len(clks_hex) != 8:
        clks_hex = '00' + clks_hex
    clks_hex = little(clks_hex)
    #Send command to write integrtion time
    if verbose == True:
        check_clks_hex = command('106c',clks_hex,verbose=True)
    else: 
        check_clks_hex = command('106c',clks_hex)
    #Convert hex response into clocks then milliseconds
    check_clks = hex_to_int(check_clks_hex)
    check_time = check_clks / clk_speed
    check_time = check_time * 1000
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
    check_time = check_time * 1000
    return check_time

#Similar to set_int_time but calls command for frame time
def set_frame_time(t,verbose=False,rate=False):
    #Takes Hz or ms as input
    if rate == True:
        t = 1 / t
    else:
        t = t / 1000
    #Convert time to number of clocks@16MHz
    clk_speed = 16E6 #Hz
    clks = t * clk_speed #Integer no. of clocks
    clks_hex = int_to_hex(int(clks))
    #Pad out data input
    while len(clks_hex) != 8:
        clks_hex = '00' + clks_hex
    clks_hex = little(clks_hex)
    #Send command to write frame time
    if verbose == True:
        check_clks_hex = command('106e',clks_hex,verbose=True)
    else: 
        check_clks_hex = command('106e',clks_hex)
    #Convert hex response into clocks then milliseconds
    check_clks = hex_to_int(check_clks_hex)
    check_time = check_clks / clk_speed
    check_rate = 1 / check_time
    check_time = check_time * 1000
    print('Frame Rate is now: {0}Hz, Frame Time is now: {1}ms'.format(check_rate,check_time))
    return check_time

def read_frame_time(verbose=False):
    if verbose == True:
        check_clks_hex = command('106f',verbose=True)
    else: 
        check_clks_hex = command('106f')
    #Convert hex response into clocks then milliseconds
    check_clks = hex_to_int(check_clks_hex)
    check_time = check_clks / 16E6 
    check_time = check_time * 1000
    return check_time

#Sorts file into appropriate folder and renames appropriately
def file_sorting(loc,i,t,ext='.fits',routine='simple_take_fits'):
    now = datetime.datetime.now()
    today = now.strftime("%d-%m-%Y")
    folder_name = loc + '/images' + today
    k = 0
    image_name = '/' + routine + '_' + str(i) + '_' + str(t) + '_' + str(k) + ext
    if os.path.exists(folder_name) == True:
        while os.path.exists(folder_name + image_name) == True:
            k = k + 1
            image_name = '/' + routine + '_' + str(i) + '_' + str(t) + '_' + str(k) + ext
    else:
        os.chdir(loc)
        os.mkdir('images' + today)
        while os.path.exists(folder_name + image_name) == True:
            k = k + 1
            image_name = '/' + routine + '_' + str(i) + '_' + str(t) + '_' + str(k) + ext
    os.rename(loc + '/image_unsorted' + ext, folder_name + image_name)
    #Write int and frame time into header
    if ext == '.fits':
        with fits.open(folder_name+image_name, mode = 'update') as hdu:
            hdu_header = hdu[0].header
            hdu_header.set('INT_T',i,'Integration time in ms')
            hdu_header.set('FRAME_T',t,'Frame time in ms')
            hdu_header.set('ROUTINE',routine,'EDT Routine used for capture')
            hdu.flush()
    else:
        pass
    print('Successfully saved image to {}'.format(folder_name + image_name))
    return folder_name+image_name

def import_fits(routine='simple_take_fits',date='today',i=False,t=False,\
    loc='//merger.anu.edu.au/mbirch/images/images'):
    if date == 'today':
        now = datetime.datetime.now()
        date = now.strftime("%d-%m-%Y")
    else:
        pass
    folder_name = loc + date + '/'
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
