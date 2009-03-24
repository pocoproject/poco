#
# Makefile
#
# $Id: //poco/Main/Data/SQLite/Makefile#4 $
#
# Makefile for Poco SQLite
#

include $(POCO_BASE)/build/rules/global

SYSFLAGS += -DSQLITE_THREADSAFE=1 -DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_PROGRESS_CALLBACK \
	-DSQLITE_OMIT_COMPLETE -DSQLITE_OMIT_TCL_VARIABLE

INCLUDE += -Isrc

objects = Binder Extractor SessionImpl Connector \
	SQLiteException SQLiteStatementImpl Utility \
	sqlite3

target         = PocoSQLite
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
