#
# Makefile
#
# Makefile for Poco Data Tests
#

.PHONY: projects
clean distclean all: projects
projects:
	$(MAKE) -f Makefile-testrunner $(MAKECMDGOALS)
	$(MAKE) -C ../DataTest $(MAKECMDGOALS)
