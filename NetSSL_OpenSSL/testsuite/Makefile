#
# Makefile
#
# Makefile for Poco NetSSL testsuite
#

include $(POCO_BASE)/build/rules/global

# Note: linking order is important, do not change it.
ifeq ($(POCO_CONFIG),FreeBSD)
SYSLIBS += -lssl -lcrypto -lz
else
ifeq ($(POCO_CONFIG),QNX)
SYSLIBS += -lssl -lcrypto -lz
else
ifeq ($(findstring AIX, $(POCO_CONFIG)), AIX)
SYSLIBS += -lssl_a -lcrypto_a -lz -ldl
else
SYSLIBS += -lssl -lcrypto -lz -ldl
endif
endif
endif

objects = NetSSLTestSuite Driver \
	HTTPSClientSessionTest HTTPSClientTestSuite HTTPSServerTest HTTPSServerTestSuite \
	HTTPSStreamFactoryTest HTTPSTestServer TCPServerTest TCPServerTestSuite \
	WebSocketTest WebSocketTestSuite FTPSClientSessionTest FTPSClientTestSuite \
	DialogServer

target         = testrunner
target_version = 1
target_libs    = PocoNetSSL PocoNet PocoCrypto PocoUtil PocoJSON PocoXML PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
