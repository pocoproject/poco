#
# Makefile
#
# $Id: //poco/Main/Crypto/testsuite/Makefile#2 $
#
# Makefile for Poco Crypto testsuite
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -lssl -lcrypto

objects = CryptoTestSuite Driver \
	CryptoTest RSATest

target         = testrunner
target_version = 1
target_libs    = PocoCrypto PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
