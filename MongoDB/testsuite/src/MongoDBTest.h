//
// MongoDBTest.h
//
// Definition of the MongoDBTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDBTest_INCLUDED
#define MongoDBTest_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "CppUnit/TestCase.h"


class MongoDBTest: public CppUnit::TestCase
{
public:
	MongoDBTest(const std::string& name);

	virtual ~MongoDBTest();
	
	void setUp();
	void tearDown();

	void testObjectID();
	void testArray();
	void testBuildInfo();
	void testHello();
	void testConnectURI();

	// Old wire protocol
	void testInsertRequest();
	void testQueryRequest();
	void testDBQueryRequest();
	void testCountCommand();
	void testDBCountCommand();
	void testDBCount2Command();
	void testDeleteRequest();

	void testConnectionPool();
	void testCursorRequest();
	void testCommand();
	void testUUID();

	// New wire protocol using OP_CMD
	void testOpCmdUUID();
	void testOpCmdHello();
	void testOpCmdWriteRead();
	void testOpCmdInsert();
	void testOpCmdFind();
	void testOpCmdCursor();
	void testOpCmdCursorAggregate();
	void testOpCmdCursorEmptyFirstBatch();
	void testOpCmdKillCursor();
	void testOpCmdCount();
	void testOpCmdDelete();
	void testOpCmdUnaknowledgedInsert();
	void testOpCmdConnectionPool();
	void testOpCmdDropDatabase();

	static CppUnit::Test* suite();

private:
	static Poco::MongoDB::Connection::Ptr	_mongo;
	static Poco::Int64						_wireVersion;
};


#endif // MongoDBTest_INCLUDED
