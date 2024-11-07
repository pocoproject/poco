#!/bin/bash

# BUILD_SHARED_LIBS=OFF - for static build
# POCO_UNBUNDLED - for no built-in version of libs
# CMAKE_INSTALL_PREFIX=path - for install path

rm -rf cmake-build

cmake -H. -Bcmake-build -DCMAKE_BUILD_TYPE=Debug  $1 $2 $3 $4 $5
cmake --build cmake-build --target all -- -j3
cmake --build cmake-build --target install

rm -rf cmake-build

cmake -H. -Bcmake-build -DCMAKE_BUILD_TYPE=Release $1 $2 $3 $4 $5
cmake --build cmake-build --target all -- -j3
cmake --build cmake-build --target install
