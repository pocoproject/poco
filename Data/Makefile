#
# Makefile
#
# $Id: //poco/Main/Data/Makefile#11 $
#
# Makefile for Poco Data
#

include $(POCO_BASE)/build/rules/global

objects = AbstractBinder AbstractBinding AbstractExtraction \
	AbstractExtractor AbstractPreparation AbstractPrepare \
	BLOB BLOBStream DataException Limit MetaColumn \
	PooledSessionHolder PooledSessionImpl \
	Range RecordSet Row RowIterator Session SessionFactory SessionImpl \
	Connector SessionPool Statement StatementCreator StatementImpl

target         = PocoData
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
