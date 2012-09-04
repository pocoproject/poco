#
# Makefile
#
# $Id: //poco/Main/Data/ODBC/Makefile#2 $
#
# Makefile for Poco ODBC
#

include $(POCO_BASE)/build/rules/global

include ODBC.make

objects = Binder ConnectionHandle Connector DataTypes EnvironmentHandle \
	Extractor ODBCColumn ODBCException ODBCStatementImpl \
	Parameter Preparation SessionImpl Utility 

target         = PocoDataODBC
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
