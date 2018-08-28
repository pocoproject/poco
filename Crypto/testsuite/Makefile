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
SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB) -lz -ldl
endif

objects = CryptoTestSuite Driver \
	CryptoTest DigestEngineTest ECTest \
	EVPTest RSATest PKCS12ContainerTest

target         = testrunner
target_version = 1
target_libs    = PocoCrypto PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
