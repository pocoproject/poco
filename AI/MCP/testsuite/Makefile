#
# Makefile
#
# Makefile for Poco AI MCP testsuite
#

.PHONY: projects
%: projects ; @:
projects:
	$(MAKE) -f Makefile-Driver $(MAKECMDGOALS)
	$(MAKE) -f Makefile-TestServer $(MAKECMDGOALS)
