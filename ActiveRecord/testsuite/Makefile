#
# Makefile
#
# Makefile for Poco ActiveRecord testsuite
#

include $(POCO_BASE)/build/rules/global

objects = ActiveRecordTestSuite ActiveRecordTest Driver \
	Employee Role

target         = testrunner
target_version = 1
target_libs    = PocoActiveRecord PocoDataSQLite PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
