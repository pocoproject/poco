set -ev
export POCO_BASE=`pwd`
export CPPUNIT_IGNORE="\
  N7CppUnit10TestCallerI8PathTestEE.testExpand \
  N7CppUnit10TestCallerI13RawSocketTestEE.testEchoIPv4 \
  N7CppUnit10TestCallerI13RawSocketTestEE.testSendToReceiveFromIPv4 \
  N7CppUnit10TestCallerI14ICMPClientTestEE.testPing"
export EXCLUDE_TESTS="Data/MySQL Data/ODBC MongoDB PDF"
export PATH=$PATH:.
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
build/script/runtests.sh
