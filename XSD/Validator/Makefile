#
# Makefile
#
# Makefile for Poco XSDValidator
#

include $(POCO_BASE)/build/rules/global

objects = Validator

target         = PocoXSDValidator
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

# libxml2 is a system dependency; pkg-config first, xml2-config second.
SYSFLAGS += $(shell pkg-config --cflags libxml-2.0 2>/dev/null || xml2-config --cflags 2>/dev/null || echo -I/usr/include/libxml2)
SYSLIBS  += $(shell pkg-config --libs libxml-2.0 2>/dev/null || xml2-config --libs 2>/dev/null || echo -lxml2)

include $(POCO_BASE)/build/rules/lib
