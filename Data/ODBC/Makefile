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

target         = PocoDataODBC
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
