
import subprocess
import argparse
import os
import shutil

parser = argparse.ArgumentParser(prog='Compile File', description='Compiles file and moves it to Framegrabber API')
parser.add_argument('n', type=str, help='Name of file to compile')
parser.add_argument('-v', action='store_true', help='Verbose mode')
args = parser.parse_args()

#Add file to makefile if not already
targ = args.n + ".exe"

include = open('includes.mk', 'a+')
check = open('includes.mk', 'r+')
if check.read().find(targ) == -1:
    include.write('\\\n    ' + targ)
    print('Appended List')
else:
    pass
include.close()
check.close()

#Run compiler
path = os.path.dirname(os.path.realpath(__file__))
file_path = path + r'\compile.bat'
if args.v:
    compiler = subprocess.Popen([file_path, targ])
else:
    compiler = subprocess.Popen([file_path, targ],stdout=subprocess.PIPE)
output, _ = compiler.communicate()

#Copy File and script to Framegrabber API
shutil.copy(path + '//' + targ, 'C:/EDT/pdv//'+targ)
shutil.copy(path + '//' + args.n +'.c', 'C:/EDT/pdv//'+args.n +'.c')
print('{} has been successfully compiled and sent to EDT API'.format(targ))
