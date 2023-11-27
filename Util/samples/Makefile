#
# Makefile
#
# Makefile for Poco Util Samples
#

.PHONY: projects
clean distclean all: projects
projects:
	$(MAKE) -C SampleApp $(MAKECMDGOALS)
	$(MAKE) -C SampleServer $(MAKECMDGOALS)
	$(MAKE) -C pkill $(MAKECMDGOALS)
