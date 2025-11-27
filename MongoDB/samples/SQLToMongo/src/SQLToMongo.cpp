//
// main.cpp
//
// This sample shows SQL to MongoDB Shell to C++ examples using OP_MSG wire protocol.
//
// Copyright (c) 2013, Applied Informatics Software Engineering GmbH.
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


// INSERT INTO players
// VALUES( "Messi", "Lionel", 1987)
void sample1(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 1 ***" << std::endl;

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

	std::cout << request->documents().size() << std::endl;

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);
	if (!response.responseOk())
	{
		std::cout << "Error: " << response.body().toString(2) << std::endl;
	}
}


// SELECT lastname, birthyear FROM players
void sample2(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 2 ***" << std::endl;

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
}


// SELECT * FROM players
void sample3(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 3 ***" << std::endl;

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
}


// SELECT * FROM players WHERE birthyear = 1978
void sample4(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 4 ***" << std::endl;

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
}


// SELECT * FROM players WHERE birthyear = 1987 ORDER BY name
void sample5(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 5 ***" << std::endl;

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
}


// SELECT * FROM players WHERE birthyear > 1969 and birthyear <= 1980
void sample6(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 6 ***" << std::endl;

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
}


// CREATE INDEX playername
// ON players(lastname)
void sample7(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 7 ***" << std::endl;

	Poco::MongoDB::Database db("sample");
	Poco::MongoDB::Document::Ptr keys = new Poco::MongoDB::Document();
	keys->add("lastname", 1);
	Poco::MongoDB::Document::Ptr resultDoc = db.ensureIndex(connection, "players", "lastname", keys);

	std::cout << resultDoc->toString(2);
}


// SELECT * FROM players LIMIT 10 SKIP 20
void sample8(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 8 ***" << std::endl;

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
}


// SELECT * FROM players LIMIT 1
void sample9(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 9 ***" << std::endl;

	Poco::MongoDB::Database db("sample");
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(Poco::MongoDB::OpMsgMessage::CMD_FIND);
	request->body().add("limit", 1);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);
	if (!response.documents().empty())
	{
		std::cout << response.documents()[0]->toString(2) << std::endl;
	}
}


// SELECT DISTINCT birthyear FROM players WHERE birthyear > 1980
void sample10(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 10 ***" << std::endl;

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
		for (std::size_t i = 0; i < values->size(); ++i)
		{
			std::cout << values->get<int>(i) << std::endl;
		}
	}
}


// SELECT COUNT(*) FROM players WHERE birthyear > 1980
void sample11(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 11 ***" << std::endl;

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
		std::cout << "Count: " << response.body().getInteger("n") << std::endl;
	}
}


// UPDATE players SET birthyear = birthyear + 1 WHERE firstname = 'Victor'
void sample12(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 12 ***" << std::endl;

	Poco::MongoDB::Database db("sample");
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(Poco::MongoDB::OpMsgMessage::CMD_UPDATE);

	Poco::MongoDB::Document::Ptr update = new Poco::MongoDB::Document();
	update->addNewDocument("q").add("firstname", "Victor");
	update->addNewDocument("u").addNewDocument("$inc").add("birthyear", 1);
	request->documents().push_back(update);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);

	std::cout << "Response: " << response.body().toString(2) << std::endl;
}


// DELETE players WHERE firstname = 'Victor'
void sample13(Poco::MongoDB::Connection& connection)
{
	std::cout << "*** SAMPLE 13 ***" << std::endl;

	Poco::MongoDB::Database db("sample");
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = db.createOpMsgMessage("players");
	request->setCommandName(Poco::MongoDB::OpMsgMessage::CMD_DELETE);

	Poco::MongoDB::Document::Ptr del = new Poco::MongoDB::Document();
	del->addNewDocument("q").add("firstname", "Victor");
	del->add("limit", 0);  // 0 = delete all matching
	request->documents().push_back(del);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);

	std::cout << "Response: " << response.body().toString(2) << std::endl;
}


int main(int argc, char** argv)
{
	Poco::MongoDB::Connection connection("localhost", 27017);

	try
	{
		sample1(connection);
		sample2(connection);
		sample3(connection);
		sample4(connection);
		sample5(connection);
		sample6(connection);
		sample7(connection);
		sample8(connection);
		sample9(connection);
		sample10(connection);
		sample11(connection);
		sample12(connection);
		sample13(connection);
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
	}

	return 0;
}
