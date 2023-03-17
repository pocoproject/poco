@echo off
setlocal EnableDelayedExpansion EnableExtensions
if "%POCO_BASE%"=="" (
	echo POCO_BASE not set
	goto :exit
)
pushd %POCO_BASE%
mkdir ProGen\log

call ProGen\progen.bat Win32 >ProGen\log\progen32.log 2>&1
rd /q /s lib bin     & call buildwin  90 rebuild all both Win32 samples tests devenv  > ProGen\log\90.x32.log 2>&1
rd /q /s lib bin     & call buildwin 100 rebuild all both Win32 samples tests msbuild > ProGen\log\100.x32.log 2>&1
rd /q /s lib bin     & call buildwin 110 rebuild all both Win32 samples tests msbuild > ProGen\log\110.x32.log 2>&1
rd /q /s lib bin     & call buildwin 120 rebuild all both Win32 samples tests msbuild > ProGen\log\120.x32.log 2>&1
rd /q /s lib bin     & call buildwin 140 rebuild all both Win32 samples tests msbuild > ProGen\log\140.x32.log 2>&1
rd /q /s lib bin     & call buildwin 150 rebuild all both Win32 samples tests msbuild > ProGen\log\150.x32.log 2>&1
popd
:exit
endlocal
