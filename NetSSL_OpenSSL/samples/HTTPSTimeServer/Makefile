#
# Makefile
#
# $Id: //poco/Main/Net/samples/HTTPTimeServer/Makefile#1 $
#
# Makefile for Poco HTTPSTimeServer
#

include $(POCO_BASE)/build/rules/global

# Note: linking order is important, do not change it.
SYSLIBS += -lssl -lcrypto -lz -ldl

objects = HTTPSTimeServer

target         = HTTPSTimeServer
target_version = 1
target_libs    = PocoNetSSL PocoCrypto PocoNet PocoUtil PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
