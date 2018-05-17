#!/bin/bash
set -ev
export POCO_BASE=$(pwd)
export PATH=$PATH:.
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
source ./appveyor/ignored.sh
source ./appveyor/Cygwin/excluded.sh
#/usr/sbin/cygserver.exe &
build/script/runtests.sh
