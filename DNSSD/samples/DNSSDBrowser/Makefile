#
# Makefile
#
# $Id: //poco/1.7/DNSSD/samples/DNSSDBrowser/Makefile#1 $
#
# Makefile for DNSSDBrowser sample
#

include $(POCO_BASE)/build/rules/global

ifeq ($(OSNAME),Linux)
DNSSDLibrary = PocoDNSSDAvahi
else
DNSSDLibrary = PocoDNSSDBonjour
endif

objects = DNSSDBrowser

target         = DNSSDBrowser
target_version = 1
target_libs    = $(DNSSDLibrary) PocoDNSSD PocoNet PocoUtil PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
