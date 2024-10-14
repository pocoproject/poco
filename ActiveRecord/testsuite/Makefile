#
# Makefile
#
# Makefile for Poco ActiveRecord testsuite
#

include $(POCO_BASE)/build/rules/global

objects = ActiveRecordTestSuite ActiveRecordTest Driver \
	Employee Role

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes = $(POCO_BASE)/Data/src
endif

target         = testrunner
target_version = 1
target_libs    = PocoActiveRecord PocoDataSQLite PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
