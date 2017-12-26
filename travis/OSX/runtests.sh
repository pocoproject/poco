#!/bin/bash
set -ev
export POCO_BASE=`pwd`
export PATH=$PATH:.
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
source ./travis/OSX/ignored.sh
source ./travis/OSX/excluded.sh
build/script/runtests.sh

