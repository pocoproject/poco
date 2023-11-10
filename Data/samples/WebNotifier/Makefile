#
# Makefile
#
# Makefile for Poco Data WebNotifier sample
#

include $(POCO_BASE)/build/rules/global

objects = WebNotifier

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes = $(POCO_BASE)/Data/src
endif

target         = WebNotifier
target_version = 1
target_libs    = PocoDataSQLite PocoData PocoNet PocoFoundation

include $(POCO_BASE)/build/rules/exec
