#!/usr/bin/env bash

sudo apt-get -y update && sudo apt-get -y install cmake ninja-build libssl-dev unixodbc-dev libmysqlclient-dev redis-server libxml2-dev g++-11
# GCC 11 is the oldest GCC built in CI (README: GCC 10 or newer); the runner's default is GCC 13.
CC=gcc-11 CXX=g++-11 cmake -H. -Bcmake-build -GNinja -DENABLE_PDF=OFF -DENABLE_DNSSD=OFF -DENABLE_TESTS=ON -DENABLE_XSD_VALIDATOR=ON -DENABLE_AI=ON && cmake --build cmake-build --target all --parallel 4
