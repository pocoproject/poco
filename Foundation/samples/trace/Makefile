#
# Makefile
#
# Makefile for Poco Trace sample
#


ifdef POCO_ENABLE_TRACE

include $(POCO_BASE)/build/rules/global

objects = trace

target          = trace
target_version  = 1
target_libs     = PocoTrace PocoFoundation
target_includes = $(POCO_BASE)/Trace/include

include $(POCO_BASE)/build/rules/exec

else

all:

endif
