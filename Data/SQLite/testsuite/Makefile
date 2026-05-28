#
# Makefile
#
# Makefile for Poco SQLite testsuite
#

include $(POCO_BASE)/build/rules/global

objects = SQLiteTestSuite Driver \
	SQLiteTest UtilityTest SQLiteThreadSafetyTest

ifndef POCO_DATA_NO_SQL_PARSER
	# MemoryDBTest #includes <sqlite3.h> to lower SQLITE_LIMIT_ATTACHED at
	# runtime. Point at the bundled SQLite source dir directly rather than
	# Data/SQLite/src - the latter only contains sqlite3.h after the bundled
	# SQLite Makefile has run its `ln -sf` prebuild, which doesn't happen in
	# clean cmake-style or out-of-tree builds. For POCO_SQLITE_UNBUNDLED the
	# system header is on the default include path so no extra -I is needed.
	target_includes = $(POCO_BASE)/Data/SQLParser $(POCO_BASE)/Data/SQLParser/src \
		$(POCO_BASE)/dependencies/sqlite3/src
	objects += MemoryDBTest
endif

target         = testrunner
target_version = 1
target_libs    = PocoDataSQLite PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
