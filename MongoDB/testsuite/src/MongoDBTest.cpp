//
// MongoDBTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DateTime.h"
#include "Poco/ObjectPool.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/PoolableConnectionFactory.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/OpMsgCursor.h"
#include "Poco/MongoDB/ObjectId.h"
#include "Poco/MongoDB/Binary.h"
#include "Poco/Net/NetException.h"
#include "Poco/UUIDGenerator.h"
#include "MongoDBTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <iostream>


using namespace Poco::MongoDB;
using namespace std::string_literals;


Poco::MongoDB::Connection::Ptr MongoDBTest::_mongo;
Poco::Int64 MongoDBTest::_wireVersion {0};


MongoDBTest::MongoDBTest(const std::string& name):
	CppUnit::TestCase("MongoDB")
{
}


MongoDBTest::~MongoDBTest()
{
}


void MongoDBTest::setUp()
{
}


void MongoDBTest::tearDown()
{
}


void MongoDBTest::testBuildInfo()
{
	Poco::MongoDB::Database db("config");
	try
	{
		Poco::MongoDB::Document::Ptr doc = db.queryBuildInfo(*_mongo);
		std::cout << doc->toString(2);
	}
	catch(Poco::NotImplementedException& nie)
	{
		std::cout << nie.message() << std::endl;
	}
}

void MongoDBTest::testHello()
{
	Poco::MongoDB::Database db("config");
	try
	{
		Poco::MongoDB::Document::Ptr doc = db.queryServerHello(*_mongo);
		std::cout << doc->toString(2);
	}
	catch(Poco::NotImplementedException& nie)
	{
		std::cout << nie.message() << std::endl;
	}
}


void MongoDBTest::testObjectID()
{
	ObjectId oid("536aeebba081de6815000002");
	std::string str2 = oid.toString();
	assertTrue (str2 == "536aeebba081de6815000002");
}


void MongoDBTest::testConnectURI()
{
	Poco::MongoDB::Connection conn;
	Poco::MongoDB::Connection::SocketFactory sf;

#if POCO_OS == POCO_OS_ANDROID
		std::string host = "10.0.2.2";
#else
		std::string host = "127.0.0.1";
#endif

	conn.connect("mongodb://" + host, sf);
	conn.disconnect();

	try
	{
		conn.connect("http://" + host, sf);
		fail("invalid URI scheme - must throw");
	}
	catch (Poco::UnknownURISchemeException&)
	{
	}

	try
	{
		conn.connect("mongodb://" + host + "?ssl=true", sf);
		fail("SSL not supported, must throw");
	}
	catch (Poco::NotImplementedException&)
	{
	}

	conn.connect("mongodb://" + host + "/admin?ssl=false&connectTimeoutMS=10000&socketTimeoutMS=10000", sf);
	conn.disconnect();

	try
	{
		conn.connect("mongodb://" + host + "/admin?connectTimeoutMS=foo", sf);
		fail("invalid parameter - must throw");
	}
	catch (Poco::Exception&)
	{
	}

#ifdef MONGODB_TEST_AUTH
	conn.connect("mongodb://admin:admin@127.0.0.1/admin", sf);
	conn.disconnect();
#endif
}


void MongoDBTest::testDBCount()
{
	// First insert some documents
	Database db("team");
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(OpMsgMessage::CMD_INSERT);

	Document::Ptr player = new Document();
	player->add("lastname"s, "TestPlayer"s);
	player->add("firstname"s, "Test"s);
	request->documents().push_back(player);

	OpMsgMessage response;
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());

	// Now test the count method
	Poco::Int64 count = db.count(*_mongo, "players");
	assertTrue (count >= 1);

	// Cleanup
	request = db.createOpMsgMessage("players");
	request->setCommandName(OpMsgMessage::CMD_DELETE);
	Document::Ptr del = new Document();
	del->add("limit"s, 0).addNewDocument("q").add("lastname"s, "TestPlayer"s);
	request->documents().push_back(del);
	_mongo->sendRequest(*request, response);
}


CppUnit::Test* MongoDBTest::suite()
{
#if POCO_OS == POCO_OS_ANDROID
		std::string host = "10.0.2.2";
#else
		std::string host = "127.0.0.1";
#endif
	try
	{
		_wireVersion = 0;
		_mongo = new Poco::MongoDB::Connection(host, 27017);
		std::cout << "Connected to [" << host << ":27017]" << std::endl;

		Poco::MongoDB::Database db("config");
		Poco::MongoDB::Document::Ptr doc = db.queryServerHello(*_mongo);
		_wireVersion = doc->getInteger("maxWireVersion");
		std::cout << "MongoDB wire version: " << _wireVersion << std::endl;
	}
	catch (Poco::Net::ConnectionRefusedException& e)
	{
		std::cout << "Couldn't connect to " << e.message() << ". " << std::endl;
		return nullptr;
	}
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MongoDBTest");

	CppUnit_addTest(pSuite, MongoDBTest, testObjectID);
	CppUnit_addTest(pSuite, MongoDBTest, testConnectURI);
	CppUnit_addTest(pSuite, MongoDBTest, testHello);
	CppUnit_addTest(pSuite, MongoDBTest, testBuildInfo);

	if (_wireVersion >= Poco::MongoDB::Database::VER_36)
	{
		// Database supports OP_MSG wire protocol
		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdWriteRead);
		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdHello);

		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdInsert);
		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdFind);
		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdCount);
		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdConnectionPool);

		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdDelete);
		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdUnaknowledgedInsert);
		
		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdCursor);
		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdCursorAggregate);
		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdKillCursor);
		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdCursorEmptyFirstBatch);

		CppUnit_addTest(pSuite, MongoDBTest, testDBCount);

		CppUnit_addTest(pSuite, MongoDBTest, testOpCmdDropDatabase);		
	}

	return pSuite;
}
