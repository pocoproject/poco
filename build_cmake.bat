REM POCO_STATIC=1 - for static build
REM POCO_UNBUNDLED - for no built-in version of libs
REM CMAKE_INSTALL_PREFIX=path - for install path

RMDIR cmake-build
mkdir cmake-build
cd cmake-build

cmake ../. -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug  %1 %2 %3 %4 %5
nmake
nmake install

del CMakeCache.txt

cmake ../. -G"NMake Makefiles" %1 %2 %3 %4 %5
nmake
nmake install


