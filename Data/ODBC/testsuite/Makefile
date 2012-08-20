#
# Makefile
#
# $Id: //poco/Main/Data/ODBC/testsuite/Makefile#1 $
#
# Makefile for Poco SQLite testsuite
#
# For Unicode support, add following to COMMONFLAGS:
#
# -DUNICODE
#
# Unicode is supported only for UnixODBC
#

include $(POCO_BASE)/build/rules/global

ifeq ($(LINKMODE),STATIC)
LIBLINKEXT = $(STATICLIBLINKEXT)
else
LIBLINKEXT = $(SHAREDLIBLINKEXT)
endif

INCLUDE += $(POCO_ODBC_INCLUDE)
SYSLIBS += $(POCO_ODBC_LIBRARY)

ifeq ($(POCO_CONFIG),MinGW)
# -DODBCVER=0x0300: SQLHandle declaration issue
# -DNOMINMAX      : MIN/MAX macros defined in windows conflict with libstdc++
CXXFLAGS += -DODBCVER=0x0300 -DNOMINMAX
else ifeq ($(POCO_CONFIG),CYGWIN)
# -DODBCVER=0x0300: SQLHandle declaration issue
# -DNOMINMAX      : MIN/MAX macros defined in windows conflict with libstdc++
CXXFLAGS += -DODBCVER=0x0300 -DNOMINMAX
# CYGWIN platform has its own ODBC library in /lib/w32api 
SYSLIBS += -L/lib/w32api -lodbc32 -lodbccp32
else
ifeq (0, $(shell test -e $(POCO_ODBC_LIBRARY)/libodbc$(LIBLINKEXT); echo $$?))
SYSLIBS += -lodbc -lodbcinst
COMMONFLAGS += -DPOCO_UNIXODBC
else
ifeq (0, $(shell test -h $(POCO_ODBC_LIBRARY)/libodbc$(LIBLINKEXT); echo $$?))
SYSLIBS += -lodbc -lodbcinst
COMMONFLAGS += -DPOCO_UNIXODBC
else
ifeq (0, $(shell test -e $(POCO_ODBC_LIBRARY)/libiodbc$(LIBLINKEXT); echo $$?))
SYSLIBS += -liodbc -liodbcinst
COMMONFLAGS += -DPOCO_IODBC -I/usr/include/iodbc
else
ifeq (0, $(shell test -h $(POCO_ODBC_LIBRARY)/libiodbc$(LIBLINKEXT); echo $$?))
SYSLIBS += -liodbc -liodbcinst -I/usr/include/iodbc
COMMONFLAGS += -DPOCO_IODBC
else
ifndef (POCO_NO_ODBC)
$(warning No ODBC library found. Please install unixODBC or iODBC.)
endif
endif
endif
endif
endif
endif

##################################################################################################
# Note:                                                                                          #
#  Do not change linking order or move this line up, these libs have to be linked in this order. #
##################################################################################################
SYSLIBS += -lltdl -ldl

objects = ODBCTestSuite Driver \
	ODBCDB2Test ODBCMySQLTest ODBCOracleTest ODBCPostgreSQLTest \
	ODBCSQLiteTest ODBCSQLServerTest ODBCTest SQLExecutor
ifeq ($(POCO_CONFIG),MinGW)
    objects += ODBCAccessTest
endif

target         = testrunner
target_version = 1
target_libs    = PocoDataODBC PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
