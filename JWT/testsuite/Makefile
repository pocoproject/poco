#
# Makefile
#
# Makefile for Poco JWT testsuite
#

include $(POCO_BASE)/build/rules/global

objects = SerializerTest TokenTest SignerTest JWTTestSuite Driver

target         = testrunner
target_version = 1
target_libs    = PocoJWT PocoJSON PocoCrypto PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
        SYSLIBS += -lpcre
endif
