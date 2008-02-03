//
// Binding.cpp
//
// $Id: //poco/1.3/Data/samples/TypeHandler/src/TypeHandler.cpp#2 $
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


#include "Poco/SharedPtr.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/TypeHandler.h"
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


namespace Poco {
namespace Data {


template <>
class TypeHandler<Person>
	/// Defining a specialization of TypeHandler for Person allows us
	/// to use the Person struct in use and into clauses.
{
public:
	static std::size_t size()
	{
		return 3;
	}
	
	static void bind(std::size_t pos, const Person& person, AbstractBinder* pBinder)
	{
		TypeHandler<std::string>::bind(pos++, person.name, pBinder);
		TypeHandler<std::string>::bind(pos++, person.address, pBinder);
		TypeHandler<int>::bind(pos++, person.age, pBinder);
	}
	
	static void extract(std::size_t pos, Person& person, const Person& deflt, AbstractExtractor* pExtr)
	{
		TypeHandler<std::string>::extract(pos++, person.name, deflt.name, pExtr);
		TypeHandler<std::string>::extract(pos++, person.address, deflt.address, pExtr);
		TypeHandler<int>::extract(pos++, person.age, deflt.age, pExtr);
	}
	
	static void prepare(std::size_t pos, const Person& person, AbstractPreparation* pPrep)
	{
		TypeHandler<std::string>::prepare(pos++, person.name, pPrep);
		TypeHandler<std::string>::prepare(pos++, person.address, pPrep);
		TypeHandler<int>::prepare(pos++, person.age, pPrep);
	}
};


} } // namespace Poco::Data


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
	insert << "INSERT INTO Person VALUES(?, ?, ?)",
		use(person);
		
	insert.execute();
	
	person.name    = "Lisa Simpson";
	person.address = "Springfield";
	person.age     = 10;
	
	insert.execute();
	
	// a simple query
	Statement select(session);
	select << "SELECT Name, Address, Age FROM Person",
		into(person),
		range(0, 1); //  iterate over result set one row at a time
		
	while (!select.done())
	{
		select.execute();
		std::cout << person.name << " " << person.address << " " << person.age << std::endl;
	}
	
	// another query - store the result in a container
	std::vector<Person> persons;
	session << "SELECT Name, Address, Age FROM Person",
		into(persons),
		now;
		
	for (std::vector<Person>::const_iterator it = persons.begin(); it != persons.end(); ++it)
	{
		std::cout << it->name << " " << it->address << " " << it->age << std::endl;
	}
	
	return 0;
}
