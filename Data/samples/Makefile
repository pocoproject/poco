#
# Makefile
#
# Makefile for Poco Data Samples
#

.PHONY: projects
clean distclean all: projects
projects:
	$(MAKE) -C Binding $(MAKECMDGOALS)
	$(MAKE) -C TypeHandler $(MAKECMDGOALS)
	$(MAKE) -C RecordSet $(MAKECMDGOALS)
	$(MAKE) -C RowFormatter $(MAKECMDGOALS)
	$(MAKE) -C Tuple $(MAKECMDGOALS)
	$(MAKE) -C WebNotifier $(MAKECMDGOALS)
