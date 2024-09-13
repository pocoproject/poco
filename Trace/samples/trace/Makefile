#
# Makefile
#
# Makefile for Poco Trace sample
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I $(POCO_BASE)/Trace/include/Poco/Trace
LIBRARY += -L$(POCO_BASE)/lib/$(OSNAME)/$(OSARCH)/lib

objects = trace

target          = trace
target_version  = 1
target_libs     = PocoTrace PocoFoundation
target_includes = $(POCO_BASE)/Trace/include

include $(POCO_BASE)/build/rules/exec
