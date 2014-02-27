#
# Makefile
#
# $Id: //poco/1.5/Data/PostgreSQL/Makefile#1 $
#
# Makefile for Poco PostgreSQL
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -L$(POSTGRESQL_BASE)/lib -lpq
INCLUDE += -I$(POSTGRESQL_BASE)/include
SYSFLAGS += -DTHREADSAFE -DNO_TCL

objects = Extractor Binder SessionImpl Connector \
	PostgreSQLStatementImpl PostgreSQLException \
	SessionHandle StatementExecutor Utility

target         = PocoDataPostgreSQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
