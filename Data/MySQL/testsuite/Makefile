#
# Makefile
#
# Makefile for Poco MySQL testsuite
#

include $(POCO_BASE)/build/rules/global

include $(POCO_BASE)/Data/MySQL/MySQL.make

# Note: linking order is important, do not change it.
SYSLIBS += -lmysqlclient -lz -lpthread -ldl

objects = MySQLTestSuite Driver MySQLTest SQLExecutor

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes += $(POCO_BASE)/Data/SQLParser $(POCO_BASE)/Data/SQLParser/src
endif

target         = testrunner
target_version = 1
target_libs    = PocoDataMySQL PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
