#
# Makefile
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

include $(POCO_BASE)/Data/ODBC/ODBC.make

##################################################################################################
# Note:                                                                                          #
#  Do not change linking order or move this line up, these libs have to be linked in this order. #
##################################################################################################
ifneq ($(OSNAME),Darwin)
SYSLIBS += -lltdl
endif
ifneq ($(OSNAME),FreeBSD)
ifneq ($(OSNAME),Darwin)
SYSLIBS += -ldl
endif
endif

objects = ODBCTestSuite Driver \
	ODBCDB2Test ODBCMySQLTest ODBCOracleTest ODBCPostgreSQLTest \
	ODBCSQLiteTest ODBCSQLServerTest ODBCTest SQLExecutor

ifeq ($(POCO_CONFIG),MinGW)
	objects += ODBCAccessTest
endif

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes += $(POCO_BASE)/Data/SQLParser $(POCO_BASE)/Data/SQLParser/src
endif

target           = testrunner
target_version   = 1
target_libs      = PocoDataODBC PocoDataTest PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
