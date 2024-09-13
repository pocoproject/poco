#
# Makefile
#
# Makefile for POCO Trace and dependencies
#

.PHONY: clean distclean backtrace trace all

all: trace

trace: backtrace
	$(MAKE) -f Makefile-Trace $(MAKECMDGOALS) $(MAKEARGS)

backtrace:
	$(MAKE) -f Makefile-backtrace $(MAKECMDGOALS) $(MAKEARGS) install

clean distclean:
	$(MAKE) -f Makefile-Trace $(MAKECMDGOALS) $(MAKEARGS)
	$(MAKE) -f Makefile-backtrace $(MAKECMDGOALS) $(MAKEARGS)
