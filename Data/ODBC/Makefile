#
# Makefile
#
# $Id: //poco/Main/DataConnectors/ODBC/Makefile#2 $
#
# Makefile for Poco ODBC
#

include $(POCO_BASE)/build/rules/global

ifeq (0, $(shell test -e /usr/lib/libodbc.so; echo $$?))
SYSLIBS += -lodbc -lodbcinst
else ifeq (0, $(shell test -e /usr/lib/libiodbc.so; echo $$?))
SYSLIBS += -liodbc -liodbcinst
else
$(error No ODBC library found. Please install unixODBC or iODBC and try again)
endif

objects = Binder ConnectionHandle TypeInfo EnvironmentHandle \
	Extractor ODBCColumn ODBCException ODBCStatementImpl Parameter Preparation \
	SessionImpl Connector Utility 

target         = PocoODBC
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
