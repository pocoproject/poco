#
# Makefile
#
# Makefile for pocossh sample
#

include $(POCO_BASE)/build/rules/global

objects = pocossh

target         = pocossh
target_version = 1
target_libs    = PocoSSH PocoFoundation

ifeq ($(OSNAME),Darwin)
SYSFLAGS += $(shell pkg-config --cflags libssh 2>/dev/null)
SYSLIBS  += $(shell pkg-config --libs libssh 2>/dev/null || echo -lssh)
else
SYSLIBS  += -lssh
endif

include $(POCO_BASE)/build/rules/exec
