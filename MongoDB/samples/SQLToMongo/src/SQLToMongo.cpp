//
// SQLToMongo.cpp
//
// This sample demonstrates basic MongoDB operations using the Poco::MongoDB library
// with the OP_MSG wire protocol. Each function shows how to translate common SQL
// operations to MongoDB commands.
//
// The examples use a "players" collection to demonstrate:
// - INSERT operations (inserting documents)
// - SELECT operations (querying with filters, projections, sorting, limits)
// - UPDATE operations (modifying documents)
// - DELETE operations (removing documents)
// - Creating indexes
// - Distinct queries and aggregations
//
// Copyright (c) 2013-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/OpMsgCursor.h"
#include "Poco/MongoDB/Array.h"


// SQL: INSERT INTO players VALUES ("Valdes", "Victor", 1982), ...
// MongoDB: db.players.insertMany([{...}, {...}])
//
// This sample demonstrates inserting multiple documents at once.
void sample1(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 1: INSERT multiple documents ***" << std::endl;

	Poco::MongoDB::Database db("sample");
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(Poco::MongoDB::OpMsgMessage::CMD_INSERT);

	// Add multiple documents
	auto& docs = request->documents();

	Poco::MongoDB::Document::Ptr doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Valdes").add("firstname", "Victor").add("birthyear", 1982);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Alves").add("firstname", "Daniel").add("birthyear", 1983);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Bartra").add("firstname", "Marc").add("birthyear", 1991);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Alba").add("firstname", "Jordi").add("birthyear", 1989);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Montoya").add("firstname", "Martin").add("birthyear", 1991);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Abidal").add("firstname", "Eric").add("birthyear", 1979);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Fontas").add("firstname", "Andreu").add("birthyear", 1989);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Messi").add("firstname", "Lionel").add("birthyear", 1987);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Puyol").add("firstname", "Carles").add("birthyear", 1978);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Piqué").add("firstname", "Gerard").add("birthyear", 1987);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Muniesa").add("firstname", "Marc").add("birthyear", 1992);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Fabrégas").add("firstname", "Cesc").add("birthyear", 1987);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Hernandez").add("firstname", "Xavi").add("birthyear", 1980);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Iniesta").add("firstname", "Andres").add("birthyear", 1984);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Alcantara").add("firstname", "Thiago").add("birthyear", 1991);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Dos Santos").add("firstname", "Jonathan").add("birthyear", 1990);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Mascherano").add("firstname", "Javier").add("birthyear", 1984);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Busquets").add("firstname", "Sergio").add("birthyear", 1988);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Adriano").add("firstname", "").add("birthyear", 1984);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Song").add("firstname", "Alex").add("birthyear", 1987);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Villa").add("firstname", "David").add("birthyear", 1981);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Sanchez").add("firstname", "Alexis").add("birthyear", 1988);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Pedro").add("firstname", "").add("birthyear", 1987);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Cuenca").add("firstname", "Isaac").add("birthyear", 1991);
	docs.push_back(doc);

	doc = new Poco::MongoDB::Document();
	doc->add("lastname", "Tello").add("firstname", "Cristian").add("birthyear", 1991);
	docs.push_back(doc);

	std::cout << "Inserting " << request->documents().size() << " player documents..." << std::endl;

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);
	if (!response.responseOk())
	{
		std::cout << "Error: " << response.body().toString(2) << std::endl;
	}
	else
	{
		std::cout << "Successfully inserted " << request->documents().size() << " documents" << std::endl;
	}
	std::cout << std::endl;
}


