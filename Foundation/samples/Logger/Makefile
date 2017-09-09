#
# Makefile
#
# Makefile for Poco Logger
#

include $(POCO_BASE)/build/rules/global

objects = Logger

target         = Logger
target_version = 1
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
        SYSLIBS += -lz
endif

