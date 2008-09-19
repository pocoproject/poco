#
# Makefile
#
# $Id: //poco/1.3/Data/ODBC/testsuite/Makefile#2 $
#
# Makefile for Poco SQLite testsuite
#

include $(POCO_BASE)/build/rules/global

ifeq ($(POCO_CONFIG),MinGW)
SYSLIBS += -lodbc32
else
ifeq (0, $(shell test -e /usr/lib/libodbc$(SHAREDLIBLINKEXT); echo $$?))
SYSLIBS += -lodbc -lodbcinst
COMMONFLAGS += -DPOCO_UNIXODBC
else
ifeq (0, $(shell test -e /usr/lib/libiodbc$(SHAREDLIBLINKEXT); echo $$?))
SYSLIBS += -liodbc -liodbcinst
COMMONFLAGS += -DPOCO_IODBC
else
$(error No ODBC library found. Please install unixODBC or iODBC and try again)
endif
endif
endif

objects = ODBCTestSuite Driver \
	ODBCDB2Test ODBCMySQLTest ODBCOracleTest \
	ODBCPostgreSQLTest ODBCSQLiteTest SQLExecutor

target         = testrunner
target_version = 1
target_libs    = PocoODBC PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
