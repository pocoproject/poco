# POCO_STATIC=1 - for static build
# POCO_UNBUNDLED - for no built-in version of libs
# CMAKE_INSTALL_PREFIX=path - for install path


cmake -H. -Bcmake-build -DCMAKE_BUILD_TYPE=Debug -G"NMake Makefiles JOM" %1 %2 %3 %4 %5
cmake --build cmake-build --target all -- /j3
cmake --build cmake-build --target install

rmdir cmake-build

cmake -H. -Bcmake-build -DCMAKE_BUILD_TYPE=Release -G"NMake Makefiles JOM" %1 %2 %3 %4 %5
cmake --build cmake-build --target all -- /j3
cmake --build cmake-build --target install
