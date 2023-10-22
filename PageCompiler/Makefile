#
# Makefile
#
# Makefile for Poco C++ Server Page Compiler
#

include $(POCO_BASE)/build/rules/global

objects = Page PageReader \
	CodeWriter ApacheCodeWriter OSPCodeWriter \
	PageCompiler

ifdef POCO_UNBUNDLED_ZLIB
	SYSLIBS += -lz
endif

ifdef POCO_UNBUNDLED_PCRE
	SYSLIBS += -lpcre
endif

ifdef POCO_UNBUNDLED_EXPAT
	SYSLIBS += -lexpat
endif

target         = cpspc
target_version = 1
target_libs    = PocoNet PocoUtil PocoXML PocoJSON PocoFoundation

include $(POCO_BASE)/build/rules/exec
