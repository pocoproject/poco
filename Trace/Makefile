#
# Makefile
#
# Makefile for POCO Trace and dependencies
#

ifndef OSNAME
OSNAME := $(shell uname)
endif

.PHONY: clean distclean trace all

all: trace

trace:
	$(MAKE) -f Makefile-Trace $(MAKECMDGOALS) $(MAKEARGS)

clean distclean:
	$(MAKE) -f Makefile-Trace $(MAKECMDGOALS) $(MAKEARGS)
