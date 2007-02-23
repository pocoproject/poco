#
# Makefile
#
# $Id: //poco/Main/Foundation/testsuite/Makefile#8 $
#
# Makefile for Poco Foundation testsuite
#

.PHONY: projects
clean all: projects
projects:
	$(MAKE) -f Makefile-Driver $(MAKECMDGOALS)
	$(MAKE) -f Makefile-TestLibrary $(MAKECMDGOALS)
	$(MAKE) -f Makefile-TestApp $(MAKECMDGOALS)
