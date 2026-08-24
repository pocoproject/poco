//
// MemoryDBInspectorTest.h
//
// Definition of the MemoryDBInspectorTest class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MemoryDBInspectorTest_INCLUDED
#define MemoryDBInspectorTest_INCLUDED


#include "Poco/Data/SQLite/SQLite.h"
#include "CppUnit/TestCase.h"
#include <string>


class MemoryDBInspectorTest: public CppUnit::TestCase
{
public:
	MemoryDBInspectorTest(const std::string& name);
	~MemoryDBInspectorTest();

	// status
	void testStatusFresh();
	void testStatusShardsAndBytes();
	void testStatusAttachedFlag();

	// schema
	void testSchemaTablesColumnsIndexes();
	void testSchemaVirtualTables();
	void testSchemaRowCountsOmittedByDefault();
	void testSchemaSpansAttachedShards();
	void testRowCountOnDemand();
	void testRowCountUnknown();

	// query
	void testQuerySelect();
	void testQueryNullAndBlob();
	void testQueryTruncation();
	void testQueryErrorReported();

	// policy
	void testReadOnlyRejectsWrites();
	void testReadOnlyCommentSmuggling();
	void testReadOnlyCTEWrite();
	void testReadOnlyMultiStatement();
	void testReadOnlyPragmaAndExplain();
	void testPolicyIndependentOfSqlParseFeature();
	void testQueryOnlyNeverSet();
	void testCheckSQLAllowedThrows();
	void testAllowWritesPolicy();

	// JSON
	void testJSONWellFormed();
	void testJSONEscaping();
	void testJSONNumbers();

	// management
	void testActionVerbs();
	void testActionPolicyGate();
	void testFlushReportsCompletion();

	// concurrency
	void testConcurrentOwnerWritesWhileInspectorReads();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	std::string _dir;
};


#endif // MemoryDBInspectorTest_INCLUDED
