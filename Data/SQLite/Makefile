#
# Makefile
#
# $Id: //poco/Main/Data/SQLite/Makefile#4 $
#
# Makefile for Poco SQLite
#

include $(POCO_BASE)/build/rules/global

SYSFLAGS += -DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_PROGRESS_CALLBACK \
	-DSQLITE_OMIT_COMPLETE -DSQLITE_OMIT_TCL_VARIABLE

ifeq ($(findstring SunOS,$(POCO_HOST_OSNAME)),SunOS)
  POCO_SUN_FORTE = $(findstring SunOS-SunForte, $(POCO_CONFIG)) 
  POCO_SUN_STUDIO = $(findstring SunOS-SunStudio, $(POCO_CONFIG)) 
  ifneq (,$or ($(POCO_SUN_FORTE), $(POCO_SUN_STUDIO)))
    CXXFLAGS += -erroff=hidevf
  endif
endif

objects = Binder Extractor SessionImpl Connector \
	SQLiteException SQLiteStatementImpl Utility \
	sqlite3

target         = PocoSQLite
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
