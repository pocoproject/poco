#
# Makefile
#
# Makefile for Poco Data MemoryDBBrowser sample
#

include $(POCO_BASE)/build/rules/global

objects = MemoryDBBrowser

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes = $(POCO_BASE)/Data/src
endif

target         = MemoryDBBrowser
target_version = 1
target_libs    = PocoDataSQLite PocoData PocoNet PocoUtil PocoJSON PocoFoundation

include $(POCO_BASE)/build/rules/exec
