#
# Makefile
#
# $Id: //poco/1.5/Data/PostgreSQL/Makefile#1 $
#
# Makefile for Poco PostgreSQL
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -lpq

objects = Extractor Binder SessionImpl Connector \
	PostgreSQLStatementImpl PostgreSQLException \
	SessionHandle StatementExecutor Utility

target         = PocoDataPostgreSQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
