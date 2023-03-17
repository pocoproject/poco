#
# Makefile
#
# Makefile for Poco HTTPTimeServer
#

include $(POCO_BASE)/build/rules/global
include $(POCO_BASE)/PageCompiler/PageCompiler.make

# Cygwin Poco*.dll should be on PATH
ifeq ($(OSNAME), CYGWIN)
    PATH       :=$(LIBPATH):$(PATH)
endif

objects        = HTTPTimeServerApp TimeHandler

target         = HTTPTimeServer
target_version = 1
target_libs    = PocoUtil PocoJSON PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
        SYSLIBS += -lz -lpcre -lexpat
endif

# Rule for runnning PageCompiler
src/%.cpp: src/%.cpsp
	@echo "** Compiling Page" $<
	$(SET_LD_LIBRARY_PATH) $(PAGE_COMPILER) $<
