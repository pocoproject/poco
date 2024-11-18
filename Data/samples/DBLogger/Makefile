#
# Makefile
#
# Makefile for Poco Data DBLogger sample
#

include $(POCO_BASE)/build/rules/global

objects = DBLogger SQLLogInserter

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes = $(POCO_BASE)/Data/src
endif

target         = DBLogger
target_version = 1
target_libs    = PocoDataSQLite PocoData PocoFoundation PocoUtil

include $(POCO_BASE)/build/rules/exec
