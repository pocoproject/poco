#
# Makefile
#
# Makefile for POCO Trace and dependencies
#

ifndef OSNAME
OSNAME := $(shell uname)
endif

.PHONY: clean distclean backtrace trace all

all: trace

trace: backtrace
	$(MAKE) -f Makefile-Trace $(MAKECMDGOALS) $(MAKEARGS)

backtrace:
ifeq ($(OSNAME), Linux)
	$(MAKE) -f Makefile-backtrace $(MAKECMDGOALS) $(MAKEARGS) install
endif

clean distclean:
	$(MAKE) -f Makefile-Trace $(MAKECMDGOALS) $(MAKEARGS)
ifeq ($(OSNAME), Linux)
	$(MAKE) -f Makefile-backtrace $(MAKECMDGOALS) $(MAKEARGS)
endif
