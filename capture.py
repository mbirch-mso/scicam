
import scicam as cam
import argparse

parser = argparse.ArgumentParser(prog='capture Image', description='Captures image using specified routine')
parser.add_argument('-i', type=float, help='Integration Time')
parser.add_argument('-g', type=str, help='Naming Tag',default = '')
parser.add_argument('-l', type=int, help='Number of integrations (NDIT)')
args = parser.parse_args()

img_dir = '//merger.anu.edu.au/mbirch/images'
if args.i:
    int_t = cam.set_int_time(args.i)
    frame_t = cam.set_frame_time((args.i+20))
else:
    int_t = cam.read_int_time()
    frame_t = cam.read_frame_time()
if args.l:
    for i in range(args.l):
        cam.img_cap('capture',img_dir)
        cam.file_sorting(img_dir,int_t,frame_t,tag=args.g)
else:
    cam.img_cap('capture',img_dir)
    cam.file_sorting(img_dir,int_t,frame_t,tag=args.g)

