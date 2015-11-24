rem POCO_STATIC=1 - for static build
rem POCO_UNBUNDLED - for no built-in version of libs
rem CMAKE_INSTALL_PREFIX=path - for install path

mkdir cmake-build
cd cmake-build

del CMakeCache.txt
cmake ../. -DCMAKE_BUILD_TYPE=Debug -G"NMake Makefiles" -DENABLE_TESTS=ON -DENABLE_SAMPLE=ON %1 %2 %3 %4 %5
jom /i /j3
jom /i install

del CMakeCache.txt
cmake ../. -DCMAKE_BUILD_TYPE=Release -G"NMake Makefiles"  -DENABLE_TESTS=ON -DENABLE_SAMPLE=ON %1 %2 %3 %4 %5
jom /i /j3
jom /i install

cd ..
