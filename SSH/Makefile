#
# Makefile
#
# Makefile for Poco SSH Library
#

include $(POCO_BASE)/build/rules/global

ifeq ($(OSNAME),Darwin)
SYSFLAGS += $(shell pkg-config --cflags libssh 2>/dev/null)
SYSLIBS  += $(shell pkg-config --libs libssh 2>/dev/null || echo -lssh)
else
SYSLIBS  += -lssh
endif

objects = SSHChannelStream SSHClient SSHException SSHHostKeyManager SSHServer SSHSession

target         = PocoSSH
target_version = 1
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
