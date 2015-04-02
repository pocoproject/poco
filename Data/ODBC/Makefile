#
# Makefile
#
# $Id: //poco/Main/Data/ODBC/Makefile#2 $
#
# Makefile for Poco ODBC
#

include $(POCO_BASE)/build/rules/global

SHLIBFLAGS += $(POCO_ODBC_LINK_EXTRA)

include ODBC.make

objects = Binder ConnectionHandle Connector EnvironmentHandle \
	Extractor ODBCException ODBCMetaColumn ODBCStatementImpl \
	Parameter Preparator SessionImpl TypeInfo Unicode Utility 

target         = PocoDataODBC
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
