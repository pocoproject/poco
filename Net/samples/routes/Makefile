#
# Makefile
#
# $Id: //poco/Main/template/sample.make#4 $
#
# Makefile for Poco routes
#

include $(POCO_BASE)/build/rules/global

ifeq ($(OSNAME),Linux)
SYSLIBS += -lmnl
endif

objects = routes

target         = routes
target_version = 1
target_libs    = PocoUtil PocoNet PocoFoundation

include $(POCO_BASE)/build/rules/exec
