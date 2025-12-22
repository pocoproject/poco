#
# Makefile
#
# $Id: //poco/1.7/DNSSD/Avahi/Makefile#1 $
#
# Makefile for Poco DNSSD Avahi
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -lavahi-common -lavahi-client

objects = \
	AvahiResponderImpl AvahiBrowserImpl

target         = PocoDNSSDAvahi
target_version = 1
target_libs    = PocoNet PocoDNSSD PocoFoundation

include $(POCO_BASE)/build/rules/lib

