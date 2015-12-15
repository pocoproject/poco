//
// MongoDBTest.cpp
//
// $Id$
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
#include <iostream>

#include "Poco/DateTime.h"
#include "Poco/ObjectPool.h"

#include "Poco/MongoDB/InsertRequest.h"
#include "Poco/MongoDB/QueryRequest.h"
#include "Poco/MongoDB/DeleteRequest.h"
#include "Poco/MongoDB/GetMoreRequest.h"
#include "Poco/MongoDB/PoolableConnectionFactory.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Cursor.h"
#include "Poco/MongoDB/ObjectId.h"
#include "Poco/MongoDB/Binary.h"

#include "Poco/Net/NetException.h"
#include "Poco/UUIDGenerator.h"

#include "MongoDBTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"

using namespace Poco::MongoDB;

Poco::MongoDB::Connection::Ptr MongoDBTest::_mongo;


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


void MongoDBTest::testInsertRequest()
{
	Poco::MongoDB::Document::Ptr player = new Poco::MongoDB::Document();
	player->add("lastname", std::string("Braem"));
	player->add("firstname", std::string("Franky"));

	Poco::DateTime birthdate;
	birthdate.assign(1969, 3, 9);
	player->add("birthdate", birthdate.timestamp());

	player->add("start", 1993);
	player->add("active", false);

	Poco::DateTime now;
	player->add("lastupdated", now.timestamp());

	player->add("unknown", NullValue());

	Poco::MongoDB::InsertRequest request("team.players");
	request.documents().push_back(player);
	_mongo->sendRequest(request);
}

void MongoDBTest::testQueryRequest()
{
	Poco::MongoDB::QueryRequest request("team.players");
	request.selector().add("lastname" , std::string("Braem"));
	request.setNumberToReturn(1);

	Poco::MongoDB::ResponseMessage response;

	_mongo->sendRequest(request, response);

	if ( response.documents().size() > 0 )
	{
		Poco::MongoDB::Document::Ptr doc = response.documents()[0];

		try
		{
			std::string lastname = doc->get<std::string>("lastname");
			assert(lastname.compare("Braem") == 0);
			std::string firstname = doc->get<std::string>("firstname");
			assert(firstname.compare("Franky") == 0);
			Poco::Timestamp birthDateTimestamp = doc->get<Poco::Timestamp>("birthdate");
			Poco::DateTime birthDate(birthDateTimestamp);
			assert(birthDate.year() == 1969 && birthDate.month() == 3 && birthDate.day() == 9);
			Poco::Timestamp lastupdatedTimestamp = doc->get<Poco::Timestamp>("lastupdated");
			assert(doc->isType<NullValue>("unknown"));
			bool active = doc->get<bool>("active");
			assert(!active);

			std::string id = doc->get("_id")->toString();
		}
		catch(Poco::NotFoundException& nfe)
		{
			fail(nfe.message() + " not found.");
		}
	}
	else
	{
		fail("No document returned");
	}
}

void MongoDBTest::testDBQueryRequest()
{
	Database db("team");
	Poco::SharedPtr<Poco::MongoDB::QueryRequest> request = db.createQueryRequest("players");
	request->selector().add("lastname" , std::string("Braem"));

	Poco::MongoDB::ResponseMessage response;
	_mongo->sendRequest(*request, response);

	if ( response.documents().size() > 0 )
	{
		Poco::MongoDB::Document::Ptr doc = response.documents()[0];

		try
		{
			std::string lastname = doc->get<std::string>("lastname");
			assert(lastname.compare("Braem") == 0);
			std::string firstname = doc->get<std::string>("firstname");
			assert(firstname.compare("Franky") == 0);
			Poco::Timestamp birthDateTimestamp = doc->get<Poco::Timestamp>("birthdate");
			Poco::DateTime birthDate(birthDateTimestamp);
			assert(birthDate.year() == 1969 && birthDate.month() == 3 && birthDate.day() == 9);
			Poco::Timestamp lastupdatedTimestamp = doc->get<Poco::Timestamp>("lastupdated");
			assert(doc->isType<NullValue>("unknown"));

			std::string id = doc->get("_id")->toString();
		}
		catch(Poco::NotFoundException& nfe)
		{
			fail(nfe.message() + " not found.");
		}
	}
	else
	{
		fail("No document returned");
	}
}


