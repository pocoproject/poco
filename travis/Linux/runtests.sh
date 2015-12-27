#!/bin/bash
# set -ev
set -v
export POCO_BASE=`pwd`
export PATH=$PATH:.
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
source ./travis/ignored.sh
source ./travis/Linux/excluded.sh
build/script/runtests.sh
