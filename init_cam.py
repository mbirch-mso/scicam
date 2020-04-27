 

import os
import subprocess
import argparse
from scicam import command


parser = argparse.ArgumentParser(prog='Initalise camera', description='Runs the init_cam script on the EDT\
     Framegrabber to Initialise the PIRT 1280SCICAM')
parser.add_argument('-b',  action='store_true', help='Set camera to base mode (1 tap)')
parser.add_argument('-f',  action='store_true', help='Set camera to full mode (4 taps)')
args = parser.parse_args()


#Run initcam batch file here
script = os.path.dirname(os.path.realpath(__file__)) + '\pirt_init_cam.bat'
if args.b:
    init = subprocess.Popen([script,'base'])
    _, _ = init.communicate()
    print('PIRT 1280SCICAM initialised to base 1 tap operation')
elif args.f:
    init = subprocess.Popen([script,'full'])
    _, _ = init.communicate()
    #Set total number of FPA channels to 4
    command('1026', data = '02')
    #Set total number of OUTPUT mode channels to 4
    command('10ca', data = '03')
    print('PIRT 1280SCICAM initialised to full 4 tap operation')
else:
    init = subprocess.Popen([script,'base'])
    _, _ = init.communicate()
    print('PIRT 1280SCICAM initialised to base 1 tap operation')
command('2006',data = '01')
print('Metadata enabled')
