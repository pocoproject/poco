#
# Makefile
#
# $Id: //poco/1.7/DNSSD/samples/HTTPTimeServer/Makefile#1 $
#
# Makefile for HTTPTimeServer sample
#

include $(POCO_BASE)/build/rules/global

ifeq ($(OSNAME),Linux)
DNSSDLibrary = PocoDNSSDAvahi
else
DNSSDLibrary = PocoDNSSDBonjour
endif

objects = HTTPTimeServer

target         = HTTPTimeServer
target_version = 1
target_libs    = $(DNSSDLibrary) PocoDNSSD PocoNet PocoUtil PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
