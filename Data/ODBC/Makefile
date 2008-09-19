#
# Makefile
#
# $Id: //poco/Main/Data/ODBC/Makefile#2 $
#
# Makefile for Poco ODBC
#

include $(POCO_BASE)/build/rules/global

ifeq ($(POCO_CONFIG),MinGW)
SYSLIBS += -lodbc32
else
ifeq (0, $(shell test -e /usr/lib/libodbc$(SHAREDLIBLINKEXT); echo $$?))
SYSLIBS += -lodbc -lodbcinst
COMMONFLAGS += -DPOCO_UNIXODBC
else
ifeq (0, $(shell test -h /usr/lib/libodbc$(SHAREDLIBLINKEXT); echo $$?))
SYSLIBS += -lodbc -lodbcinst
COMMONFLAGS += -DPOCO_UNIXODBC
else
ifeq (0, $(shell test -e /usr/lib/libiodbc$(SHAREDLIBLINKEXT); echo $$?))
SYSLIBS += -liodbc -liodbcinst
COMMONFLAGS += -DPOCO_IODBC
else
ifeq (0, $(shell test -h /usr/lib/libiodbc$(SHAREDLIBLINKEXT); echo $$?))
SYSLIBS += -liodbc -liodbcinst
COMMONFLAGS += -DPOCO_IODBC
else
$(error No ODBC library found. Please install unixODBC or iODBC and try again)
endif
endif
endif
endif
endif

objects = Binder ConnectionHandle Connector DataTypes EnvironmentHandle \
	Extractor ODBCColumn ODBCException ODBCStatementImpl \
	Parameter Preparation SessionImpl Utility 

target         = PocoODBC
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
