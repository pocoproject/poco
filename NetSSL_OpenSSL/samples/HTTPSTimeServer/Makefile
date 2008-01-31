#
# Makefile
#
# $Id: //poco/Main/Net/samples/HTTPTimeServer/Makefile#1 $
#
# Makefile for Poco HTTPSTimeServer
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -lssl -lcrypto

objects = HTTPSTimeServer

target         = HTTPSTimeServer
target_version = 1
target_libs    = PocoNetSSL PocoNet PocoUtil PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
