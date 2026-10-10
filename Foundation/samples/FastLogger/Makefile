#
# Makefile
#
# Makefile for Poco FastLogger
#

include $(POCO_BASE)/build/rules/global

objects = FastLogger

target         = FastLogger
target_version = 1
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
        SYSLIBS += -lz
endif

