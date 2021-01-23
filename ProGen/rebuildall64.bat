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
rd /q /s lib64 bin64 & call buildwin 100 rebuild all both x64   samples tests msbuild > ProGen\log\100.x64.log 2>&1
rd /q /s lib64 bin64 & call buildwin 110 rebuild all both x64   samples tests msbuild > ProGen\log\110.x64.log 2>&1
rd /q /s lib64 bin64 & call buildwin 120 rebuild all both x64   samples tests msbuild > ProGen\log\120.x64.log 2>&1
rd /q /s lib64 bin64 & call buildwin 140 rebuild all both x64   samples tests msbuild > ProGen\log\140.x64.log 2>&1
rd /q /s lib64 bin64 & call buildwin 150 rebuild all both x64   samples tests msbuild > ProGen\log\150.x64.log 2>&1
popd
:exit
endlocal
