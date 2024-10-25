//
// MongoDBTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DateTime.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/OpMsgCursor.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/PoolableConnectionFactory.h"
#include "Poco/MongoDB/Binary.h"
#include "Poco/Net/NetException.h"
#include "Poco/UUIDGenerator.h"
#include "MongoDBTest.h"
#include <iostream>


using namespace Poco::MongoDB;


void MongoDBTest::testOpCmdUUID()
{
	Database db("team");
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("club");
	OpMsgMessage response;

	request->setCommandName(OpMsgMessage::CMD_DROP);
	_mongo->sendRequest(*request, response);

	Document::Ptr club = new Document();
	club->add("name", std::string("Barcelona"));

	Poco::UUIDGenerator generator;
	Poco::UUID uuid = generator.create();
	Binary::Ptr uuidBinary = new Binary(uuid);
	club->add("uuid", uuidBinary);

	request->setCommandName(OpMsgMessage::CMD_INSERT);
	request->documents().push_back(club);

	_mongo->sendRequest(*request, response);

	assertTrue(response.responseOk());

	request->setCommandName(OpMsgMessage::CMD_FIND);
	request->body().addNewDocument("filter").add("name", std::string("Barcelona"));

	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());

	if ( response.documents().size() > 0 )
	{
		Document::Ptr doc = response.documents()[0];
		try
		{
			const auto& name = doc->get<std::string>("name");
			assertEquals ("Barcelona", name );

			Binary::Ptr uuidBinary = doc->get<Binary::Ptr>("uuid");
			assertTrue (uuid == uuidBinary->uuid());
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


void MongoDBTest::testOpCmdHello()
{
	Database db("config");
	Poco::SharedPtr<OpMsgMessage> helloRequest = db.createOpMsgMessage();
	helloRequest->setCommandName(OpMsgMessage::CMD_HELLO);

	try
	{
		OpMsgMessage response;
		_mongo->sendRequest(*helloRequest, response);
		assertTrue(response.responseOk());
	}
	catch(Poco::NotImplementedException& nie)
	{
		std::cout << nie.message() << std::endl;
	}
}


void MongoDBTest::testOpCmdWriteRead()
{
	// Writes request to a stream and then reads it back
	// Tests send and read of a message with multiple sections without
	// the server.
	// NOTE: MongoDB 6.0 does not send responses with segments of type 1.

	Database db("abc");
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("col");
	request->setCommandName(OpMsgMessage::CMD_INSERT);

	Document::Ptr doc = new Document();
	doc->add("name", "John").add("number", -2);
	request->documents().push_back(doc);

	doc = new Document();
	doc->add("name", "Franz").add("number", -2.8);
	request->documents().push_back(doc);

	try
	{
		OpMsgMessage response;

		std::stringstream ss;
		request->send(ss);

		ss.seekg(0, std::ios_base::beg);
		response.read(ss);

		for (const auto& doc: response.documents())
		{
			std::cout << doc->toString(2);
		}
	}
	catch(Poco::NotImplementedException& nie)
	{
		std::cout << nie.message() << std::endl;
	}
}


void MongoDBTest::testOpCmdInsert()
{
	Document::Ptr player = new Document();
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

	Database db("team");
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(OpMsgMessage::CMD_INSERT);
	request->documents().push_back(player);

	try
	{
		OpMsgMessage response;
		_mongo->sendRequest(*request, response);
		
		assertTrue(response.responseOk());
	}
	catch(Poco::NotImplementedException& nie)
	{
		std::cout << nie.message() << std::endl;
	}
}

void MongoDBTest::testOpCmdFind()
{
	Database db("team");
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(OpMsgMessage::CMD_FIND);

	request->body().add("limit", 1).addNewDocument("filter").add("lastname" , std::string("Braem"));

	OpMsgMessage response;
	_mongo->sendRequest(*request, response);

	assertTrue(response.responseOk());

	if ( response.documents().size() > 0 )
	{
		Document::Ptr doc = response.documents()[0];

		try
		{
			const auto& lastname = doc->get<std::string>("lastname");
			assertEquals ("Braem", lastname);
			const auto& firstname = doc->get<std::string>("firstname");
			assertEquals ("Franky", firstname);
			const auto& birthDateTimestamp = doc->get<Poco::Timestamp>("birthdate");
			Poco::DateTime birthDate(birthDateTimestamp);
			assertTrue (birthDate.year() == 1969 && birthDate.month() == 3 && birthDate.day() == 9);
			const auto& lastupdatedTimestamp = doc->get<Poco::Timestamp>("lastupdated");
			assertTrue (doc->isType<NullValue>("unknown"));
			bool active = doc->get<bool>("active");
			assertEquals (false, active);

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


void MongoDBTest::testOpCmdUnaknowledgedInsert()
{
	Document::Ptr player = new Document();
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

	Database db("team");
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(OpMsgMessage::CMD_INSERT);
	request->setAcknowledgedRequest(false);
	request->documents().push_back(player);

	try
	{
		_mongo->sendRequest(*request);
	}
	catch(Poco::NotImplementedException& nie)
	{
		std::cout << nie.message() << std::endl;
	}
}


void MongoDBTest::testOpCmdCursor()
{
	Database db("team");

	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("numbers");
	OpMsgMessage response;

	request->setCommandName(OpMsgMessage::CMD_DROP);
	_mongo->sendRequest(*request, response);

	request->setCommandName(OpMsgMessage::CMD_INSERT);
	for(int i = 0; i < 10000; ++i)
	{
		Document::Ptr doc = new Document();
		doc->add("number", i);
		request->documents().push_back(doc);
	}
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());

	OpMsgCursor cursor("team", "numbers");
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);
	cursor.setBatchSize(1000);

	int n = 0;
	auto cresponse = cursor.next(*_mongo);
	while(true)
	{
		n += static_cast<int>(cresponse.documents().size());
		if ( cursor.cursorID() == 0 )
			break;
		cresponse = cursor.next(*_mongo);
	}
	assertEquals (10000, n);

	request->setCommandName(OpMsgMessage::CMD_DROP);
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());
}


void MongoDBTest::testOpCmdCursorAggregate()
{
	Database db("team");

	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("numbers");
	OpMsgMessage response;

	request->setCommandName(OpMsgMessage::CMD_DROP);
	_mongo->sendRequest(*request, response);

	request->setCommandName(OpMsgMessage::CMD_INSERT);
	for(int i = 0; i < 10000; ++i)
	{
		Document::Ptr doc = new Document();
		doc->add("number", i);
		request->documents().push_back(doc);
	}
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());

	Poco::SharedPtr<OpMsgCursor> cursor = db.createOpMsgCursor("numbers");
	cursor->query().setCommandName(OpMsgMessage::CMD_AGGREGATE);
	cursor->setBatchSize(1000);

	// Empty pipeline: get all documents
	cursor->query().body().addNewArray("pipeline");

	int n = 0;
	auto cresponse = cursor->next(*_mongo);
	while(true)
	{
		int batchDocSize = cresponse.documents().size();
		if (cursor->cursorID() != 0)
			assertEquals (1000, batchDocSize);

		n += batchDocSize;
		if ( cursor->cursorID() == 0 )
			break;
		cresponse = cursor->next(*_mongo);
	}
	assertEquals (10000, n);

	request->setCommandName(OpMsgMessage::CMD_DROP);
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());
}



