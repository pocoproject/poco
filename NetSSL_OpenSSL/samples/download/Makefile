#
# Makefile
#
# $Id: //poco/Main/template/sample.make#4 $
#
# Makefile for Poco download
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -lssl -lcrypto -lz -ldl

objects = download

target         = download
target_version = 1
target_libs    = PocoNetSSL PocoCrypto PocoNet PocoUtil PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
