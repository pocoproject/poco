#
# Makefile
#
# Makefile for Poco PrettyPrint
#

include $(POCO_BASE)/build/rules/global

objects = PrettyPrint

target         = PrettyPrint
target_version = 1
target_libs    = PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
        SYSLIBS += -lexpat
endif

