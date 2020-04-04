
'''
This program encodes and prepares a packet
for serial communication, runs a batch file for calling the
EDT serial_cmd.c API subroutine, and decodes the response

Typing -h at the command line provides argument informatio into arguments

A two byte hexidemical command is required (without spacing) and 
if data is need at input the program can accept integer, string, or hex
(floats must be pre-encoded into hex)

To decode the serial response an argument -ff, -ii, -ss must be called
if response is expected to be a float, integer, or string respectively
(response is given as hex if no type is specified)

Set Window Column Width
e.g. serial_command.py 1064 -i 640 -ii
                       ^cmd ^int data ^expect int reply

Read VPOS Bias
e.g. serial_command.py 1001 -ff 
                       ^cmd  ^expect float response

'''


import scicam as cam 
import subprocess
import argparse
import sys
import os

# Initialise terminal interface
parser = argparse.ArgumentParser(prog='Serial Command', description='Send Serial Command')
parser.add_argument('cmd', type=str, help='Hexidecimal serial command (no spaces)')
parser.add_argument('-i', '--i_data', dest = 'i_data', type=int, help='Integer Data for Packet', default = 0)
parser.add_argument('-s', '--s_data', dest = 's_data', type=str, help='String Data for Packet', default = 'no_data')
parser.add_argument('-x', '--h_data', dest = 'h_data', type=str, help='Hex Data for Packet', default = 'no_data')
parser.add_argument('-ff', '--float', action='store_true', help='Float reply')
parser.add_argument('-ii', '--int', action='store_true', help='Integer Reply')
parser.add_argument('-ss', '--string', action='store_true', help='String Reply')
args = parser.parse_args()

cmd = args.cmd

# Function to produce and send packet
def serial_cmd(cmd,data="no_data"):
    #Create path string for batch file
    dir_path = os.path.dirname(os.path.realpath(__file__))
    file_path = dir_path + r'\serial_command.bat'
        
    #Append data to packet if required
    if data != 'no_data': 
        packet = cmd + data      
    else:
        packet = cmd
        
    crc = cam.crc16(packet) #Generate CRC-16 bytes
    if len(crc) != 4:
        print('Invalid data (cannot compute CRC)')
        sys.exit()
    else:
        pass
    packet = "3e00ff" + packet + crc +"3e" #Construct packet
    packet = cam.byte_format(packet) #Format into spaced bytes
    print('Sending Packet: {}'.format(packet))
    print('File Path is: {}'.format(file_path))

    #Run batch file and collect output through python
    p = subprocess.Popen([file_path, packet], stdout=subprocess.PIPE, shell=True)
    (output, err) = p.communicate()
    return output, err
# Detect type of data, encode it for communication
# and send serial command to camera
# (does not work for float data so floats must be pre-encoded before running
# this program)
if args.i_data != 0:
    data = cam.int_to_hex(args.i_data)
    print('Hex Input: {}'.format(data))
    output, err = serial_cmd(args.cmd, data)
elif args.s_data != 'no_data':
    data = cam.str_to_hex(args.s_data)
    print('Hex Input: {}'.format(data))
    output, err = serial_cmd(args.cmd, data)
elif args.h_data != 'no_data':
    data = cam.little(args.h_data)
    print('Hex Input: {}'.format(data))
    output, err = serial_cmd(args.cmd, data)
else:
    output, err = serial_cmd(args.cmd)

#Extract serial response from verbose terminal readout
#Actual response is wrapped in <,>
print('The output is being captured as {}'.format(output))
strt = output.find(b'<',0,len(output))
end = output.find(b'>',strt,len(output))
reply = output.decode('utf-8') #Convert to string
reply = reply[strt+1:end] #+1 to cut off '<' symbol
reply = reply.replace(" ", "") #Strip whitespace from string
print('Raw Response: {}'.format(reply))

#Breaks the code if standard error message is returned
if reply == '3ea0bc893e':
    print("Error with Packet Sent")
    sys.exit()
else:
    pass
#Strip fluff bytes
resp = reply.replace("3e00ff" + args.cmd,"") #Cut out flag/ack/cmd bytes
resp = resp[0:-6] #Cut off CRC and end-flag bytes
resp = cam.byte_format(resp) #Format into spaced bytes
print('Hex Response: {}'.format(resp))
#Decode serial response given expected type of response
#Returns hex if not type is specified
if args.float:
    resp = cam.hex_to_float(resp)
    print('Float Response: {}'.format(resp))
elif args.int:
    resp = cam.hex_to_int(resp)
    print('Integer Response: {}'.format(resp))
elif args.string:
    resp = cam.hex_to_str(resp)
    print('String Response: {}'.format(resp))
else:
    print('Hex Response: {}'.format(resp))
