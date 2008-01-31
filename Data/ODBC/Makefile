#
# Makefile
#
# $Id: //poco/Main/DataConnectors/ODBC/Makefile#2 $
#
# Makefile for Poco ODBC
#

include $(POCO_BASE)/build/rules/global

ifeq ($(OSNAME),Darwin)
SYSLIBS += -liodbc -liodbcinst
else
SYSLIBS += -lodbc -lodbcinst
endif

objects = Binder ConnectionHandle DataTypes EnvironmentHandle \
	Extractor ODBCColumn ODBCException ODBCStatementImpl Parameter Preparation \
	SessionImpl Connector Utility 

target         = PocoODBC
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
