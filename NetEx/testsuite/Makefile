#
# Makefile
#
# $Id: //poco/1.4/Net/testsuite/Makefile#2 $
#
# Makefile for Poco Net testsuite
#

include $(POCO_BASE)/build/rules/global

objects = \
	Driver \
	NetExTestSuite \
	ICMPClientTest ICMPSocketTest ICMPClientTestSuite \
	NTPClientTest NTPClientTestSuite \
	SNMPClientTest SNMPClientTestSuite

target         = testrunner
target_version = 1
target_libs    = PocoNetEx PocoNet PocoASN PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
        SYSLIBS += -lz
endif
