rem Location of Framegrabber API
pushd C:\EDT\pdv
set routine=%1
set loc=%2
set form=%3
%routine% -%form% %loc%
popd
 