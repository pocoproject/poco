@echo off
setlocal EnableExtensions EnableDelayedExpansion
if "%POCO_BASE%"=="" (
	echo POCO_BASE not set
	goto :exit
)
pushd %POCO_BASE%
mkdir ProGen\log

call ProGen\progen x64 >ProGen\log\progen64.log 2>&1
rd /q /s lib64 bin64 & call buildwin  90 rebuild all both x64   samples tests devenv  > ProGen\log\90.x64.log 2>&1
rd /q /s lib64 bin64 & call buildwin 160 rebuild all both x64   samples tests msbuild > ProGen\log\140.x64.log 2>&1
rd /q /s lib64 bin64 & call buildwin 170 rebuild all both x64   samples tests msbuild > ProGen\log\150.x64.log 2>&1
popd
:exit
endlocal
