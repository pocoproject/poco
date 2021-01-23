#!/bin/bash
#
# set -ev
#
set -v
export POCO_BASE=`pwd`
export PATH=$PATH:.
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
source ./appveyor/ignored.sh
source ./appveyor/Cygwin/excluded.sh
build/script/runtests.sh
