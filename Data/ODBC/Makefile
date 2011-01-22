#
# Makefile
#
# $Id: //poco/Main/Data/ODBC/Makefile#2 $
#
# Makefile for Poco ODBC
#

include $(POCO_BASE)/build/rules/global

# adjust for the target system (usually '/usr/lib' or '/usr/local/lib')
ODBCLIBDIR = /usr/lib

INCLUDE += -I/usr/local/include -I/usr/include -I/usr/include/odbc -I/usr/local/include/odbc
SYSLIBS += -L/usr/local/lib/odbc -L/usr/lib/odbc -L/usr/lib -L/usr/local/lib

ifeq ($(POCO_CONFIG),MinGW)
# Lack of unixODBC or iODBC is not an error for MinGW platform since it uses Windows odbc32.lib
else
ifeq ($(POCO_CONFIG),CYGWIN)
# -DODBCVER=0x0300: SQLHandle declaration issue
# -DNOMINMAX      : MIN/MAX macros defined in windows conflict with libstdc++
CXXFLAGS += -DODBCVER=0x0300 -DNOMINMAX
# CYGWIN platform has its own ODBC library in /lib/w32api 
SYSLIBS += -L/lib/w32api -lodbc32 -lodbccp32
else
ifeq (0, $(shell test -e $(ODBCLIBDIR)/libodbc$(SHAREDLIBLINKEXT); echo $$?))
SYSLIBS += -lodbc -lodbcinst
COMMONFLAGS += -DPOCO_UNIXODBC
else
ifeq (0, $(shell test -h $(ODBCLIBDIR)/libodbc$(SHAREDLIBLINKEXT); echo $$?))
SYSLIBS += -lodbc -lodbcinst
COMMONFLAGS += -DPOCO_UNIXODBC
else
ifeq (0, $(shell test -e $(ODBCLIBDIR)/libiodbc$(SHAREDLIBLINKEXT); echo $$?))
SYSLIBS += -liodbc -liodbcinst
COMMONFLAGS += -DPOCO_IODBC
else
ifeq (0, $(shell test -h $(ODBCLIBDIR)/libiodbc$(SHAREDLIBLINKEXT); echo $$?))
SYSLIBS += -liodbc -liodbcinst
COMMONFLAGS += -DPOCO_IODBC
else
$(error No ODBC library found. Please install unixODBC or iODBC and try again)
endif
endif
endif
endif
endif
endif

objects = Binder ConnectionHandle Connector DataTypes EnvironmentHandle \
	Extractor ODBCColumn ODBCException ODBCStatementImpl \
	Parameter Preparation SessionImpl Utility 

target         = PocoDataODBC
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
