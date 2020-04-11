 

import os
import subprocess
from scicam import command


#Run initcam batch file here
init = subprocess.Popen(os.path.dirname(os.path.realpath(__file__)) + '\pirt_init_cam.bat')
_, _ = init.communicate()
#Set total number of FPA channels to 4
command('1026', data = '02')
#Set total number of OUTPUT mode channels to 4
command('10ca', data = '03')
print('PIRT 1280SCICAM initialised to full 4 channel operation')
