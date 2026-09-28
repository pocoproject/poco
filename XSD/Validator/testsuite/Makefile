#
# Makefile
#
# Makefile for Poco XSDValidator testsuite
#

include $(POCO_BASE)/build/rules/global

objects = ValidatorTestSuite Driver \
	ValidatorTest

target         = testrunner
target_version = 1
target_libs    = PocoXSDValidator PocoFoundation CppUnit

# libxml2 is a private dependency of PocoXSDValidator; static builds need it here too.
SYSLIBS  += $(shell pkg-config --libs libxml-2.0 2>/dev/null || xml2-config --libs 2>/dev/null || echo -lxml2)

include $(POCO_BASE)/build/rules/exec
