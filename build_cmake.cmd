rem POCO_STATIC=1 - for static build
rem POCO_UNBUNDLED - for no built-in version of libs
rem CMAKE_INSTALL_PREFIX=path - for install path

mkdir cmake-build
cd cmake-build

del CMakeCache.txt
cmake ../. -DCMAKE_BUILD_TYPE=Debug -G"NMake Makefiles" %1 %2 %3 %4 %5
cmake --build .
rem jom /j3
rem jom install

del CMakeCache.txt
cmake ../. -DCMAKE_BUILD_TYPE=Release -G"NMake Makefiles" %1 %2 %3 %4 %5
cmake --build .
rem jom /j3
rem jom install

cd ..