// SQL: SELECT lastname, birthyear FROM players
// MongoDB: db.players.find({}, {lastname: 1, birthyear: 1})
//
// This sample demonstrates projection (selecting specific fields).
void sample2(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 2: SELECT with projection (specific fields) ***" << std::endl;

	Poco::MongoDB::OpMsgCursor cursor("sample", "players");
	cursor.query().setCommandName(Poco::MongoDB::OpMsgMessage::CMD_FIND);
	// Selecting fields is done by adding a projection document
	cursor.query().body().addNewDocument("projection")
		.add("lastname", 1)
		.add("birthyear", 1);

	Poco::MongoDB::OpMsgMessage& response = cursor.next(connection);
	while (cursor.isActive())
	{
		for (const auto& doc : response.documents())
		{
			std::cout << doc->get<std::string>("lastname") << " (" << doc->get<int>("birthyear") << ')' << std::endl;
		}
		response = cursor.next(connection);
	}
	std::cout << std::endl;
}


// SQL: SELECT * FROM players
// MongoDB: db.players.find({})
//
// This sample demonstrates querying all fields from all documents.
void sample3(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 3: SELECT * (all fields) ***" << std::endl;

	Poco::MongoDB::OpMsgCursor cursor("sample", "players");
	cursor.query().setCommandName(Poco::MongoDB::OpMsgMessage::CMD_FIND);

	Poco::MongoDB::OpMsgMessage& response = cursor.next(connection);
	while (cursor.isActive())
	{
		for (const auto& doc : response.documents())
		{
			std::cout << doc->get<std::string>("lastname") << ' ' << doc->get<std::string>("firstname") << " (" << doc->get<int>("birthyear") << ')' << std::endl;
		}
		response = cursor.next(connection);
	}
	std::cout << std::endl;
}


// SQL: SELECT * FROM players WHERE birthyear = 1978
// MongoDB: db.players.find({birthyear: 1978})
//
// This sample demonstrates filtering with a WHERE clause (exact match).
void sample4(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 4: SELECT with WHERE clause (filter) ***" << std::endl;

	Poco::MongoDB::OpMsgCursor cursor("sample", "players");
	cursor.query().setCommandName(Poco::MongoDB::OpMsgMessage::CMD_FIND);
	cursor.query().body().addNewDocument("filter").add("birthyear", 1978);

	Poco::MongoDB::OpMsgMessage& response = cursor.next(connection);
	while (cursor.isActive())
	{
		for (const auto& doc : response.documents())
		{
			std::cout << doc->get<std::string>("lastname") << ' ' << doc->get<std::string>("firstname") << " (" << doc->get<int>("birthyear") << ')' << std::endl;
		}
		response = cursor.next(connection);
	}
	std::cout << std::endl;
}


// SQL: SELECT * FROM players WHERE birthyear = 1987 ORDER BY lastname
// MongoDB: db.players.find({birthyear: 1987}).sort({lastname: 1})
//
// This sample demonstrates sorting results with ORDER BY.
void sample5(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 5: SELECT with ORDER BY (sorting) ***" << std::endl;

	Poco::MongoDB::OpMsgCursor cursor("sample", "players");
	cursor.query().setCommandName(Poco::MongoDB::OpMsgMessage::CMD_FIND);
	cursor.query().body().addNewDocument("filter").add("birthyear", 1987);
	cursor.query().body().addNewDocument("sort").add("lastname", 1);

	Poco::MongoDB::OpMsgMessage& response = cursor.next(connection);
	while (cursor.isActive())
	{
		for (const auto& doc : response.documents())
		{
			std::cout << doc->get<std::string>("lastname") << ' ' << doc->get<std::string>("firstname") << " (" << doc->get<int>("birthyear") << ')' << std::endl;
		}
		response = cursor.next(connection);
	}
	std::cout << std::endl;
}


// SQL: SELECT * FROM players WHERE birthyear > 1969 AND birthyear <= 1980
// MongoDB: db.players.find({birthyear: {$gt: 1969, $lte: 1980}})
//
// This sample demonstrates range queries with comparison operators.
void sample6(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 6: SELECT with range query ($gt, $lte) ***" << std::endl;

	Poco::MongoDB::OpMsgCursor cursor("sample", "players");
	cursor.query().setCommandName(Poco::MongoDB::OpMsgMessage::CMD_FIND);
	cursor.query().body().addNewDocument("filter")
		.addNewDocument("birthyear")
			.add("$gt", 1969)
			.add("$lte", 1980);

	Poco::MongoDB::OpMsgMessage& response = cursor.next(connection);
	while (cursor.isActive())
	{
		for (const auto& doc : response.documents())
		{
			std::cout << doc->get<std::string>("lastname") << ' ' << doc->get<std::string>("firstname") << " (" << doc->get<int>("birthyear") << ')' << std::endl;
		}
		response = cursor.next(connection);
	}
	std::cout << std::endl;
}


