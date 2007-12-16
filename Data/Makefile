#
# Makefile
#
# $Id: //poco/Main/Data/Makefile#11 $
#
# Makefile for Poco Data
#

include $(POCO_BASE)/build/rules/global

objects = AbstractBinder AbstractBinding AbstractExtraction \
	AbstractExtractor AbstractPreparation AbstractPrepare Bulk \
	Connector BLOB BLOBStream DataException Date Limit MetaColumn \
	PooledSessionHolder PooledSessionImpl Position \
	Range RecordSet Row RowFormatter RowIterator \
	Session SessionFactory SessionImpl \
	SessionPool Statement StatementCreator StatementImpl Time

target         = PocoData
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
