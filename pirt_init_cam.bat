
@echo off
pushd C:\EDT\pdv
set  mode=%1
set start=camera_config/00pi_scicam_4x14f_
set end=.cfg
call set newvar = %start%%mode%%end%
.\initcam -u 0 -c 0 -f %start%%mode%%end%
popd


