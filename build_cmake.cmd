rem usage
rem build_cmake ( Debug | Release )

rem -DPOCO_STATIC=1 - for static build
rem -DPOCO_UNBUNDLED - for no built-in version of libs
rem -DCMAKE_INSTALL_PREFIX=path - for install path

rmdir cmake-build

cmake -H. -Bcmake-build -DCMAKE_BUILD_TYPE=%1 -G"NMake Makefiles JOM"  -DPOCO_ENABLE_TESTS=ON -DPOCO_ENABLE_SAMPLE=ON %2 %3 %4 %5
cmake --build cmake-build --target all -- /i /j3
cmake --build cmake-build --target install -- /i
