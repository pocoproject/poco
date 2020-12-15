echo off
rem  Usage:
rem  ------
rem  bpp [vsvers] [release]       // bpp 150 1.10.1 
rem  ------
setlocal
if "%1"=="" goto usage
if "%2"=="" goto usage
del *.nupkg
del *.snupkg
pushd ..\..\..\
buildwin %1 rebuild all both Win32 samples tests msbuild noenv
buildwin %1 rebuild all both x64   samples tests msbuild noenv
popd
pack %1 %2
nuget push *.nupkg -source Bintray -NonInteractive -SkipDuplicate -Timeout 3000
goto exit
:usage
echo Usage:
echo ------
echo bpp [vsvers] [release]      // bpp 150 1.10.1 
echo ------
:exit
endlocal
echo on