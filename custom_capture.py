
import scicam as cam
import argparse
import subprocess
import os

parser = argparse.ArgumentParser(prog='capture Image', description='Captures image using specified routine')
parser.add_argument('p', type=str, help='Routine for image capture')
parser.add_argument('-r', type=str, help='Location to store image as raw')
parser.add_argument('-b', type=str, help='Location to store image as bitmap')
parser.add_argument('-f', type=str, help='Location to store image as FITS')
parser.add_argument('-i', type=float, help='Integration Time')
parser.add_argument('-t', type=float, help='Frame Time')
parser.add_argument('-l', type=int, help='Number of loops')
parser.add_argument('-g', type=str, help='Naming Tag',default = '')
parser.add_argument('-v', action='store_true', help='Verbose mode')
args = parser.parse_args()

home_loc = '//merger.anu.edu.au/mbirch/images'


def capture(routine, loc='no_loc', form = 'f'):
    #Create path string for batch file
    if loc !='no_loc':
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

#Option to set integration time w/ or w/out verbose
if args.i:
    if args.v:
        int_t = cam.set_int_time(args.i,verbose=True)
    else:
        int_t = cam.set_int_time(args.i)
else:
    if args.v:
        int_t = cam.read_int_time(verbose=True)
    else:
        int_t = cam.read_int_time()

#Option to set frame time w/ or w/out verbose
if args.t:
    if args.v:
        frame_t = cam.set_frame_time(args.t,verbose=True)
    else:
        frame_t = cam.set_frame_time(args.t)
else:
    if args.v:
        frame_t = cam.read_frame_time(verbose=True)
    else:
        frame_t = cam.read_frame_time()


#Capture image w/ or w/out defined location
if args.l:
    for i in range(args.l):
        if args.r:
            capture_resp, _ = capture(args.p,args.r,'f')
            cam.file_sorting(args.r,int_t,frame_t,'.raw',args.p,tag=args.g)
        elif args.b:
            capture_resp, _ = capture(args.p,args.b,'b')
            cam.file_sorting(args.b,int_t,frame_t,routine=args.p,tag=args.g)
        elif args.f:
            capture_resp, _ = capture(args.p,args.f,'f')
            cam.file_sorting(args.f,int_t,frame_t,'.fits',routine=args.p,tag=args.g)
        else:
            capture_resp = capture(args.p,home_loc,'f')
            cam.file_sorting(home_loc,int_t,frame_t,routine=args.p,tag=args.g)
else:
    if args.r:
        capture_resp, _ = capture(args.p,args.r,'f')
        cam.file_sorting(args.r,int_t,frame_t,'.raw',args.p,tag=args.g)
    elif args.b:
        capture_resp, _ = capture(args.p,args.b,'b')
        cam.file_sorting(args.b,int_t,frame_t,routine=args.p,tag=args.g)
    elif args.f:
        capture_resp, _ = capture(args.p,args.f,'f')
        cam.file_sorting(args.f,int_t,frame_t,'.fits',routine=args.p,tag=args.g)
    else:
        capture_resp = capture(args.p,home_loc,'f')
        cam.file_sorting(home_loc,int_t,frame_t,routine=args.p,tag=args.g)
