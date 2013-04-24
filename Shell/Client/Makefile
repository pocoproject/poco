#
# Makefile
#
# $Id: //poco/1.4/Shell/Client/Makefile#2 $
#
# Makefile for POCO Shell Client
#

include $(POCO_BASE)/build/rules/global

# check for editline (libedit) and use it if it's available
ifeq ($(shell test -f /usr/include/histedit.h && echo 1),1)
POCOSH_HAVE_EDITLINE = 1
$(info ** Building pocosh with editline (libedit) support)
endif

ifdef POCOSH_HAVE_EDITLINE
CXXFLAGS += -DPOCOSH_HAVE_EDITLINE
SYSLIBS += -ledit
endif

objects = Client

target         = pocosh
target_version = 1
target_libs    = PocoUtil PocoXML PocoNet PocoFoundation

include $(POCO_BASE)/build/rules/exec
