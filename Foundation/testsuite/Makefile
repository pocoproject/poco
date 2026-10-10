#
# Makefile
#
# Makefile for Poco Foundation testsuite
#

.PHONY: projects
# Forward any goal verbatim to the child Makefiles. Catches the mode targets
# (shared_release, shared_debug, ...) that come from build/rules/exec and are
# only defined in leaf Makefiles - this wrapper does not include those rules
# and must not duplicate their names. The pattern rule's empty recipe (`@:`)
# is a no-op; the real work happens in `projects`, which forwards
# $(MAKECMDGOALS) to the children that DO understand the goal.
%: projects ; @:
projects:
	$(MAKE) -f Makefile-Driver $(MAKECMDGOALS)
ifneq ($(LINKMODE),STATIC)
	$(MAKE) -f Makefile-TestLibrary $(MAKECMDGOALS)
	$(MAKE) -f Makefile-NonExistentLibrary $(MAKECMDGOALS)
	$(MAKE) -f Makefile-TestLibraryMissingDeps $(MAKECMDGOALS)
endif
	$(MAKE) -f Makefile-TestApp $(MAKECMDGOALS)
