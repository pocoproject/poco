#
# Makefile
#
# Makefile for Poco MySQL
#

include $(POCO_BASE)/build/rules/global

include MySQL.make

SYSLIBS += -lmysqlclient
SYSFLAGS += -DTHREADSAFE -DNO_TCL

objects = Binder Extractor SessionImpl Connector \
	MySQLStatementImpl ResultMetadata MySQLException \
	SessionHandle StatementExecutor Utility

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes = $(POCO_BASE)/Data/src
endif

target         = PocoDataMySQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
