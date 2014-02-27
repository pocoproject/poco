#
# Makefile
#
# $Id: //poco/1.5/Data/PostgreSQL/testsuite/Makefile#1 $
#
# Makefile for Poco PostgreSQL testsuite
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I./../include -I$(POSTGRESQL_BASE)/include

# Note: linking order is important, do not change it.
SYSLIBS += -L$(POSTGRESQL_BASE)/lib -lpq -lz -lpthread -ldl

objects = PostgreSQLTestSuite Driver PostgreSQLTest SQLExecutor

target         = testrunner
target_version = 1
target_libs    = PocoDataPostgreSQL PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
