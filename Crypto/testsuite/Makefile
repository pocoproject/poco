#
# Makefile
#
# Makefile for Poco Crypto testsuite
#

include $(POCO_BASE)/build/rules/global

# Note: linking order is important, do not change it.
ifeq ($(POCO_CONFIG),FreeBSD)
SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB) -lz
else
ifeq ($(POCO_CONFIG),QNX)
SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB) -lz
else
SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB) -lz -ldl
endif
endif

objects = CryptoTestSuite Driver \
	CryptoTest DigestEngineTest ECTest \
	EVPTest RSATest PKCS12ContainerTest \
	EnvelopeTest

target         = testrunner
target_version = 1
target_libs    = PocoCrypto PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
