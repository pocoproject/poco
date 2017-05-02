#
# Makefile
#
# Makefile for Poco NetSSL testsuite
#

include $(POCO_BASE)/build/rules/global

# Note: linking order is important, do not change it.
ifeq ($(POCO_CONFIG),FreeBSD)
SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB) -lz
else
SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB) -lz -ldl
endif

objects = NetSSLTestSuite Driver \
	HTTPSClientSessionTest HTTPSClientTestSuite HTTPSServerTest HTTPSServerTestSuite \
	HTTPSStreamFactoryTest HTTPSTestServer TCPServerTest TCPServerTestSuite

target         = testrunner
target_version = 1
target_libs    = PocoNetSSL PocoNet PocoCrypto PocoUtil PocoJSON PocoXML PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
