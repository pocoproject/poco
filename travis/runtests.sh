#!/bin/bash
set -ev
export POCO_BASE=`pwd`
source ./travis/ignored.sh
source ./travis/excluded.sh
export PATH=$PATH:.
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
build/script/runtests.sh
