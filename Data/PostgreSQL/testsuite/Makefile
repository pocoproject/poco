#
# Makefile
#
# $Id: //poco/1.5/Data/PostgreSQL/testsuite/Makefile#1 $
#
# Makefile for Poco PostgreSQL testsuite
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I/usr/include/postgresql -I/usr/local/include/postgresql -I/usr/local/postgresql/include -I/opt/postgresql/include

SYSLIBS += -L/usr/lib$(LIB64SUFFIX)/postgresql -L/usr/local/lib$(LIB64SUFFIX)/postgresql -L/usr/local/postgresql/lib$(LIB64SUFFIX) -L/opt/postgresql/lib$(LIB64SUFFIX) -lpq

# Note: linking order is important, do not change it.
SYSLIBS += -lz -lpthread -ldl

objects = PostgreSQLTestSuite Driver PostgreSQLTest SQLExecutor

target         = testrunner
target_version = 1
target_libs    = PocoDataPostgreSQL PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
