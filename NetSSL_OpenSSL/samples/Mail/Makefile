#
# Makefile
#
# $Id: //poco/1.4/Net/samples/Mail/Makefile#1 $
#
# Makefile for Poco Mail
#

include $(POCO_BASE)/build/rules/global

# Note: linking order is important, do not change it.
SYSLIBS += -lssl -lcrypto -lz -ldl

objects = Mail

target         = Mail
target_version = 1
target_libs    = PocoNetSSL PocoNet PocoCrypto PocoUtil PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
