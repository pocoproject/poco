#!/bin/bash

# POCO_STATIC=1 - for static build
# POCO_UNBUNDLED - for no built-in version of libs
# CMAKE_INSTALL_PREFIX=path - for install path

rm -rf cmake-build
mkdir cmake-build
cd cmake-build

cmake ../. -DCMAKE_INSTALL_PREFIX=/opt/devel -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON -G "Sublime Text 2 - Unix Makefiles"  $1 $2 $3 $4 $5
make -j8
sudo make install

#rm -rf CMakeCache.txt

#cmake ../. -DCMAKE_INSTALL_PREFIX=/opt/devel -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTS=ON $1 $2 $3 $4 $5
#make -j8
#sudo make install


cd ..
