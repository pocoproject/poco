#
# Makefile
#
# $Id: //poco/Main/Data/ODBC/testsuite/Makefile#1 $
#
# Makefile for Poco SQLite testsuite
#

include $(POCO_BASE)/build/rules/global

ifeq (0, $(shell test -e /usr/lib/libodbc.so; echo $$?))
SYSLIBS += -lodbc -lodbcinst
else ifeq (0, $(shell test -e /usr/lib/libiodbc.so; echo $$?))
SYSLIBS += -liodbc -liodbcinst
else
$(error No ODBC library found. Please install unixODBC or iODBC and try again)
endif

objects = ODBCTestSuite Driver \
	ODBCDB2Test ODBCMySQLTest ODBCOracleTest ODBCPostgreSQLTest \
	ODBCSQLiteTest ODBCSQLServerTest ODBCTest SQLExecutor

target         = testrunner
target_version = 1
target_libs    = PocoODBC PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
