set -ev
osname=`uname`
osarch=`uname -m`
export POCO_BASE=`pwd`
export EXCLUDE_TESTS="Data/MySQL Data/ODBC MongoDB PDF"
export PATH=$PATH:.
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.:$POCO_BASE/lib/$osname/$osarch
build/script/runtests.sh
