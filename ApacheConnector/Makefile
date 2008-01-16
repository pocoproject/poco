#
# Makefile
#
# $Id: //poco/Main/ApacheConnector/Makefile#7 $
#
# Makefile for ApacheConnector
#

include $(POCO_BASE)/build/rules/global

SYSFLAGS += -I/usr/include/apache2 -I/usr/include/apr-0

objects = ApacheRequestHandlerFactory  \
	ApacheServerRequest ApacheServerResponse \
	ApacheStream ApacheConnector ApacheChannel \
	ApacheApplication ApacheChannel

target         = mod_poco
target_version = 1
target_libs    = PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/dylib

