#!/usr/bin/env bash

sudo apt-get -y update && sudo apt-get -y install cmake ninja-build libssl-dev unixodbc-dev libmysqlclient-dev redis-server libxml2-dev
cmake -H. -Bcmake-build -GNinja -DENABLE_PDF=OFF -DENABLE_DNSSD=OFF -DENABLE_TESTS=ON -DENABLE_XSD_VALIDATOR=ON && cmake --build cmake-build --target all --parallel 4
