#
# Makefile
#
# $Id: //poco/1.4/PageCompiler/File2Page/Makefile#1 $
#
# Makefile for Poco C++ Server Page Compiler File2Page Utility
#

include $(POCO_BASE)/build/rules/global

objects = File2Page

ifdef POCO_UNBUNDLED
	SYSLIBS += -lz -lpcre -lexpat
else
	objects += $(zlib_objects) $(pcre_objects) $(pcre_utf8_objects) $(expat_objects)
endif

target         = f2cpsp
target_version = 1
target_libs    = PocoUtil PocoJSON PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
