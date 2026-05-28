//
// MemoryDBTest.h
//
// Definition of the MemoryDBTest class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MemoryDBTest_INCLUDED
#define MemoryDBTest_INCLUDED


#include "Poco/Data/SQLite/SQLite.h"
#include "CppUnit/TestCase.h"
#include <string>


class MemoryDBTest: public CppUnit::TestCase
{
public:
	MemoryDBTest(const std::string& name);
	~MemoryDBTest();

	void testPersistAndReload();
	void testManualFlushAndDirty();
	void testDestructorFlush();
	void testShardingBySize();
	void testActiveOnlyRewrite();
	void testUpdateSealedRow();
	void testTruncate();
	void testWithoutRowidRejected();
	void testWithoutRowidRejectedViaSession();
	void testLoadArchivedFalse();
	void testIdleFlush();
	void testCustomShardNamer();
	void testIndexPreservedAcrossReload();
	void testHistoryView();
	void testDeleteSealedShardLoaded();
	void testDeleteSealedShardUnloaded();
	void testDeleteActiveShardRefused();
	void testDeleteNonexistentShardRefused();
	void testShardDescriptors();
	void testRetentionByAge();
	void testRetentionByTotalBytes();
	void testDropOlderThan();
	void testDropToFit();
	void testRetentionDisabledIsNoop();
	void testHistoryViewTimeRange();
	void testHistoryViewTimeRangeOverflow();
	void testDetachAllArchived();
	void testShardCeilingAutoDrop();
	void testConcurrentAccess();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	std::string _dir;
};


#endif // MemoryDBTest_INCLUDED
