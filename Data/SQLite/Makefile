#
# Makefile
#
# $Id: //poco/Main/Data/SQLite/Makefile#4 $
#
# Makefile for Poco SQLite
#

include $(POCO_BASE)/build/rules/global

SYSFLAGS += -DTHREADSAFE -DNO_TCL -DSQLITE_DISABLE_LFS

objects = Binder Extractor SessionImpl Connector \
	SQLiteException SQLiteStatementImpl Utility \
	sqlite3

target         = PocoSQLite
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
