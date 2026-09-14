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
#include "TestDocuments.h"

#include <iostream>
#include <sstream>
#include <tuple>


using namespace Poco::MongoDB;
using namespace std::string_literals;


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
	doc->add("name"s, "John").add("number", -2);
	request->documents().push_back(doc);

	doc = new Document();
	doc->add("name"s, "Franz").add("number", -2.8);
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
	player->add("lastname"s, "Braem"s);
	player->add("firstname"s, "Franky"s);

	Poco::DateTime birthdate;
	birthdate.assign(1969, 3, 9);
	player->add("birthdate"s, birthdate.timestamp());

	player->add("start"s, 1993);
	player->add("active"s, false);

	Poco::DateTime now;
	player->add("lastupdated"s, now.timestamp());

	player->add("unknown"s, NullValue());

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

	request->body().add("limit"s, 1).addNewDocument("filter").add("lastname"s, "Braem"s);

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
			(void) doc->get<Poco::Timestamp>("lastupdated");
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
	player->add("lastname"s, "Braem"s);
	player->add("firstname"s, "Franky"s);

	Poco::DateTime birthdate;
	birthdate.assign(1969, 3, 9);
	player->add("birthdate"s, birthdate.timestamp());

	player->add("start"s, 1993);
	player->add("active"s, false);

	Poco::DateTime now;
	player->add("lastupdated"s, now.timestamp());

	player->add("unknown"s, NullValue());

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
		doc->add("number"s, i);
		request->documents().push_back(doc);
	}
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());

	OpMsgCursor cursor("team", "numbers");
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);
	cursor.setBatchSize(1000);

	int n = 0;
	auto cresponse = cursor.next(*_mongo);
	while(cursor.isActive())
	{
		n += static_cast<int>(cresponse.documents().size());
		cresponse = cursor.next(*_mongo);
	}
	assertEquals (10000, n);
	assertFalse(cursor.isActive());

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
		doc->add("number"s, i);
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
	while(cursor->isActive())
	{
		int batchDocSize = cresponse.documents().size();
		if (cursor->cursorID() != 0)
			assertEquals (1000, batchDocSize);

		n += batchDocSize;
		cresponse = cursor->next(*_mongo);
	}
	assertEquals (10000, n);
	assertFalse(cursor->isActive());

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
		doc->add("number"s, i);
		request->documents().push_back(doc);
	}
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());

	OpMsgCursor cursor("team", "numbers");
	cursor.query().setCommandName(OpMsgMessage::CMD_FIND);
	cursor.setBatchSize(1000);

	int n = 0;
	auto cresponse = cursor.next(*_mongo);
	while(cursor.isActive())
	{
		n += static_cast<int>(cresponse.documents().size());
		cursor.kill(*_mongo);
		assertFalse(cursor.isActive());
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
		doc->add("number"s, i);
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
	del->add("limit"s, 0).addNewDocument("q").add("lastname"s, "Braem"s);
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


namespace
{
	const std::string largeRepliesDb {"largeReplies"s};

	void dropLargeRepliesDatabase(Connection& connection)
	{
		Database db(largeRepliesDb);
		Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage();
		request->setCommandName(OpMsgMessage::CMD_DROP_DATABASE);
		OpMsgMessage response;
		connection.sendRequest(*request, response);
	}

	void insertDocuments(Connection& connection, const std::string& collection, const Document::Vector& documents)
	{
		dropLargeRepliesDatabase(connection);
		Database db(largeRepliesDb);
		Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage(collection);
		request->setCommandName(OpMsgMessage::CMD_INSERT);
		request->documents() = documents;
		OpMsgMessage response;
		connection.sendRequest(*request, response);
		if (!response.responseOk() || response.body().exists("writeErrors"s))
			throw Poco::RuntimeException("insert failed: " + response.body().toString());
	}
}


void MongoDBTest::testOpCmdFindMaxSizeDocument()
{
	// The reply body of a find that returns a 16 MiB document is larger than 16 MiB.
	insertDocuments(*_mongo, "large"s, {sizedDocument(1, BSON_MAX_DOCUMENT_SIZE)});

	Database db(largeRepliesDb);
	Poco::SharedPtr<OpMsgMessage> find = db.createOpMsgMessage("large"s);
	find->setCommandName(OpMsgMessage::CMD_FIND);
	find->body().addNewDocument("filter"s).add("_id"s, 1);
	OpMsgMessage response;
	_mongo->sendRequest(*find, response);
	assertTrue(response.responseOk());
	assertEquals (1, response.documents().size());
	assertEquals (BSON_MAX_DOCUMENT_SIZE - 22, response.documents()[0]->get<Binary::Ptr>("p"s)->buffer().size());
	assertFalse(Database(largeRepliesDb).queryServerHello(*_mongo).isNull());

	dropLargeRepliesDatabase(*_mongo);
}


void MongoDBTest::testOpCmdFindShowRecordId()
{
	// showRecordId adds $recordId to the stored document, so the returned document exceeds 16 MiB.
	insertDocuments(*_mongo, "large"s, {sizedDocument(1, BSON_MAX_DOCUMENT_SIZE)});

	Database db(largeRepliesDb);
	Poco::SharedPtr<OpMsgMessage> find = db.createOpMsgMessage("large"s);
	find->setCommandName(OpMsgMessage::CMD_FIND);
	find->body().addNewDocument("filter"s).add("_id"s, 1);
	find->body().add("showRecordId"s, true);
	OpMsgMessage response;
	_mongo->sendRequest(*find, response);
	assertTrue(response.responseOk());
	assertEquals (1, response.documents().size());
	assertTrue(response.documents()[0]->exists("$recordId"s));

	dropLargeRepliesDatabase(*_mongo);
}


void MongoDBTest::testOpCmdAggregateOutputAbove16MB()
{
	// Aggregation can return documents of up to 16 MiB + 16 KiB.
	insertDocuments(*_mongo, "large"s, {sizedDocument(1, BSON_MAX_DOCUMENT_SIZE)});

	Database db(largeRepliesDb);
	Poco::SharedPtr<OpMsgMessage> aggregate = db.createOpMsgMessage("large"s);
	aggregate->setCommandName(OpMsgMessage::CMD_AGGREGATE);
	Array& pipeline = aggregate->body().addNewArray("pipeline"s);
	pipeline.addNewDocument("0"s).addNewDocument("$match"s).add("_id"s, 1);
	pipeline.addNewDocument("1"s).addNewDocument("$addFields"s).add("padding"s, std::string(1024, 'x'));
	aggregate->body().addNewDocument("cursor"s);
	OpMsgMessage response;
	_mongo->sendRequest(*aggregate, response);
	assertTrue(response.responseOk());
	assertEquals (1, response.documents().size());
	assertEquals (1024, response.documents()[0]->get<std::string>("padding"s).size());

	dropLargeRepliesDatabase(*_mongo);
}


void MongoDBTest::testOpCmdCursorLargeBatch()
{
	// The server fills a batch while the reply buffer stays within 16 MiB and then appends
	// the cursor id and namespace, so a getMore with both documents has a body above 16 MiB.
	// They leave 16 bytes to the limit of MongoDB 6.0 to 9.0; a server that counts more
	// overhead returns them in two batches.
	const std::string collection(100, 'c');
	insertDocuments(*_mongo, collection, {sizedDocument(0, 22), sizedDocument(1, 16777118)});

	Database db(largeRepliesDb);
	Poco::SharedPtr<OpMsgCursor> cursor = db.createOpMsgCursor(collection);
	cursor->query().setCommandName(OpMsgMessage::CMD_FIND);
	cursor->query().body().addNewDocument("sort"s).add("_id"s, 1);
	cursor->setEmptyFirstBatch(true);

	assertEquals (0, cursor->next(*_mongo).documents().size());
	std::size_t documents = 0;
	int batches = 0;
	do
	{
		const OpMsgMessage& batch = cursor->next(*_mongo);
		assertTrue(batch.responseOk());
		documents += batch.documents().size();
		++batches;
	}
	while (cursor->cursorID() != 0);
	assertEquals (2, documents);
	if (batches > 1)
		warnmsg("The server returned the documents in " + std::to_string(batches) + " batches: no reply above 16 MiB was read.");

	dropLargeRepliesDatabase(*_mongo);
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


void MongoDBTest::testOpCmdDropIndex()
{
	Database db("team");

	// Start from a clean collection.
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage("players");
	OpMsgMessage response;
	request->setCommandName(OpMsgMessage::CMD_DROP);
	_mongo->sendRequest(*request, response); // Ignore result: the collection may not exist yet.

	// Insert one document so the collection exists.
	request = db.createOpMsgMessage("players");
	request->setCommandName(OpMsgMessage::CMD_INSERT);
	Document::Ptr player = new Document();
	player->add("lastname"s, "Braem"s).add("firstname"s, "Franky"s);
	request->documents().push_back(player);
	_mongo->sendRequest(*request, response);
	assertTrue(response.responseOk());

	Database::IndexedFields lastnameField;
	lastnameField.push_back(std::make_tuple("lastname", true));
	Database::IndexedFields firstnameField;
	firstnameField.push_back(std::make_tuple("firstname", true));

	// 1. Drop by name.
	Document::Ptr result = db.createIndex(*_mongo, "players", lastnameField, "lastname_1");
	assertTrue(result->getInteger("ok") == 1);
	result = db.dropIndex(*_mongo, "players", "lastname_1");
	assertTrue(result->getInteger("ok") == 1);

	// 2. Drop by key specification.
	result = db.createIndex(*_mongo, "players", firstnameField, "firstname_1");
	assertTrue(result->getInteger("ok") == 1);
	result = db.dropIndex(*_mongo, "players", firstnameField);
	assertTrue(result->getInteger("ok") == 1);

	// 3. Drop all indexes: create two, then drop them at once.
	db.createIndex(*_mongo, "players", lastnameField, "lastname_1");
	db.createIndex(*_mongo, "players", firstnameField, "firstname_1");
	result = db.dropAllIndexes(*_mongo, "players");
	assertTrue(result->getInteger("ok") == 1);

	// Only the _id index remains.
	Poco::SharedPtr<OpMsgCursor> cursor = db.createOpMsgCursor("players");
	cursor->query().setCommandName(OpMsgMessage::CMD_LIST_INDEXES);
	auto listResponse = cursor->next(*_mongo);
	int indexCount = 0;
	while (true)
	{
		indexCount += static_cast<int>(listResponse.documents().size());
		if (cursor->cursorID() == 0) break;
		listResponse = cursor->next(*_mongo);
	}
	assertEquals (1, indexCount);

	// Cleanup.
	request = db.createOpMsgMessage("players");
	request->setCommandName(OpMsgMessage::CMD_DROP);
	_mongo->sendRequest(*request, response);
}



