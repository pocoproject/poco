#
# Makefile
#
# $Id: //poco/1.4/Crypto/testsuite/Makefile#1 $
#
# Makefile for Poco Crypto testsuite
#

include $(POCO_BASE)/build/rules/global

# Note: linking order is important, do not change it.
SYSLIBS += -lssl -lcrypto -lz -ldl

objects = CryptoTestSuite Driver \
	CryptoTest RSATest

target         = testrunner
target_version = 1
target_libs    = PocoCrypto PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
