#!/bin/sh

set -ev
osname=$(uname)
osarch=$(uname -m)
POCO_BASE=$(pwd)
export POCO_BASE
export PATH="$PATH":.
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH":.:"$POCO_BASE"/lib/"$osname"/"$osarch"
export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH":.:"$POCO_BASE"/lib/"$osname"/"$osarch"
if [ "TSAN" = "$1" ]
  then
    export TSAN_OPTIONS="suppressions=$POCO_BASE/tsan.suppress,second_deadlock_stack=1"
fi
build/script/runtests.sh
