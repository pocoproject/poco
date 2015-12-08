rem usage
rem build_cmake ( Debug | Release )

rem -DPOCO_STATIC=1 - for static build
rem -DPOCO_UNBUNDLED - for no built-in version of libs
rem -DCMAKE_INSTALL_PREFIX=path - for install path

mkdir cmake-build
cd cmake-build

del CMakeCache.txt
cmake ../. -DCMAKE_BUILD_TYPE=%1 -G"NMake Makefiles JOM"  -DENABLE_TESTS=ON -DENABLE_SAMPLE=ON %2 %3 %4 %5
jom /i /j3
jom /i install

cd ..
