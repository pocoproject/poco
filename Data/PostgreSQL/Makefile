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

target         = PocoDataPostgreSQL
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
