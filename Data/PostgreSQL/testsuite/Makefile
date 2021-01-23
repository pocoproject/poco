#
# Makefile
#
# Makefile for Poco PostgreSQL testsuite
#

include $(POCO_BASE)/build/rules/global

ifeq (0, $(shell test -e /usr/include/postgresql; echo $$?))
INCLUDE += -I/usr/include/postgresql
endif
ifeq (0, $(shell test -e /usr/local/include/postgresql; echo $$?))
INCLUDE += -I/usr/local/include/postgresql
endif
ifeq (0, $(shell test -e /usr/local/postgresql/include; echo $$?))
INCLUDE += -I/usr/local/postgresql/include
endif
ifeq (0, $(shell test -e /opt/postgresql/include; echo $$?))
INCLUDE += -I/opt/postgresql/include
endif
ifeq (0, $(shell test -e /usr/local/opt/libpq/include; echo $$?))
INCLUDE += -I/usr/local/opt/libpq/include
endif

ifeq (0, $(shell test -e /usr/lib$(LIB64SUFFIX)/postgresql; echo $$?))
SYSLIBS += -L/usr/lib$(LIB64SUFFIX)/postgresql
endif
ifeq (0, $(shell test -e /usr/local/lib$(LIB64SUFFIX)/postgresql; echo $$?))
SYSLIBS += -L/usr/local/lib$(LIB64SUFFIX)/postgresql
endif
ifeq (0, $(shell test -e /usr/local/postgresql/lib$(LIB64SUFFIX); echo $$?))
SYSLIBS += -L/usr/local/postgresql/lib$(LIB64SUFFIX)
endif
ifeq (0, $(shell test -e /opt/postgresql/lib$(LIB64SUFFIX); echo $$?))
SYSLIBS += -L/opt/postgresql/lib$(LIB64SUFFIX)
endif
ifeq (0, $(shell test -e /opt/postgresql/lib$(LIB64SUFFIX); echo $$?))
SYSLIBS += -L/opt/postgresql/lib$(LIB64SUFFIX)
endif

# Note: linking order is important, do not change it.
SYSLIBS += -lpq -lz -lpthread -ldl

objects = PostgreSQLTestSuite Driver PostgreSQLTest SQLExecutor

target         = testrunner
target_version = 1
target_libs    = PocoDataPostgreSQL PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
