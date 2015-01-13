#
# Makefile
#
# $Id: //poco/1.4/NetEx/samples/SNMP/Makefile#1 $
#
# Makefile for Poco SNMP
#

include $(POCO_BASE)/build/rules/global

objects = SNMP

target         = SNMP
target_version = 1
target_libs    = PocoUtil PocoJSON PocoNetEx PocoNet PocoASN PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
        SYSLIBS += -lz -lpcre -lexpat
endif
