#!/usr/bin/env bash

sudo apt-get -y update && sudo apt-get -y install cmake ninja-build libssl-dev unixodbc-dev libmysqlclient-dev redis-server
cmake -H. -Bcmake-build -GNinja -DENABLE_PDF=OFF -DENABLE_DNSSD=OFF -DENABLE_TESTS=ON && cmake --build cmake-build --target all
