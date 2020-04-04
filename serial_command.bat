rem Location of Framegrabber API
pushd C:\EDT\pdv
set cmd=%1
serial_cmd -x %cmd%
popd