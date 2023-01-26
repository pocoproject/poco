#
# PostgreSQL.make
#
# Makefile fragment for finding PostgreSQL library
#

ifndef POCO_PGSQL_INCLUDE
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
ifeq (0, $(shell test -e /opt/homebrew/opt/libpq/include; echo $$?))
INCLUDE += -I/opt/homebrew/opt/libpq/include
endif
endif

ifndef POCO_PGSQL_LIB
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
ifeq (0, $(shell test -e /usr/local/opt/libpq/lib; echo $$?))
SYSLIBS += -L/usr/local/opt/libpq/lib$(LIB64SUFFIX)
endif
ifeq (0, $(shell test -e /opt/homebrew/opt/libpq/lib; echo $$?))
SYSLIBS += -L/opt/homebrew/opt/libpq/lib$(LIB64SUFFIX)
endif
endif
SYSLIBS += -lpq

ifdef POCO_PGSQL_INCLUDE
INCLUDE += -I$(POCO_PGSQL_INCLUDE)
endif
ifdef POCO_PGSQL_LIB
SYSLIBS += -L$(POCO_PGSQL_LIB)
endif
