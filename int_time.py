
import scicam as cam 
import argparse

parser = argparse.ArgumentParser(prog='Write Integration Time', description='Change Camera Integration Time')
parser.add_argument('i', type=float, help='Integration time in milliseconds')
parser.add_argument('-v', action='store_true', help='Verbose mode')
args = parser.parse_args()

if args.v:
    cam.set_int_time(args.i,verbose=True)
else:
    cam.set_int_time(args.i)
    
