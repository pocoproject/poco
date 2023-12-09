#
# Makefile
#
# Makefile for Poco Crypto testsuite
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
ifeq ($(POCO_CONFIG),Darwin)
SYSLIBS += -lssl -lcrypto -lz
else
SYSLIBS += -lssl -lcrypto -lz -ldl
endif # Darwin
endif # AIX
endif # QNX
endif # FreeBSD

objects = CryptoTestSuite Driver \
	CryptoTest DigestEngineTest ECTest \
	EVPTest RSATest PKCS12ContainerTest \
	EnvelopeTest

target         = testrunner
target_version = 1
target_libs    = PocoCrypto PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
