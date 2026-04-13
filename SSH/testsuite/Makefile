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

ifeq ($(OSNAME),Darwin)
SYSFLAGS += $(shell pkg-config --cflags libssh 2>/dev/null)
SYSLIBS  += $(shell pkg-config --libs libssh 2>/dev/null || echo -lssh)
else
SYSLIBS  += -lssh
endif

include $(POCO_BASE)/build/rules/exec
