#
# Makefile
#
# Makefile for Poco Data
#

include $(POCO_BASE)/build/rules/global

objects = AbstractBinder AbstractBinding AbstractExtraction AbstractExtractor \
	AbstractPreparation AbstractPreparator ArchiveStrategy Transaction \
	Bulk Connector DataException Date DynamicLOB JSONRowFormatter \
	Limit MetaColumn PooledSessionHolder PooledSessionImpl Position \
	Range RecordSet Row RowFilter RowFormatter RowIterator \
	SimpleRowFormatter Session SessionFactory SessionImpl \
	SessionPool SessionPoolContainer SQLChannel \
	Statement StatementCreator StatementImpl Time Transcoder

ifndef POCO_DATA_NO_SQL_PARSER
	objects += SQLParser SQLParserResult \
		bison_parser flex_lexer \
		CreateStatement PrepareStatement SQLStatement \
		Expr statements sqlhelper
	target_includes += $(POCO_BASE)/Data/SQLParser $(POCO_BASE)/Data/SQLParser/src
endif

target          = PocoData
target_version  = $(LIBVERSION)
target_libs     = PocoFoundation

include $(POCO_BASE)/build/rules/lib
