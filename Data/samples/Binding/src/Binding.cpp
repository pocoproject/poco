/*
//
// Binding.cpp
//
// $Id: //poco/Main/Data/samples/Binding/src/Binding.cpp#1 $
//
// This sample demonstrates the Data library.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics
// Software Engineering GmbH.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part, without
// prior written permission from Applied Informatics.
//

#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include <vector>
#include <iostream>


using namespace Poco::Data;


struct Person
{
	std::string name;
	std::string address;
	int         age;
};


int main(int argc, char** argv)
{
	// register SQLite connector
	Poco::Data::SQLite::Connector::registerConnector();
	
	// create a session
	Session session("SQLite", "sample.db");

	// drop sample table, if it exists
	session << "DROP TABLE IF EXISTS Person", now;
	
	// (re)create table
	session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age INTEGER(3))", now;
	
	// insert some rows
	Person person = 
	{
		"Bart Simpson",
		"Springfield",
		12
	};
	
	Statement insert(session);
	insert << "INSERT INTO Person VALUES(:name, :address, :age)",
		use(person.name),
		use(person.address),
		use(person.age);
	
	insert.execute();
	
	person.name    = "Lisa Simpson";
	person.address = "Springfield";
	person.age     = 10;
	
	insert.execute();
	
	// a simple query
	Statement select(session);
	select << "SELECT Name, Address, Age FROM Person",
		into(person.name),
		into(person.address),
		into(person.age),
		range(0, 1); //  iterate over result set one row at a time
		
	while (!select.done())
	{
		select.execute();
		std::cout << person.name << " " << person.address << " " << person.age << std::endl;
	}

	// another query - store the result in a container
	std::vector<std::string> names;
	session << "SELECT Name FROM Person",
		into(names),
		now;
		
	for (std::vector<std::string>::const_iterator it = names.begin(); it != names.end(); ++it)
	{
		std::cout << *it << std::endl;
	}

	return 0;
}

*/


#include "Poco/Data/Session.h"
//#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Tuple.h"
#include "Poco/Data/ODBC/Connector.h"
#include <vector>
#include <iostream>


using namespace Poco::Data;


struct Person
{
	std::string name;
	std::string address;
	int         age;
};


int main(int argc, char** argv)
{
	// register SQLite connector
	//Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::ODBC::Connector::registerConnector();
	
	// create a session
	//Session session("SQLite", "sample.db");
	Session session("ODBC", "Dsn=PocoDataPgSQLTest;database=postgres;uid=postgres;pwd=postgres");
	//Session session("ODBC", "Dsn=AstolatV3;description=AstolatV3;uid=astolat;trusted_connection=Yes;database=AstolatV3");
	//session.setFeature("autoBind", true);
	// drop sample table, if it exists
	//session << "DROP TABLE IF EXISTS Person", now;
	try {
	session << "DROP TABLE Person", now;
	}
	catch(...) { }
	
	// (re)create table
	//session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR(3), Age INTEGER(3))", now;
	session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR(30), Age smallint)", now;
	
	// insert some rows
	Person person = 
	{
		"Bart",
		"Springfield",
		12
	};
	
/*
	typedef Poco::Tuple<std::string, std::string, int> Pers;
	typedef std::vector<Pers> People;
	People people;
	people.push_back(Pers("Bart Simpson",	"Springfield", 12));
	people.push_back(Pers("Lisa Simpson",	"Springfield", 10));
	people.push_back(Pers("Marge Simpson","Springfield", 35));
*/
	Statement insert(session);
	//insert << "INSERT INTO Person VALUES(:name, :address, :age)",
	insert << "INSERT INTO Person VALUES(?, ?, ?)", 
		use(person.name),
		use(person.address),
		use(person.age);
	
	insert.execute();
	
	person.name    = "Lisa Simpson";
	person.address = "Springfield";
	person.age     = 10;
	
	insert.execute();
	
	person.name    = "Marge Simpson";
	person.address = "Springfield";
	person.age     = 35;
	
	insert.execute();
	
	// a simple query
	Statement select(session);
	select << "SELECT Name, Address, Age FROM Person",
		into(person.name),
		into(person.address),
		into(person.age),
		range(0, 1); //  iterate over result set one row at a time
		
	while (!select.done())
	{
		select.execute();
		std::cout << person.name << " " << person.address << " " << person.age << std::endl;
	}

	// another query - store the result in a container
	std::vector<std::string> names;
	session << "SELECT Name FROM Person",
		into(names),
		now;
		
	for (std::vector<std::string>::const_iterator it = names.begin(); it != names.end(); ++it)
	{
		std::cout << "Name:" << *it << std::endl;
	}

	return 0;
}