void MongoDBTest::testOpCmdKillCursor()
{
	Database db("team");

	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("numbers");
	OpMsgMessage response;

	request->setCommandName(OpMsgMessage::CMD_DROP);
	_mongo->sendRequest(*request, response);

	request->setCommandName(OpMsgMessage::CMD_INSERT);
	for(int i = 0; i < 10000; ++i)
	{
		Document::Ptr doc = new Document();
		doc->add("number", i);
		request->documents().push_back(doc);
	}
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());

	OpMsgCursor cursor("team", "numbers");
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);
	cursor.setBatchSize(1000);

	int n = 0;
	auto cresponse = cursor.next(*_mongo);
	while(true)
	{
		n += static_cast<int>(cresponse.documents().size());
		if ( cursor.cursorID() == 0 )
			break;

		cursor.kill(*_mongo);
		cresponse = cursor.next(*_mongo);
	}
	assertEquals (1000, n);

	request->setCommandName(OpMsgMessage::CMD_DROP);

	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());

}

void MongoDBTest::testOpCmdCount()
{
	Database db("team");
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(OpMsgMessage::CMD_COUNT);

	OpMsgMessage response;
	_mongo->sendRequest(*request, response);

	assertTrue(response.responseOk());
	const auto& doc = response.body();
	assertEquals (1, doc.getInteger("n"));
}