void MongoDBTest::testCountCommand()
{
	Poco::MongoDB::QueryRequest request("team.$cmd");
	request.setNumberToReturn(1);
	request.selector().add("count", std::string("players"));

	Poco::MongoDB::ResponseMessage response;

	_mongo->sendRequest(request, response);

	if ( response.documents().size() > 0 )
	{
		Poco::MongoDB::Document::Ptr doc = response.documents()[0];
		assert(doc->getInteger("n") == 1);
	}
	else
	{
		fail("Didn't get a response from the count command");
	}
}


void MongoDBTest::testDBCountCommand()
{
	Poco::MongoDB::Database db("team");
	Poco::SharedPtr<Poco::MongoDB::QueryRequest> request = db.createCountRequest("players");

	Poco::MongoDB::ResponseMessage response;
	_mongo->sendRequest(*request, response);

	if ( response.documents().size() > 0 )
	{
		Poco::MongoDB::Document::Ptr doc = response.documents()[0];
		assert(doc->getInteger("n") == 1);
	}
	else
	{
		fail("Didn't get a response from the count command");
	}
}


void MongoDBTest::testDBCount2Command()
{
	Poco::MongoDB::Database db("team");
	Poco::Int64 count = db.count(*_mongo, "players");
	assert(count == 1);
}


void MongoDBTest::testDeleteRequest()
{
	Poco::MongoDB::DeleteRequest request("team.players");
	request.selector().add("lastname", std::string("Braem"));

	_mongo->sendRequest(request);
}


void MongoDBTest::testCursorRequest()
{
	Poco::MongoDB::Database db("team");
	Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertRequest = db.createInsertRequest("numbers");
	for(int i = 0; i < 10000; ++i)
	{
		Document::Ptr doc = new Document();
		doc->add("number", i);
		insertRequest->documents().push_back(doc);
	}
	_mongo->sendRequest(*insertRequest);

	Poco::Int64 count = db.count(*_mongo, "numbers");
	assert(count == 10000);

	Poco::MongoDB::Cursor cursor("team", "numbers");

	int n = 0;
	Poco::MongoDB::ResponseMessage& response = cursor.next(*_mongo);
	while(1)
	{
		n += response.documents().size();
		if ( response.cursorID() == 0 )
			break;
		response = cursor.next(*_mongo);
	}
	assert(n == 10000);

	Poco::MongoDB::QueryRequest drop("team.$cmd");
	drop.setNumberToReturn(1);
	drop.selector().add("drop", std::string("numbers"));

	Poco::MongoDB::ResponseMessage responseDrop;
	_mongo->sendRequest(drop, responseDrop);
}


void MongoDBTest::testBuildInfo()
{
	Poco::MongoDB::QueryRequest request("team.$cmd");
	request.setNumberToReturn(1);
	request.selector().add("buildInfo", 1);

	Poco::MongoDB::ResponseMessage response;

	try
	{
		_mongo->sendRequest(request, response);
	}
	catch(Poco::NotImplementedException& nie)
	{
		std::cout << nie.message() << std::endl;
		return;
	}

	if ( response.documents().size() > 0 )
	{
		Poco::MongoDB::Document::Ptr doc = response.documents()[0];
		std::cout << doc->toString(2);
	}
	else
	{
		fail("Didn't get a response from the buildinfo command");
	}
}


void MongoDBTest::testConnectionPool()
{
	Poco::Net::SocketAddress sa("localhost", 27017);
	Poco::PoolableObjectFactory<Poco::MongoDB::Connection, Poco::MongoDB::Connection::Ptr> factory(sa);
	Poco::ObjectPool<Poco::MongoDB::Connection, Poco::MongoDB::Connection::Ptr> pool(factory, 10, 15);

	Poco::MongoDB::PooledConnection pooledConnection(pool);

	Poco::MongoDB::QueryRequest request("team.$cmd");
	request.setNumberToReturn(1);
	request.selector().add("count", std::string("players"));

	Poco::MongoDB::ResponseMessage response;
	((Connection::Ptr) pooledConnection)->sendRequest(request, response);

	if ( response.documents().size() > 0 )
	{
		Poco::MongoDB::Document::Ptr doc = response.documents()[0];
		assert(doc->getInteger("n") == 1);
	}
	else
	{
		fail("Didn't get a response from the count command");
	}
}


