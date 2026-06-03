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
	$(MAKE) -C DBLogger $(MAKECMDGOALS)
ifndef POCO_DATA_NO_SQL_PARSER
	$(MAKE) -C MemoryDB $(MAKECMDGOALS)
endif
