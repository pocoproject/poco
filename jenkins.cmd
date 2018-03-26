@echo off

echo Writing components list ...

echo Foundation> components
echo XML>> components
echo JSON>> components
echo Util>> components
echo Net>> components
echo Crypto>> components
echo NetSSL_OpenSSL>> components
echo Zip>> components

type components

echo Initializing Visual Studio environment
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"

echo Building poco
buildwin 140 build all both Win32 nosamples vcexpress