void MongoDBTest::testObjectID()
{
	ObjectId oid("536aeebba081de6815000002");
	std::string str2 = oid.toString();
	assert(str2 == "536aeebba081de6815000002");
}


void MongoDBTest::testCommand() {
	Poco::MongoDB::Database db("team");
	Poco::SharedPtr<Poco::MongoDB::QueryRequest> command = db.createCommand();
	command->selector().add("create", "fixCol")
		.add("capped", true)
		.add("max", 1024*1024)
		.add("size", 1024);

	Poco::MongoDB::ResponseMessage response;
	_mongo->sendRequest(*command, response);
	if ( response.documents().size() > 0 )
	{
		Poco::MongoDB::Document::Ptr doc = response.documents()[0];
	}
	else
	{
		Poco::MongoDB::Document::Ptr lastError = db.getLastErrorDoc(*_mongo);
		fail(lastError->toString(2));
	}
}

void MongoDBTest::testUUID()
{
	Poco::MongoDB::Document::Ptr club = new Poco::MongoDB::Document();
	club->add("name", std::string("Barcelona"));

	Poco::UUIDGenerator generator;
	Poco::UUID uuid = generator.create();
	Poco::MongoDB::Binary::Ptr uuidBinary = new Poco::MongoDB::Binary(uuid);
	club->add("uuid", uuidBinary);

	Poco::MongoDB::InsertRequest request("team.club");
	request.documents().push_back(club);

	_mongo->sendRequest(request);

	Poco::MongoDB::QueryRequest queryReq("team.club");
	queryReq.selector().add("name" , std::string("Barcelona"));

	Poco::MongoDB::ResponseMessage response;
	_mongo->sendRequest(queryReq, response);

	if ( response.documents().size() > 0 )
	{
		Poco::MongoDB::Document::Ptr doc = response.documents()[0];

		try
		{
			std::string name = doc->get<std::string>("name");
			assert(name.compare("Barcelona") == 0);

			Poco::MongoDB::Binary::Ptr uuidBinary = doc->get<Binary::Ptr>("uuid");
			assert(uuid == uuidBinary->uuid());
		}
		catch(Poco::NotFoundException& nfe)
		{
			fail(nfe.message() + " not found.");
		}
	}
	else
	{
		fail("No document returned");
	}

	Poco::MongoDB::DeleteRequest delRequest("team.club");
	delRequest.selector().add("name", std::string("Barcelona"));
	_mongo->sendRequest(delRequest);
}


CppUnit::Test* MongoDBTest::suite()
{
	try
	{
		_mongo = new Poco::MongoDB::Connection("localhost", 27017);
		std::cout << "Connected to [localhost:27017]" << std::endl;
	}
	catch (Poco::Net::ConnectionRefusedException& e)
	{
		std::cout << "Couldn't connect to " << e.message() << ". " << std::endl;
		return 0;
	}

	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MongoDBTest");

	CppUnit_addTest(pSuite, MongoDBTest, testBuildInfo);
	CppUnit_addTest(pSuite, MongoDBTest, testInsertRequest);
	CppUnit_addTest(pSuite, MongoDBTest, testQueryRequest);
	CppUnit_addTest(pSuite, MongoDBTest, testDBQueryRequest);
	CppUnit_addTest(pSuite, MongoDBTest, testCountCommand);
	CppUnit_addTest(pSuite, MongoDBTest, testDBCountCommand);
	CppUnit_addTest(pSuite, MongoDBTest, testDBCount2Command);
	CppUnit_addTest(pSuite, MongoDBTest, testConnectionPool);
	CppUnit_addTest(pSuite, MongoDBTest, testDeleteRequest);
	CppUnit_addTest(pSuite, MongoDBTest, testCursorRequest);
	CppUnit_addTest(pSuite, MongoDBTest, testObjectID);
	CppUnit_addTest(pSuite, MongoDBTest, testCommand);
	CppUnit_addTest(pSuite, MongoDBTest, testUUID);

	return pSuite;
}
