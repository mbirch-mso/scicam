from fabric import Connection
import fits_importing as fits
import subprocess
import scicam as cam
import argparse

parser = argparse.ArgumentParser(prog='capture Image', description='Captures and View Images remotely on SSH')
parser.add_argument('-a', type=str, help='Type of Image Analysis')
parser.add_argument('-p', type=str, help='Routine for image capture',default='')
parser.add_argument('-i', type=str, help='Integration Time',default='')
parser.add_argument('-t', type=str, help='Frame Time',default='')
parser.add_argument('-l', type=str, help='Number of loops',default='')
args = parser.parse_args()

print(args)
home_loc = '//merger.anu.edu.au/mbirch/scicam/'
cmd_pre = lambda script: home_loc + "python " + script + ".py " 
argf = lambda s: " -" + s + " " 
img_loc = '//merger.anu.edu.au/mbirch/images'
def args_form(p='simple_take_fits',i='',t='',l=''):
    args = p
    if i != '':
        args = args + argf('i') + i
    if t != '':
        args = args + argf('t') + t
    if l != '':
        args = args + argf('l') + l
    #By default same image to home_dir as fits file
    return args + argf('f') + img_loc

#Capture Image
if args.p:
    capture_cmd = cmd_pre('capture') + \
    args_form(args.p,args.i,args.t,args.l)
else: 
    capture_cmd = cmd_pre('capture') + \
    args_form(i=args.i,t=args.t,l=args.l)

#Connect to camera server
connection = Connection(host='150.203.88.24', user='mbirch',\
     connect_kwargs={"password": 'BFtrEXZ9'})
connection.run(capture_cmd)

print('images successfully captured')
#Display and Analyse Image
if args.a:
    files = cam.grab_from_args()
    if args.a == 'view':
        print('in view')
        fits.group_display(files)
    elif args.a == 'hist':
        fits.group_hist(files)
    else:
        pass
else:
    pass

print(capture_cmd)
print(files)