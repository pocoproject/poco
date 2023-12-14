#
# Makefile
#
# Makefile for Poco PostgreSQL
#

include $(POCO_BASE)/build/rules/global

include PostgreSQL.make

objects = Extractor BinaryExtractor Binder SessionImpl Connector \
	PostgreSQLStatementImpl PostgreSQLException \
	SessionHandle StatementExecutor PostgreSQLTypes Utility


target_includes = $(POCO_BASE)/Data/testsuite/include
ifndef POCO_DATA_NO_SQL_PARSER
	target_includes += $(POCO_BASE)/Data/src
endif


target         = PocoDataPostgreSQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
