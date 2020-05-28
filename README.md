# scicam

Code and Programs for controlling the Princeton Infrared Technologies 1280SCICAM for the DREAMS Telescope.

API contained in pdv_env
C-based routines can be compiled inside pdv_env with compile.py while on the camera server (mbirch@150.203.88.24).
capture.c is the main framegrabber routine used for image capture and FITS output.

Primary python module for functions is scicam.py.
serial_command.py for sending and receiving serial communication found in PIRT-ICD. 
remote_cap.py can be used to automatically capture FITS images remotely and view or analyse locally.

analysing.py contains plotting and master_frame construction routines.

testing.py contains complex routines for taking images for a variety of parameter spaces

expose.py is most generic python command with command-line interface for local image capture and fits output/file sorting (runs capture.c)

