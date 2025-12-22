#
# Makefile
#
# Makefile for Poco ActiveRecord Library
#

include $(POCO_BASE)/build/rules/global

objects = Context ActiveRecord IDTraits StatementPlaceholderProvider

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes += $(POCO_BASE)/Data/SQLParser $(POCO_BASE)/Data/SQLParser/src
endif

target         = PocoActiveRecord
target_version = 1
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
