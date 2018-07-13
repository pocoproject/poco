@echo off

set POCO_BASE=%cd%

echo Initializing Visual Studio environment
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"

echo Building poco Release

mkdir cmake_build_Release
cd cmake_build_Release

cmake .. ^
	-DENABLE_PDF=OFF ^
	-DENABLE_MONGODB=OFF ^
	-DENABLE_DATA=OFF ^
	-DENABLE_PAGECOMPILER=OFF ^
	-DENABLE_CPPPARSER=OFF ^
	-DENABLE_APACHECONNECTOR=OFF ^
	-DENABLE_SEVENZIP=OFF ^
	-DENABLE_REDIS=OFF ^
	-DENABLE_POCODOC=OFF ^
    -DPOCO_STATIC=1 ^
    -DCMAKE_INSTALL_PREFIX=%POCO_BASE%/cmake_install_Release
cmake --build . --target install
cd ..

echo Building poco Debug
mkdir cmake_build_Debug
cd cmake_build_Debug

cmake .. ^
	-DENABLE_PDF=OFF ^
	-DENABLE_MONGODB=OFF ^
	-DENABLE_DATA=OFF ^
	-DENABLE_PAGECOMPILER=OFF ^
	-DENABLE_CPPPARSER=OFF ^
	-DENABLE_APACHECONNECTOR=OFF ^
	-DENABLE_SEVENZIP=OFF ^
	-DENABLE_REDIS=OFF ^
	-DENABLE_POCODOC=OFF ^
    -DPOCO_STATIC=1 ^
    -DCMAKE_INSTALL_PREFIX=%POCO_BASE%/cmake_install_Debug
cmake --build . --target install
cd ..
