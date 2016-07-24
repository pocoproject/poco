#
# Makefile
#
# $Id: //poco/1.5/Data/PostgreSQL/Makefile#1 $
#
# Makefile for Poco PostgreSQL
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I/usr/include/postgresql -I/usr/local/include/postgresql -I/usr/local/postgresql/include -I/opt/postgresql/include

SYSLIBS += -L/usr/lib$(LIB64SUFFIX)/postgresql -L/usr/local/lib$(LIB64SUFFIX)/postgresql -L/usr/local/postgresql/lib$(LIB64SUFFIX) -L/opt/postgresql/lib$(LIB64SUFFIX) -lpq

objects = Extractor Binder SessionImpl Connector \
	PostgreSQLStatementImpl PostgreSQLException \
	SessionHandle StatementExecutor PostgreSQLTypes Utility

target         = PocoDataPostgreSQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
