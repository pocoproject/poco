#
# Makefile
#
# $Id: //poco/1.3/Data/MySQL/Makefile#3 $
#
# Makefile for Poco MySQL
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -L/usr/local/lib/mysql
INCLUDE += -I/usr/local/include/mysql/
SYSFLAGS += -DTHREADSAFE -DNO_TCL

objects = Binder Extractor SessionImpl Connector \
	MySQLStatementImpl ResultMetadata MySQLException \
	SessionHandle StatementExecutor

target         = PocoMySQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
