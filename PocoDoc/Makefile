#
# Makefile
#
# Makefile for Poco PocoDoc
#

include $(POCO_BASE)/build/rules/global

objects = DocWriter PocoDoc

target         = PocoDoc
target_version = 1
target_libs    = PocoCppParser PocoDataSQLite PocoData PocoFoundation PocoXML PocoUtil

ifdef POCO_ENABLE_SQLITE_FTS5
CXXFLAGS += -DPOCO_ENABLE_SQLITE_FTS5
endif

include $(POCO_BASE)/build/rules/exec
