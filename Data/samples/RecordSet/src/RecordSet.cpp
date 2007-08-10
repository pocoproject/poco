//
// RecordSet.cpp
//
// $Id: //poco/1.3/Data/samples/RecordSet/src/RecordSet.cpp#1 $
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
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Column.h"
#include "Poco/Data/SQLite/Connector.h"
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
	session << "INSERT INTO Person VALUES('Bart Simpson', 'Springfield', 12)", now;
	session << "INSERT INTO Person VALUES('Lisa Simpson', 'Springfield', 10)", now;
		
	// a simple query
	Statement select(session);
	select << "SELECT * FROM Person";
	select.execute();

	// create a RecordSet 
	RecordSet rs(select);
	std::size_t cols = rs.columnCount();
	// print all column names
	for (std::size_t col = 0; col < cols; ++col)
	{
		std::cout << rs.columnName(col) << std::endl;
	}
	// iterate over all rows and columns
	bool more = rs.moveFirst();
	while (more)
	{
		for (std::size_t col = 0; col < cols; ++col)
		{
			std::cout << rs[col].convert<std::string>() << " ";
		}
		std::cout << std::endl;
		more = rs.moveNext();
	}

	return 0;
}
