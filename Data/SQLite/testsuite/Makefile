#
# Makefile
#
# Makefile for Poco SQLite testsuite
#

include $(POCO_BASE)/build/rules/global

objects = SQLiteTestSuite Driver \
	SQLiteTest

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes = $(POCO_BASE)/Data/SQLParser $(POCO_BASE)/Data/SQLParser/src
endif

target         = testrunner
target_version = 1
target_libs    = PocoDataSQLite PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