void MongoDBTest::testOpCmdCursorEmptyFirstBatch()
{
	Database db("team");

	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("numbers");
	OpMsgMessage response;

	request->setCommandName(OpMsgMessage::CMD_DROP);
	_mongo->sendRequest(*request, response);

	request->setCommandName(OpMsgMessage::CMD_INSERT);
	for(int i = 0; i < 10000; ++i)
	{
		Document::Ptr doc = new Document();
		doc->add("number", i);
		request->documents().push_back(doc);
	}
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());

	Poco::SharedPtr<OpMsgCursor> cursor = db.createOpMsgCursor("numbers");
	cursor->query().setCommandName(OpMsgMessage::CMD_AGGREGATE);
	cursor->setEmptyFirstBatch(true);
	cursor->setBatchSize(0); // Will be ignored, default is used

	// Empty pipeline: get all documents
	cursor->query().body().addNewArray("pipeline");

	auto cresponse = cursor->next(*_mongo);
	assertEquals (0, cresponse.documents().size()); // First batch is empty

	int n = 0;
	while(true)
	{
		n += static_cast<int>(cresponse.documents().size());
		if ( cursor->cursorID() == 0 )
			break;
		cresponse = cursor->next(*_mongo);
	}
	assertEquals (10000, n);

	request->setCommandName(OpMsgMessage::CMD_DROP);
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());
}


void MongoDBTest::testOpCmdDelete()
{
	Database db("team");
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(OpMsgMessage::CMD_DELETE);

	Document::Ptr del = new Document();
	del->add("limit", 0).addNewDocument("q").add("lastname" , std::string("Braem"));
	request->documents().push_back(del);

	OpMsgMessage response;
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());
}

void MongoDBTest::testOpCmdConnectionPool()
{
#if POCO_OS == POCO_OS_ANDROID
		std::string host = "10.0.2.2";
#else
		std::string host = "127.0.0.1";
#endif

	Poco::Net::SocketAddress sa(host, 27017);
	Poco::PoolableObjectFactory<Connection, Connection::Ptr> factory(sa);
	Poco::ObjectPool<Connection, Connection::Ptr> pool(factory, 10, 15);

	PooledConnection pooledConnection(pool);

	Database db("team");
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(OpMsgMessage::CMD_COUNT);

	OpMsgMessage response;

	((Connection::Ptr) pooledConnection)->sendRequest(*request, response);
	assertTrue(response.responseOk());

	const auto& doc = response.body();
	assertEquals (1, doc.getInteger("n"));
}


void MongoDBTest::testOpCmdDropDatabase()
{
	Database db("team");
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage();
	request->setCommandName(OpMsgMessage::CMD_DROP_DATABASE);

	OpMsgMessage response;
	_mongo->sendRequest(*request, response);

	std::cout << request->body().toString(2) << std::endl;
	std::cout << response.body().toString(2) << std::endl;

	assertTrue(response.responseOk());
}



