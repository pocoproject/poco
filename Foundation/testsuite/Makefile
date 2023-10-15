#
# Makefile
#
# Makefile for Poco Foundation testsuite
#

.PHONY: projects
clean distclean all: projects
projects:
	$(MAKE) -f Makefile-Driver $(MAKECMDGOALS)
ifneq ($(LINKMODE),STATIC)
	$(MAKE) -f Makefile-TestLibrary $(MAKECMDGOALS)
endif
	$(MAKE) -f Makefile-TestApp $(MAKECMDGOALS)
