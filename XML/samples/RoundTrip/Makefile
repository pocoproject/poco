#
# Makefile
#
# Makefile for Poco SAXParser
#

include $(POCO_BASE)/build/rules/global

objects = RoundTrip

target         = RoundTrip
target_version = 1
target_libs    = PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
        SYSLIBS += -lexpat
endif
