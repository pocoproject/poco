#
# Makefile
#
# Makefile for Poco SQL
#

include $(POCO_BASE)/build/rules/global

objects = AbstractBinder AbstractBinding AbstractExtraction AbstractExtractor \
	AbstractPreparation AbstractPreparator ArchiveStrategy Transaction \
	Bulk Connector SQLException Date DynamicLOB Limit JSONRowFormatter \
	MetaColumn PooledSessionHolder PooledSessionImpl Position \
	Range RecordSet Row RowFilter RowFormatter RowIterator \
	SimpleRowFormatter Session SessionFactory SessionImpl \
	SessionPool SessionPoolContainer SQLChannel \
	Statement StatementCreator StatementImpl Time

target         = PocoSQL
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
