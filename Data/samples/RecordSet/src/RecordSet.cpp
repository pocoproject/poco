//
// RecordSet.cpp
//
// $Id: //poco/Main/Data/samples/RecordSet/src/RecordSet.cpp#2 $
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
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>


using namespace Poco::Data;


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
	session << "INSERT INTO Person VALUES('Homer Simpson', 'Springfield', 42)", now;
	session << "INSERT INTO Person VALUES('Marge Simpson', 'Springfield', 38)", now;
	session << "INSERT INTO Person VALUES('Bart Simpson', 'Springfield', 12)", now;
	session << "INSERT INTO Person VALUES('Lisa Simpson', 'Springfield', 10)", now;
		
	// create a recordset and print the column names and data
	RecordSet rs(session, "SELECT * FROM Person");
	std::cout << rs;

	// print just two middle rows
	std::cout << std::endl << "Middle rows :" << std::endl;
	rs.copyValues(std::cout, 1, 2);

	std::cout << "---" << std::endl << "Thats all, folks!" << std::endl;

	return 0;
}
