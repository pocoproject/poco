#
# Makefile
#
# Makefile for Poco ODBC
#

include $(POCO_BASE)/build/rules/global

include ODBC.make

objects = Binder ConnectionHandle Connector EnvironmentHandle \
	Extractor ODBCException ODBCMetaColumn ODBCStatementImpl \
	Parameter Preparator SessionImpl TypeInfo Unicode Utility

target_includes = $(POCO_BASE)/Data/testsuite/include
ifndef POCO_DATA_NO_SQL_PARSER
	target_includes += $(POCO_BASE)/Data/src
endif

target         = PocoDataODBC
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
