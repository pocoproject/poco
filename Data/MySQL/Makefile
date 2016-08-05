#
# Makefile
#
# $Id: //poco/1.4/Data/MySQL/Makefile#1 $
#
# Makefile for Poco MySQL
#

include $(POCO_BASE)/build/rules/global

MYSQL_LIBDIR ?= /usr/local/lib$(LIB64SUFFIX)/mysql \
		/usr/lib$(LIB64SUFFIX)/mysql \
		/usr/mysql/lib$(LIB64SUFFIX) \
		/usr/mysql/lib$(LIB64SUFFIX)/mysql \
		/usr/local/mysql/lib$(LIB64SUFFIX)
MYSQL_INCDIR ?= /usr/local/include/mysql/ \
		/usr/include/mysql/ \
		/usr/mysql/include/mysql \
		/usr/local/mysql/include

SYSLIBS += $(foreach dir,$(MYSQL_LIBDIR),-L$(dir)) -lmysqlclient
INCLUDE += $(foreach dir,$(MYSQL_INCDIR),-I$(dir))
SYSFLAGS += -DTHREADSAFE -DNO_TCL

objects = Binder Extractor SessionImpl Connector \
	MySQLStatementImpl ResultMetadata MySQLException \
	SessionHandle StatementExecutor Utility

target         = PocoDataMySQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
