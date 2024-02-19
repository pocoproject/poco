#
# Makefile
#
# Makefile for Poco PostgreSQL testsuite
#

include $(POCO_BASE)/build/rules/global

include $(POCO_BASE)/Data/PostgreSQL/PostgreSQL.make

# Note: linking order is important, do not change it.
SYSLIBS += -lpq -lz -lpthread -ldl

objects = PostgreSQLTestSuite Driver PostgreSQLTest SQLExecutor

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes += $(POCO_BASE)/Data/SQLParser $(POCO_BASE)/Data/SQLParser/src
endif

target         = testrunner
target_version = 1
target_libs    = PocoDataPostgreSQL PocoDataTest PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
