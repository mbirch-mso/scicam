
set file_name=%1

:someRoutine
setlocal
%@Try%
  pushd C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build
%@EndTry%
:@Catch
  pushd C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build
:@EndCatch

vcvars32

popd

make -%file_name%.exe


