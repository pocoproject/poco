#
# Makefile
#
# $Id: //poco/1.5/Data/PostgreSQL/testsuite/Makefile#1 $
#
# Makefile for Poco PostgreSQL testsuite
#

include $(POCO_BASE)/build/rules/global

ifndef POCO_POSTGRESQL_INCLUDE
POCO_POSTGRESQL_INCLUDE = /usr/include/postgresql
endif

ifndef POCO_POSTGRESQL_LIB
ifeq (0, $(shell test -d /usr/lib/$(OSARCH)-linux-gnu; echo $$?))
POCO_POSTGRESQL_LIB = /usr/lib/$(OSARCH)-linux-gnu
else ifeq (0, $(shell test -d /usr/lib64; echo $$?))
POCO_POSTGRESQL_LIB = /usr/lib64
else
POCO_POSTGRESQL_LIB = /usr/lib
endif
endif

ifeq ($(LINKMODE),STATIC)
LIBLINKEXT = .a
else
LIBLINKEXT = $(SHAREDLIBLINKEXT)
endif

INCLUDE += -I./../include
INCLUDE += -I$(POCO_POSTGRESQL_INCLUDE)

SYSLIBS += -L$(POCO_POSTGRESQL_LIB) -lpq

# Note: linking order is important, do not change it.
SYSLIBS += -lz -lpthread -ldl

objects = PostgreSQLTestSuite Driver PostgreSQLTest SQLExecutor

target         = testrunner
target_version = 1
target_libs    = PocoDataPostgreSQL PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