// SQL: CREATE INDEX playername ON players(lastname)
// MongoDB: db.players.createIndex({lastname: 1})
//
// This sample demonstrates creating an index on a collection.
void sample7(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 7: CREATE INDEX ***" << std::endl;

	Poco::MongoDB::Database db("sample");

	// Create index on lastname field (ascending: true)
	Poco::MongoDB::Database::IndexedFields indexedFields;
	indexedFields.push_back(std::make_tuple("lastname", true));  // true = ascending

	Poco::MongoDB::Document::Ptr resultDoc = db.createIndex(
		connection,
		"players",           // collection name
		indexedFields,       // fields to index
		"lastname_idx"       // index name
	);

	std::cout << "Index created: " << resultDoc->toString(2) << std::endl;
	std::cout << std::endl;
}


// SQL: SELECT * FROM players LIMIT 10 OFFSET 20
// MongoDB: db.players.find({}).skip(20).limit(10)
//
// This sample demonstrates pagination with LIMIT and SKIP.
void sample8(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 8: SELECT with LIMIT and SKIP (pagination) ***" << std::endl;

	Poco::MongoDB::OpMsgCursor cursor("sample", "players");
	cursor.query().setCommandName(Poco::MongoDB::OpMsgMessage::CMD_FIND);
	cursor.query().body()
		.add("limit", 10)
		.add("skip", 20);

	Poco::MongoDB::OpMsgMessage& response = cursor.next(connection);
	while (cursor.isActive())
	{
		for (const auto& doc : response.documents())
		{
			std::cout << doc->get<std::string>("lastname") << ' ' << doc->get<std::string>("firstname") << " (" << doc->get<int>("birthyear") << ')' << std::endl;
		}
		response = cursor.next(connection);
	}
	std::cout << std::endl;
}


// SQL: SELECT * FROM players LIMIT 1
// MongoDB: db.players.findOne({})
//
// This sample demonstrates fetching a single document.
void sample9(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 9: SELECT LIMIT 1 (single document) ***" << std::endl;

	Poco::MongoDB::Database db("sample");
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(Poco::MongoDB::OpMsgMessage::CMD_FIND);
	request->body().add("limit", 1);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);
	if (!response.documents().empty())
	{
		std::cout << "First document:" << std::endl;
		std::cout << response.documents()[0]->toString(2) << std::endl;
	}
	std::cout << std::endl;
}


