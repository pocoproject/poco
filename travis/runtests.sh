set -ev
export POCO_BASE=`pwd`
export EXCLUDE_TESTS="Data/MySQL Data/ODBC MongoDB PDF"
export PATH=$PATH:.
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
build/script/runtests.sh
