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

target         = PocoDataMySQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
