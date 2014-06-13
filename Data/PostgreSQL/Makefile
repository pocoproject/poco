#
# Makefile
#
# $Id: //poco/1.5/Data/PostgreSQL/Makefile#1 $
#
# Makefile for Poco PostgreSQL
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -L/usr/local/lib$(LIB64SUFFIX)/postgresql/9.3/lib -L/usr/lib$(LIB64SUFFIX)/postgresql/9.3/lib -L$(POSTGRESQL_BASE)/lib -lpq
INCLUDE += -I/usr/local/include/postgresql/ -I/usr/include/postgresql/ -I$(POSTGRESQL_BASE)/include
SYSFLAGS += -DTHREADSAFE -DNO_TCL

objects = Extractor Binder SessionImpl Connector \
	PostgreSQLStatementImpl PostgreSQLException \
	SessionHandle StatementExecutor Utility

target         = PocoDataPostgreSQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
