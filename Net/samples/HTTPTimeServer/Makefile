#
# Makefile
#
# Makefile for Poco HTTPTimeServer
#

include $(POCO_BASE)/build/rules/global

objects = HTTPTimeServer

target         = HTTPTimeServer
target_version = 1
target_libs    = PocoUtil PocoJSON PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED_ZLIB
SYSLIBS += -lz
endif

ifdef POCO_UNBUNDLED_PCRE
SYSLIBS += -lpcre
endif

ifdef POCO_UNBUNDLED_EXPAT
SYSLIBS += -lexpat
endif
