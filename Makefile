#
# Makefile
#
# $Id: //poco/1.1.0/dist/Makefile#2 $
#
# Makefile for Poco
#

ifndef POCO_BASE
$(warning WARNING: POCO_BASE is not defined. Assuming current directory.)
export POCO_BASE=$(shell pwd)
endif

.DEFAULT all:
	$(MAKE) -C CppUnit $(MAKECMDGOALS)
	$(MAKE) -C Foundation $(MAKECMDGOALS)
	$(MAKE) -C Foundation/testsuite $(MAKECMDGOALS)
	$(MAKE) -C Foundation/samples $(MAKECMDGOALS)
	$(MAKE) -C XML $(MAKECMDGOALS)
	$(MAKE) -C XML/testsuite $(MAKECMDGOALS)
	$(MAKE) -C XML/samples $(MAKECMDGOALS)
	$(MAKE) -C Util $(MAKECMDGOALS)
	$(MAKE) -C Util/testsuite $(MAKECMDGOALS)
	$(MAKE) -C Util/samples $(MAKECMDGOALS)
	$(MAKE) -C Net $(MAKECMDGOALS)
	$(MAKE) -C Net/testsuite $(MAKECMDGOALS)
	$(MAKE) -C Net/samples $(MAKECMDGOALS)

.PHONY test:
	$(POCO_BASE)/build/script/testall
