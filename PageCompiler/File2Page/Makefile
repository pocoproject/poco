#
# Makefile
#
# Makefile for Poco C++ Server Page Compiler File2Page Utility
#

include $(POCO_BASE)/build/rules/global

objects = File2Page

ifdef POCO_UNBUNDLED_ZLIB
	SYSLIBS += -lz
endif

ifdef POCO_UNBUNDLED_PCRE
	SYSLIBS += -lpcre
endif

ifdef POCO_UNBUNDLED_EXPAT
	SYSLIBS += -lexpat
endif

target         = f2cpsp
target_version = 1
target_libs    = PocoUtil PocoXML PocoJSON PocoFoundation

include $(POCO_BASE)/build/rules/exec
