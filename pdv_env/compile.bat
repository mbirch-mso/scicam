
@echo off
set file_name=%1


:someRoutine
setlocal
%@Try%
  call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
%@EndTry%
:@Catch
  call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars32.bat"
:@EndCatch
@echo on

make %file_name%


