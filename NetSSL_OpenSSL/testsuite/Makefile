#
# Makefile
#
# $Id: //poco/Main/template/TestSuite.make#3 $
#
# Makefile for Poco NetSSL testsuite
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -lssl -lcrypto

objects = NetSSLTestSuite Driver \
	HTTPSClientSessionTest HTTPSClientTestSuite HTTPSServerTest HTTPSServerTestSuite \
	HTTPSStreamFactoryTest HTTPSTestServer TCPServerTest TCPServerTestSuite

target         = testrunner
target_version = 1
target_libs    = PocoNetSSL PocoNet PocoUtil PocoXML PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
