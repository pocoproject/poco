#
# Makefile
#
# $Id: //poco/1.3/Data/ODBC/testsuite/Makefile#1 $
#
# Makefile for Poco SQLite testsuite
#

include $(POCO_BASE)/build/rules/global

ifeq ($(OSNAME),Darwin)
SYSLIBS += -liodbc -liodbcinst
else
SYSLIBS += -lodbc -lodbcinst
endif

objects = ODBCTestSuite Driver \
	ODBCDB2Test ODBCMySQLTest ODBCOracleTest \
	ODBCPostgreSQLTest ODBCSQLiteTest SQLExecutor

target         = testrunner
target_version = 1
target_libs    = PocoODBC PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