// SQL: SELECT DISTINCT birthyear FROM players WHERE birthyear > 1980
// MongoDB: db.players.distinct("birthyear", {birthyear: {$gt: 1980}})
//
// This sample demonstrates the distinct command with a filter.
void sample10(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 10: SELECT DISTINCT with WHERE ***" << std::endl;

	Poco::MongoDB::Database db("sample");
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(Poco::MongoDB::OpMsgMessage::CMD_DISTINCT);
	request->body()
		.add("key", "birthyear")
		.addNewDocument("query")
			.addNewDocument("birthyear")
				.add("$gt", 1980);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);
	if (response.responseOk())
	{
		Poco::MongoDB::Array::Ptr values = response.body().get<Poco::MongoDB::Array::Ptr>("values");
		std::cout << "Distinct birthyears (> 1980): ";
		for (std::size_t i = 0; i < values->size(); ++i)
		{
			if (i > 0) std::cout << ", ";
			std::cout << values->get<int>(i);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


// SQL: SELECT COUNT(*) FROM players WHERE birthyear > 1980
// MongoDB: db.players.countDocuments({birthyear: {$gt: 1980}})
//
// This sample demonstrates the count command with a filter.
void sample11(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 11: SELECT COUNT with WHERE ***" << std::endl;

	Poco::MongoDB::Database db("sample");
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(Poco::MongoDB::OpMsgMessage::CMD_COUNT);
	request->body()
		.addNewDocument("query")
			.addNewDocument("birthyear")
				.add("$gt", 1980);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);

	if (response.responseOk())
	{
		std::cout << "Count of players born after 1980: " << response.body().getInteger("n") << std::endl;
	}
	std::cout << std::endl;
}


// SQL: UPDATE players SET birthyear = birthyear + 1 WHERE firstname = 'Victor'
// MongoDB: db.players.updateMany({firstname: "Victor"}, {$inc: {birthyear: 1}})
//
// This sample demonstrates the update command with the $inc operator.
void sample12(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 12: UPDATE with increment operator ***" << std::endl;

	Poco::MongoDB::Database db("sample");
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(Poco::MongoDB::OpMsgMessage::CMD_UPDATE);

	Poco::MongoDB::Document::Ptr update = new Poco::MongoDB::Document();
	update->addNewDocument("q").add("firstname", "Victor");
	update->addNewDocument("u").addNewDocument("$inc").add("birthyear", 1);
	request->documents().push_back(update);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);

	std::cout << "Update response: " << response.body().toString(2) << std::endl;
	std::cout << std::endl;
}


// SQL: DELETE FROM players WHERE firstname = 'Victor'
// MongoDB: db.players.deleteMany({firstname: "Victor"})
//
// This sample demonstrates the delete command.
void sample13(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 13: DELETE ***" << std::endl;

	Poco::MongoDB::Database db("sample");
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(Poco::MongoDB::OpMsgMessage::CMD_DELETE);

	Poco::MongoDB::Document::Ptr del = new Poco::MongoDB::Document();
	del->addNewDocument("q").add("firstname", "Victor");
	del->add("limit", 0);  // 0 = delete all matching
	request->documents().push_back(del);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);

	std::cout << "Delete response: " << response.body().toString(2) << std::endl;
	std::cout << std::endl;
}


int main(int argc, char** argv)
{
	// Connect to MongoDB server
	// For replica set connections, see the ReplicaSet samples
	Poco::MongoDB::Connection connection("localhost", 27017);

	try
	{
		std::cout << "==================================================" << std::endl;
		std::cout << "Poco::MongoDB SQL to MongoDB Examples" << std::endl;
		std::cout << "==================================================" << std::endl;
		std::cout << std::endl;
		std::cout << "This sample demonstrates how to translate SQL" << std::endl;
		std::cout << "operations to MongoDB using the OP_MSG protocol." << std::endl;
		std::cout << std::endl;
		std::cout << "Connected to: localhost:27017" << std::endl;
		std::cout << "Database: sample" << std::endl;
		std::cout << "Collection: players" << std::endl;
		std::cout << std::endl;

		sample1(connection);   // INSERT multiple documents
		sample2(connection);   // SELECT with projection (specific fields)
		sample3(connection);   // SELECT * (all fields)
		sample4(connection);   // SELECT with WHERE clause (filter)
		sample5(connection);   // SELECT with ORDER BY (sorting)
		sample6(connection);   // SELECT with range query ($gt, $lte)
		sample7(connection);   // CREATE INDEX
		sample8(connection);   // SELECT with LIMIT and SKIP (pagination)
		sample9(connection);   // SELECT LIMIT 1 (single document)
		sample10(connection);  // SELECT DISTINCT with WHERE
		sample11(connection);  // SELECT COUNT with WHERE
		sample12(connection);  // UPDATE with increment operator
		sample13(connection);  // DELETE

		std::cout << std::endl;
		std::cout << "==================================================" << std::endl;
		std::cout << "All samples completed successfully!" << std::endl;
		std::cout << "==================================================" << std::endl;
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << "ERROR: " << exc.displayText() << std::endl;
		return 1;
	}

	return 0;
}
