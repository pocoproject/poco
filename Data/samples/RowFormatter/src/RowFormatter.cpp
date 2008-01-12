//
// RecordSet.cpp
//
// $Id: //poco/Main/Data/samples/RecordSet/src/RowFormatter.cpp#2 $
//
// This sample demonstrates the Data library recordset formatting
// capabilities.
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
#include "Poco/Data/Statement.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/RowFormatter.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>


using namespace Poco::Data;


class HTMLTableFormatter : public RowFormatter
{
public:
	HTMLTableFormatter()
	{
		std::ostringstream os;
		os << "<TABLE border=\"1\" cellspacing=\"0\">" << std::endl;
		setPrefix(os.str());
		
		os.str("");
		os << "</TABLE>" << std::endl;
		setPostfix(os.str());
	}

	std::string& formatNames(const NameVecPtr pNames, std::string& formattedNames) const
	{
		std::ostringstream str;

		str << "\t<TR>" << std::endl;
		NameVec::const_iterator it = pNames->begin();
		NameVec::const_iterator end = pNames->end();
		for (; it != end; ++it)	str << "\t\t<TH align=\"center\">" << *it << "</TH>" << std::endl;
		str << "\t</TR>" << std::endl;

		return formattedNames = str.str();
	}

	std::string& formatValues(const ValueVec& vals, std::string& formattedValues) const
	{
		std::ostringstream str;

		str << "\t<TR>" << std::endl;
		ValueVec::const_iterator it = vals.begin();
		ValueVec::const_iterator end = vals.end();
		for (; it != end; ++it)
		{
			if (it->isNumeric()) 
				str << "\t\t<TD align=\"right\">";
			else 
				str << "\t\t<TD align=\"left\">";

			str << it->convert<std::string>() << "</TD>" << std::endl;
		}
		str << "\t</TR>" << std::endl;

		return formattedValues = str.str();
	}
};


int main(int argc, char** argv)
{
	// register SQLite connector
	Poco::Data::SQLite::Connector::registerConnector();
	
	// create a session
	Session session("SQLite", "sample.db");

	// drop sample table, if it exists
	session << "DROP TABLE IF EXISTS Simpsons", now;
	
	// (re)create table
	session << "CREATE TABLE Simpsons (Name VARCHAR(30), Address VARCHAR, Age INTEGER(3))", now;
	
	// insert some rows
	session << "INSERT INTO Simpsons VALUES('Homer Simpson', 'Springfield', 42)", now;
	session << "INSERT INTO Simpsons VALUES('Marge Simpson', 'Springfield', 38)", now;
	session << "INSERT INTO Simpsons VALUES('Bart Simpson', 'Springfield', 12)", now;
	session << "INSERT INTO Simpsons VALUES('Lisa Simpson', 'Springfield', 10)", now;
		
	// create a statement and print the column names and data as HTML table
	HTMLTableFormatter tf;
	Statement stmt = (session << "SELECT * FROM Simpsons", format(tf), now);
	RecordSet rs(stmt);
	std::cout << rs << std::endl;

	// Note: The code above is divided into individual steps for clarity purpose.
	// The four lines can be reduced to the following single line of code:
	std::cout << RecordSet(session, "SELECT * FROM Simpsons", new HTMLTableFormatter);

	// simple formatting example (uses the default SimpleRowFormatter provided by framework)
	std::cout << std::endl << "Simple formatting:" << std::endl << std::endl;
	std::cout << RecordSet(session, "SELECT * FROM Simpsons");

	return 0;
}
