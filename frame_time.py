
import scicam as cam 
import argparse

parser = argparse.ArgumentParser(prog='Write Frame Time', description='Change Camera Frame Time')
parser.add_argument('f', type=float, help='Frame time in milliseconds')
parser.add_argument('-v', action='store_true', help='Verbose mode')
parser.add_argument('-r', action='store_true',  help='Enter data as rate')
args = parser.parse_args()

if args.v:
    if args.r:
        cam.set_frame_time(args.f,verbose=True,rate=True)
    else:
        cam.set_frame_time(args.f,verbose=False)
else:
    if args.r:
        cam.set_frame_time(args.f,rate=True)
    else:
        cam.set_frame_time(args.f)


