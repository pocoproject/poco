#
# Makefile
#
# $Id: //poco/1.2/dist/Makefile#1 $
#
# Makefile for Poco
#

sinclude config.make

ifndef POCO_BASE
$(warning WARNING: POCO_BASE is not defined. Assuming current directory.)
export POCO_BASE=$(shell pwd)
endif

ifndef POCO_PREFIX
export POCO_PREFIX=/usr/local
endif

INSTALLDIR = $(DESTDIR)$(POCO_PREFIX)
COMPONENTS = Foundation XML Util Net

.PHONY: all libs cppunit tests samples install

all: libs tests samples

libs:
	$(MAKE) -C $(POCO_BASE)/Foundation 
	$(MAKE) -C $(POCO_BASE)/XML 
	$(MAKE) -C $(POCO_BASE)/Util 
	$(MAKE) -C $(POCO_BASE)/Net 

cppunit:
	$(MAKE) -C $(POCO_BASE)/CppUnit 

tests: cppunit libs
	$(MAKE) -C $(POCO_BASE)/Foundation/testsuite
	$(MAKE) -C $(POCO_BASE)/XML/testsuite 
	$(MAKE) -C $(POCO_BASE)/Util/testsuite 
	$(MAKE) -C $(POCO_BASE)/Net/testsuite 

samples: libs
	$(MAKE) -C $(POCO_BASE)/Foundation/samples
	$(MAKE) -C $(POCO_BASE)/XML/samples 
	$(MAKE) -C $(POCO_BASE)/Util/samples 
	$(MAKE) -C $(POCO_BASE)/Net/samples 

install: libs
	mkdir -p $(INSTALLDIR)/include/Poco
	mkdir -p $(INSTALLDIR)/lib
	for comp in $(COMPONENTS) ; do \
		cp -Rf $(POCO_BASE)/$$comp/include/* $(INSTALLDIR)/include/ ; \
	done
	find $(POCO_BUILD)/lib -name "libPoco*" -exec cp -Rf {} $(INSTALLDIR)/lib \;
