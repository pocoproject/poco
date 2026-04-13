#
# Makefile
#
# Makefile for Poco SSH testsuite
#

include $(POCO_BASE)/build/rules/global

objects = SSHTest SSHTestSuite Driver

target         = testrunner
target_version = 1
target_libs    = PocoSSH PocoFoundation CppUnit

SYSLIBS += -lssh

include $(POCO_BASE)/build/rules/exec
