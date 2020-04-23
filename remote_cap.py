from fabric import Connection
import scicam as cam
import argparse


parser = argparse.ArgumentParser(prog='Remotely Capture and View Image/s', description='Captures and View Images remotely on SSH')
parser.add_argument('-a', type=str, help='Type of Image Analysis')
parser.add_argument('-p', type=str, help='Routine for image capture',default='')
parser.add_argument('-i', type=str, help='Integration Time',default='')
parser.add_argument('-t', type=str, help='Frame Time',default='')
parser.add_argument('-l', type=str, help='Number of loops',default='')
parser.add_argument('-g', type=str, help='Naming Tag',default='')
args = parser.parse_args()

home_loc = '//merger.anu.edu.au/mbirch/scicam/'
cmd_pre = lambda script: "python " + home_loc + script + ".py " 
argf = lambda s: " -" + s + " " 
img_loc = '//merger.anu.edu.au/mbirch/images'
def args_form(p='capture',i='',t='',l='',g=''):
    args = p
    if i != '':
        args = args + argf('i') + i
    if t != '':
        args = args + argf('t') + t
    if l != '':
        args = args + argf('l') + l
    if g != '':
        args = args + argf('g') + g
    #By default same image to home_dir as fits file
    return args + argf('f') + img_loc

#Capture Image
if args.p:
    capture_cmd = cmd_pre('capture') + \
    args_form(args.p,args.i,args.t,args.l,args.g)
else: 
    capture_cmd = cmd_pre('capture') + \
    args_form(i=args.i,t=args.t,l=args.l,g=args.g)

#Connect to camera server
print(capture_cmd)
connection = Connection(host='150.203.88.24', user='mbirch',\
     connect_kwargs={"password": 'BFtrEXZ9'})
connection.run(capture_cmd)

#Display and Analyse Image
if args.a:
    files = cam.grab_from_args(args.p,args.i,args.t,args.g)
    if args.a == 'view':
        cam.group_display(files)
    elif args.a == 'hist':
        cam.group_hist(files)
    elif args.a == 'analysis':
        cam.group_analysis(files)
    else:   
        pass
else:
    pass