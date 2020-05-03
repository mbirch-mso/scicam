from fabric import Connection
import scicam as cam
import argparse

parser = argparse.ArgumentParser(prog='Remotely Capture and View Image/s', description='Captures and View Image/s remotely on SSH')
parser.add_argument('-a', action='store_true', help='Set to view image/s')
parser.add_argument('-i', type=str, help='Integration Time (sets frame time +20ms)',default='')
parser.add_argument('-l', type=str, help='Number of loops',default='')
parser.add_argument('-g', type=str, help='Naming Tag',default='')
args = parser.parse_args()

home_loc = '//merger.anu.edu.au/mbirch/scicam/'
cmd_pre = lambda script: "python " + home_loc + script + ".py " 
argf = lambda s: " -" + s + " " 
img_loc = '//merger.anu.edu.au/mbirch/images'
def args_form(p='capture',i='',t='',l='',g=''):
    args =''
    if i != '':
        args = args + argf('i') + i
    if l != '':
        args = args + argf('l') + l
    if g != '':
        args = args + argf('g') + g
    return args

#Capture Image
capture_cmd = cmd_pre('capture') + \
args_form(i=args.i,l=args.l,g=args.g)
connection = Connection(host='150.203.88.24', user='mbirch',\
     connect_kwargs={"password": 'BFtrEXZ9'})
connection.run(capture_cmd)

#Display and Analyse Image
if args.a:
    files = cam.grab_from_args('capture',args.i,'',args.g)
    cam.group_display(files)
