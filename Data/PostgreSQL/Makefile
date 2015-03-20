#
# Makefile
#
# $Id: //poco/1.5/Data/PostgreSQL/Makefile#1 $
#
# Makefile for Poco PostgreSQL
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

INCLUDE += -I$(POCO_POSTGRESQL_INCLUDE)

SYSLIBS += -L$(POCO_POSTGRESQL_LIB) -lpq

objects = Extractor Binder SessionImpl Connector \
	PostgreSQLStatementImpl PostgreSQLException \
	SessionHandle StatementExecutor PostgreSQLTypes Utility

target         = PocoDataPostgreSQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
