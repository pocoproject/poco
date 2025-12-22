#
# Makefile
#
# $Id: //poco/1.7/DNSSD/Default/Makefile#1 $
#
# Makefile for DNSSD Default Implementation.
# Use Avahi on Linux, otherwise Bonjour
#

include $(POCO_BASE)/build/rules/global

ifeq ($(OSNAME),Linux)
DNSSDLibrary = Avahi
else
DNSSDLibrary = Bonjour
endif

.PHONY: projects
clean all: projects
projects:
	$(MAKE) -C $(POCO_BASE)/DNSSD/$(DNSSDLibrary) $(MAKECMDGOALS)
