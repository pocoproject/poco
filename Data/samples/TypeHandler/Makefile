#
# Makefile
#
# Makefile for Poco Data TypeHandler sample
#

include $(POCO_BASE)/build/rules/global

objects = TypeHandler

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes = $(POCO_BASE)/Data/src
endif

target         = TypeHandler
target_version = 1
target_libs    = PocoDataSQLite PocoData PocoFoundation

include $(POCO_BASE)/build/rules/exec
