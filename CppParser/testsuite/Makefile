#
# Makefile
#
# Makefile for Poco CppParser testsuite
#

.PHONY: projects
%: projects ; @:
projects:
	$(MAKE) -f Makefile-Driver $(MAKECMDGOALS)
	$(MAKE) -f Makefile-TestPreprocessor $(MAKECMDGOALS)
