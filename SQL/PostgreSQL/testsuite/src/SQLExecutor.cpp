//
// SQLExecutor.cpp
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppUnit/TestCase.h"
#include "SQLExecutor.h"
#include "Poco/String.h"
#include "Poco/ByteOrder.h"
#include "Poco/Format.h"
#include "Poco/Tuple.h"
#include "Poco/Types.h"
#include "Poco/DateTime.h"
#include "Poco/Any.h"
#include "Poco/Exception.h"
#include "Poco/SQL/LOB.h"
#include "Poco/SQL/Date.h"
#include "Poco/SQL/Time.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SQL/RecordSet.h"
#include "Poco/SQL/Transaction.h"
#include "Poco/SQL/PostgreSQL/PostgreSQLException.h"

#include <iostream>
#include <limits>


using namespace Poco::SQL;
using namespace Poco::SQL::Keywords;
using Poco::SQL::PostgreSQL::PostgreSQLException;
using Poco::SQL::PostgreSQL::ConnectionException;
using Poco::SQL::PostgreSQL::StatementException;
using Poco::format;
using Poco::ByteOrder;
using Poco::Tuple;
using Poco::DateTime;
using Poco::Any;
using Poco::AnyCast;
using Poco::NotFoundException;
using Poco::InvalidAccessException;
using Poco::BadCastException;
using Poco::RangeException;


struct Person
{
	std::string lastName;
	std::string firstName;
	std::string address;
	int age;
	Person(){age = 0;}
	Person(const std::string& ln, const std::string& fn, const std::string& adr, int a):lastName(ln), firstName(fn), address(adr), age(a)
	{
	}
	bool operator==(const Person& other) const
	{
		return lastName == other.lastName && firstName == other.firstName && address == other.address && age == other.age;
	}

	bool operator < (const Person& p) const
	{
		if (age < p.age)
			return true;
		if (lastName < p.lastName)
			return true;
		if (firstName < p.firstName)
			return true;
		return (address < p.address);
	}

	const std::string& operator () () const
		/// This method is required so we can extract data to a map!
	{
		// we choose the lastName as examplary key
		return lastName;
	}
};


namespace Poco {
namespace SQL {


template <>
class TypeHandler<Person>
{
public:
	static void bind(std::size_t pos, const Person& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		poco_assert_dbg (!pBinder.isNull());
		pBinder->bind(pos++, obj.lastName, dir);
		pBinder->bind(pos++, obj.firstName, dir);
		pBinder->bind(pos++, obj.address, dir);
		pBinder->bind(pos++, obj.age, dir);
	}

	static void prepare(std::size_t pos, const Person& obj, AbstractPreparator::Ptr pPrepare)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		poco_assert_dbg (!pPrepare.isNull());
		pPrepare->prepare(pos++, obj.lastName);
		pPrepare->prepare(pos++, obj.firstName);
		pPrepare->prepare(pos++, obj.address);
		pPrepare->prepare(pos++, obj.age);
	}

	static std::size_t size()
	{
		return 4;
	}

	static void extract(std::size_t pos, Person& obj, const Person& defVal, AbstractExtractor::Ptr pExt)
	{
		poco_assert_dbg (!pExt.isNull());
		if (!pExt->extract(pos++, obj.lastName))
			obj.lastName = defVal.lastName;
		if (!pExt->extract(pos++, obj.firstName))
			obj.firstName = defVal.firstName;
		if (!pExt->extract(pos++, obj.address))
			obj.address = defVal.address;
		if (!pExt->extract(pos++, obj.age))
			obj.age = defVal.age;
	}

private:
	TypeHandler();
	~TypeHandler();
	TypeHandler(const TypeHandler&);
	TypeHandler& operator=(const TypeHandler&);
};


} } // namespace Poco::SQL


SQLExecutor::SQLExecutor(const std::string& name, Poco::SQL::Session* pSession):
	CppUnit::TestCase(name),
	_pSession(pSession)
{
}


SQLExecutor::~SQLExecutor()
{
}

void SQLExecutor::oidPostgreSQLTest(std::string host, std::string user, std::string pwd, std::string db, std::string port, const char* tableCreateString, const Oid anOIDArray[])
{
	std::string connectionString;

	connectionString.append("host=");
	connectionString.append(host);
	connectionString.append(" ");

	connectionString.append("user=");
	connectionString.append(user);
	connectionString.append(" ");

	connectionString.append("password=");
	connectionString.append(pwd);
	connectionString.append(" ");

	connectionString.append("dbname=");
	connectionString.append(db);
	connectionString.append(" ");

	connectionString.append("port=");
	connectionString.append(port);

	PGconn *  pConnection = 0;

	pConnection = PQconnectdb(connectionString.c_str());

	assertTrue (PQstatus(pConnection) == CONNECTION_OK);

	PGresult * pResult = 0;
	std::string sql = "DROP TABLE IF EXISTS Test";

	pResult = PQexec(pConnection, sql.c_str());

	std::cout << "Drop Table Test Result: " <<  PQresStatus(PQresultStatus(pResult)) << std::endl;

	assertTrue (PQresultStatus(pResult) == PGRES_COMMAND_OK
		|| PQresultStatus(pResult) == PGRES_FATAL_ERROR);

	PQclear(pResult);

	sql = tableCreateString;
	pResult = PQexec(pConnection, sql.c_str());

	std::cout << "Create Table Test Result: " <<  PQresStatus(PQresultStatus(pResult)) << std::endl;

	assertTrue (PQresultStatus(pResult) == PGRES_COMMAND_OK);
	PQclear(pResult);

	sql = "SELECT * FROM Test";
	std::string selectStatementName = "SELECT Statement";

	pResult = PQprepare(pConnection, selectStatementName.c_str(), sql.c_str(), 0, 0);

	assertTrue (PQresultStatus(pResult) == PGRES_COMMAND_OK);
	PQclear(pResult);

	pResult = PQdescribePrepared(pConnection, selectStatementName.c_str());

	assertTrue (PQresultStatus(pResult) == PGRES_COMMAND_OK);

	int fieldCount = PQnfields(pResult);
	assertTrue (fieldCount >= 0);

	bool wasErrorEncountered = false;

	for (int i = 0; i < fieldCount; ++i)
	{
		//char* columnFieldName	= PQfname(pResult, i);
		Oid columnFieldType		= PQftype(pResult, i);
		//int columnLength		= PQfsize(pResult, i); // TODO: Verify this is correct for all the returned types
		//int columnPrecision		= PQfmod(pResult, i);

		if (columnFieldType != anOIDArray[i])
		{
			// the binary API has changed - sadness!
			wasErrorEncountered = true;
			break;
		}
		
	}

	PQclear(pResult);

	sql = "DROP TABLE Test";

	pResult = PQexec(pConnection, sql.c_str());

	assertTrue (PQresultStatus(pResult) == PGRES_COMMAND_OK);
	PQclear(pResult);

	PQfinish(pConnection);

	if (wasErrorEncountered) {
		throw PostgreSQLException("PostgreSQL binary Data type values have changed in this release.  Major breakage!");
	}
}


void SQLExecutor::barebonePostgreSQLTest(std::string host, std::string user, std::string pwd, std::string db, std::string port, const char* tableCreateString)
{
	std::string connectionString;

	connectionString.append("host=");
	connectionString.append(host);
	connectionString.append(" ");

	connectionString.append("user=");
	connectionString.append(user);
	connectionString.append(" ");

	connectionString.append("password=");
	connectionString.append(pwd);
	connectionString.append(" ");

	connectionString.append("dbname=");
	connectionString.append(db);
	connectionString.append(" ");

	connectionString.append("port=");
	connectionString.append(port);

	PGconn *  pConnection = 0;

	pConnection = PQconnectdb(connectionString.c_str());

	assertTrue (PQstatus(pConnection) == CONNECTION_OK);
	
	PGresult * pResult = 0;
	std::string sql = "DROP TABLE IF EXISTS Test";

	pResult = PQexec(pConnection, sql.c_str());

	std::cout << "Drop Table Test Result: " <<  PQresStatus(PQresultStatus(pResult)) << " statement: "<< sql.c_str() << std::endl;

	assertTrue (	PQresultStatus(pResult) == PGRES_COMMAND_OK
			|| PQresultStatus(pResult) == PGRES_FATAL_ERROR);

	PQclear(pResult);

	sql = tableCreateString;
	pResult = PQexec(pConnection, sql.c_str());

	std::cout << "create Table Test Result: " <<  PQresStatus(PQresultStatus(pResult)) << " statement: "<< sql.c_str() << std::endl;

	assertTrue (PQresultStatus(pResult) == PGRES_COMMAND_OK);
	PQclear(pResult);

	sql = "INSERT INTO Test VALUES ($1,$2,$3,$4::int4,$5::float)";
	std::string insertStatementName = "Insert Statement";

	pResult = PQprepare(pConnection,
						insertStatementName.c_str(),
						sql.c_str(),
						5,
						0
						);

	assertTrue (PQresultStatus(pResult) == PGRES_COMMAND_OK);
	PQclear(pResult);

	std::string str[3] = { "111", "222", "333" };
	int fourth = ByteOrder::toNetwork((Poco::UInt32) 4);
	float fifth = 1.5;

	const char *paramValues[5] = { 0 };
	int paramLengths[5] = { 0 };
	int paramFormats[5] = { 0 };


	paramValues[0]  = const_cast<char*>(str[0].c_str());
	paramLengths[0] = static_cast<int>(str[0].length());
	paramFormats[0] = 0;  // text

	paramValues[1]	= const_cast<char*>(str[1].c_str());
	paramLengths[1] = static_cast<int>(str[1].length());
	paramFormats[1] = 0;   // text

	paramValues[2]	= const_cast<char*>(str[2].c_str());
	paramLengths[2] = static_cast<int>(str[2].length());
	paramFormats[2] = 0;   // text

	paramValues[3] = reinterpret_cast<char*>(&fourth);
	paramLengths[3] = sizeof(fourth);
	paramFormats[3] = 1; // binary

	paramValues[4] = reinterpret_cast<char*>(&fifth);
	paramLengths[4] = sizeof(fifth);
	paramLengths[4] = 1;  // binary

	pResult = PQexecPrepared(pConnection,
								insertStatementName.c_str(),
								5,
								paramValues,
								paramLengths,
								paramFormats,
								1 // ask for binary resilts
							);

	std::cout << "exec prepared Test Result: " <<  PQresStatus(PQresultStatus(pResult)) << " statement: "<< sql.c_str() << std::endl;

	assertTrue (PQresultStatus(pResult) == PGRES_COMMAND_OK);
	PQclear(pResult);

	sql = "SELECT * FROM Test";
	pResult = PQexec(pConnection, sql.c_str());

	std::cout << "select * Test Result: " <<  PQresStatus(PQresultStatus(pResult)) << " statement: "<< sql.c_str() << std::endl;

	assertTrue (PQresultStatus(pResult) == PGRES_TUPLES_OK);

	assertTrue (PQntuples(pResult) == 1);

	char* pSelectResult[5] = { 0 };
	int pResultLengths[5] = { 0 };

	// column 0
	pSelectResult[ 0 ] = PQgetvalue(pResult,
										0,
										0
									);

	pResultLengths[ 0 ] = PQgetlength(pResult,
										0,
										0
									);
	assertTrue (pSelectResult[ 0 ] != 0);
	assertTrue (pResultLengths[ 0 ] != 0);

	// column 1
	pSelectResult[ 1 ] = PQgetvalue(pResult,
										0,
										1
									);

	pResultLengths[ 1 ] = PQgetlength(pResult,
										0,
										1
									);
	assertTrue (pSelectResult[ 1 ] != 0);
	assertTrue (pResultLengths[ 1 ] != 0);

	// column 2
	pSelectResult[ 2 ] = PQgetvalue(pResult,
										0,
										2
									);

	pResultLengths[ 2 ] = PQgetlength(pResult,
										0,
										2
									);
	assertTrue (pSelectResult[ 2 ] != 0);
	assertTrue (pResultLengths[ 2 ] != 0);

// column 3
	pSelectResult[ 3 ] = PQgetvalue(pResult,
										0,
										3
									);

	pResultLengths[ 3 ] = PQgetlength(pResult,
											0,
											3
									);
	assertTrue (pSelectResult[ 3 ] != 0);
	assertTrue (pResultLengths[ 3 ] != 0);

	// column 4
	pSelectResult[ 4 ] = PQgetvalue(pResult,
										0,
										4
									);

	pResultLengths[ 4 ] = PQgetlength(pResult,
										0,
										4
									);
	assertTrue (pSelectResult[ 4 ] != 0);
	assertTrue (pResultLengths[ 4 ] != 0);

/*
 * The binary representation of INT4 is in network byte order, which
 * we'd better coerce to the local byte order.
*/

	fourth = ByteOrder::fromNetwork(*((Poco::UInt32 *) pSelectResult[ 3 ]));
	fifth = *((float *) pSelectResult[ 4 ]);

	assertTrue (0 == std::strncmp("111", pSelectResult[0], 3));
	assertTrue (0 == std::strncmp("222", pSelectResult[1], 3));
	assertTrue (0 == std::strncmp("333", pSelectResult[2], 3));
	assertTrue (4 == fourth);
	assertTrue (1.5 == fifth);

	PQclear(pResult);

	sql = "DROP TABLE Test";

	pResult = PQexec(pConnection, sql.c_str());

	assertTrue (PQresultStatus(pResult) == PGRES_COMMAND_OK);
	PQclear(pResult);

	PQfinish(pConnection);
}


void SQLExecutor::simpleAccess()
{
	std::string funct = "simpleAccess()";
	std::string lastName = "lastName";
	std::string firstName("firstName");
	std::string address("Address");
	int age = 133132;
	int count = 0;
	std::string result;

	count = 0;
	try
	{
		Statement stmt(*_pSession);
		stmt << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(age);//, now;
		stmt.execute();
	}
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now;  }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	assertTrue (count == 1);

	try { *_pSession << "SELECT LastName FROM Person", into(result), now;  }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (lastName == result);

	try { *_pSession << "SELECT Age FROM Person", into(count), now;  }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == age);
}


void SQLExecutor::complexType()
{
	std::string funct = "complexType()";
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(p1), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(p2), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);

	Person c1;
	Person c2;
	try { *_pSession << "SELECT * FROM Person WHERE LastName = 'LN1'", into(c1), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (c1 == p1);
}


void SQLExecutor::simpleAccessVector()
{
	std::string funct = "simpleAccessVector()";
	std::vector<std::string> lastNames;
	std::vector<std::string> firstNames;
	std::vector<std::string> addresses;
	std::vector<int> ages;
	std::string tableName("Person");
	lastNames.push_back("LN1");
	lastNames.push_back("LN2");
	firstNames.push_back("FN1");
	firstNames.push_back("FN2");
	addresses.push_back("ADDR1");
	addresses.push_back("ADDR2");
	ages.push_back(1);
	ages.push_back(2);
	int count = 0;
	std::string result;

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::vector<std::string> lastNamesR;
	std::vector<std::string> firstNamesR;
	std::vector<std::string> addressesR;
	std::vector<int> agesR;
	try { *_pSession << "SELECT * FROM Person", into(lastNamesR), into(firstNamesR), into(addressesR), into(agesR), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (ages == agesR);
	assertTrue (lastNames == lastNamesR);
	assertTrue (firstNames == firstNamesR);
	assertTrue (addresses == addressesR);
}


void SQLExecutor::complexTypeVector()
{
	std::string funct = "complexTypeVector()";
	std::vector<Person> people;
	people.push_back(Person("LN1", "FN1", "ADDR1", 1));
	people.push_back(Person("LN2", "FN2", "ADDR2", 2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::vector<Person> result;
	try { *_pSession << "SELECT * FROM Person", into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (result == people);
}


void SQLExecutor::insertVector()
{
	std::string funct = "insertVector()";
	std::vector<std::string> str;
	str.push_back("s1");
	str.push_back("s2");
	str.push_back("s3");
	str.push_back("s3");
	int count = 100;

	{
		Statement stmt((*_pSession << "INSERT INTO Strings VALUES ($1)", use(str)));
		try { *_pSession << "SELECT COUNT(*) FROM Strings", into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
		assertTrue (count == 0);

		try { stmt.execute(); }
		catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

		try { *_pSession << "SELECT COUNT(*) FROM Strings", into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
		assertTrue (count == 4);
	}
	count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Strings", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 4);
}


void SQLExecutor::insertEmptyVector()
{
	std::string funct = "insertEmptyVector()";
	std::vector<std::string> str;

	try
	{
		*_pSession << "INSERT INTO Strings VALUES ($1)", use(str), now;
		fail("empty collections should not work");
	}
	catch (Poco::Exception&)
	{
	}
}


void SQLExecutor::insertSingleBulk()
{
	std::string funct = "insertSingleBulk()";
	int x = 0;
	Statement stmt((*_pSession << "INSERT INTO Strings VALUES ($1)", use(x)));

	for (x = 0; x < 100; ++x)
	{
		std::size_t i = stmt.execute();
		assertTrue (i == 1);
	}

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Strings", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 100);

	try { *_pSession << "SELECT SUM(str) FROM Strings", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == ((0+99)*100/2));
}


void SQLExecutor::unsignedInts()
{
	std::string funct = "unsignedInts()";
	Poco::UInt32 data = std::numeric_limits<Poco::UInt32>::max();
	Poco::UInt32 ret = 0;

	try { *_pSession << "INSERT INTO Strings VALUES ($1)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Strings", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	try { *_pSession << "SELECT str FROM Strings", into(ret), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (ret == data);
}


void SQLExecutor::floats()
{
	std::string funct = "floats()";
	float data = 1.5f;
	float ret = 0.0f;

	try { *_pSession << "INSERT INTO Strings VALUES ($1)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Strings", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	try { *_pSession << "SELECT str FROM Strings", into(ret), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (ret == data);
}


void SQLExecutor::doubles()
{
	std::string funct = "floats()";
	double data = 1.5;
	double ret = 0.0;

	try { *_pSession << "INSERT INTO Strings VALUES ($1)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Strings", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	try { *_pSession << "SELECT str FROM Strings", into(ret), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (ret == data);
}


void SQLExecutor::insertSingleBulkVec()
{
	std::string funct = "insertSingleBulkVec()";
	std::vector<int> data;
	
	for (int x = 0; x < 100; ++x)
		data.push_back(x);

	Statement stmt((*_pSession << "INSERT INTO Strings VALUES ($1)", use(data)));
	stmt.execute();

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Strings", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	assertTrue (count == 100);
	try { *_pSession << "SELECT SUM(str) FROM Strings", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == ((0+99)*100/2));
}


void SQLExecutor::limits()
{
	std::string funct = "limit()";
	std::vector<int> data;
	for (int x = 0; x < 100; ++x)
	{
		data.push_back(x);
	}

	try { *_pSession << "INSERT INTO Strings VALUES ($1)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	std::vector<int> retData;
	try { *_pSession << "SELECT * FROM Strings", into(retData), limit(50), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (retData.size() == 50);
	for (int x = 0; x < 50; ++x)
	{
		assertTrue (data[x] == retData[x]);
	}
}


void SQLExecutor::limitZero()
{
	std::string funct = "limitZero()";
	std::vector<int> data;
	for (int x = 0; x < 100; ++x)
	{
		data.push_back(x);
	}

	try { *_pSession << "INSERT INTO Strings VALUES ($1)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	std::vector<int> retData;
	try { *_pSession << "SELECT * FROM Strings", into(retData), limit(0), now; }// stupid test, but at least we shouldn't crash
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (retData.size() == 0);
}


void SQLExecutor::limitOnce()
{
	std::string funct = "limitOnce()";
	std::vector<int> data;
	for (int x = 0; x < 101; ++x)
	{
		data.push_back(x);
	}

	try { *_pSession << "INSERT INTO Strings VALUES ($1)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	std::vector<int> retData;
	Statement stmt = (*_pSession << "SELECT * FROM Strings", into(retData), limit(50), now);
	assertTrue (!stmt.done());
	assertTrue (retData.size() == 50);
	stmt.execute();
	assertTrue (!stmt.done());
	assertTrue (retData.size() == 100);
	stmt.execute();
	assertTrue (stmt.done());
	assertTrue (retData.size() == 101);

	for (int x = 0; x < 101; ++x)
	{
		assertTrue (data[x] == retData[x]);
	}
}


void SQLExecutor::limitPrepare()
{
	std::string funct = "limitPrepare()";
	std::vector<int> data;
	for (int x = 0; x < 100; ++x)
	{
		data.push_back(x);
	}

	try { *_pSession << "INSERT INTO Strings VALUES ($1)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	std::vector<int> retData;
	Statement stmt = (*_pSession << "SELECT * FROM Strings", into(retData), limit(50));
	assertTrue (retData.size() == 0);
	assertTrue (!stmt.done());

	try { stmt.execute(); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (!stmt.done());
	assertTrue (retData.size() == 50);

	try { stmt.execute(); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (stmt.done());
	assertTrue (retData.size() == 100);

	try { stmt.execute(); }// will restart execution!
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (!stmt.done());
	assertTrue (retData.size() == 150);
	for (int x = 0; x < 150; ++x)
	{
		assertTrue (data[x%100] == retData[x]);
	}
}



void SQLExecutor::prepare()
{
	std::string funct = "prepare()";
	std::vector<int> data;
	for (int x = 0; x < 100; x += 2)
	{
		data.push_back(x);
	}

	{
		Statement stmt((*_pSession << "INSERT INTO Strings VALUES ($1)", use(data)));
	}
	// stmt should not have been executed when destroyed
	int count = 100;
	try { *_pSession << "SELECT COUNT(*) FROM Strings", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 0);
}


void SQLExecutor::setSimple()
{
	std::string funct = "setSimple()";
	std::set<std::string> lastNames;
	std::set<std::string> firstNames;
	std::set<std::string> addresses;
	std::set<int> ages;
	std::string tableName("Person");
	lastNames.insert("LN1");
	lastNames.insert("LN2");
	firstNames.insert("FN1");
	firstNames.insert("FN2");
	addresses.insert("ADDR1");
	addresses.insert("ADDR2");
	ages.insert(1);
	ages.insert(2);
	int count = 0;
	std::string result;

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::set<std::string> lastNamesR;
	std::set<std::string> firstNamesR;
	std::set<std::string> addressesR;
	std::set<int> agesR;
	try { *_pSession << "SELECT * FROM Person", into(lastNamesR), into(firstNamesR), into(addressesR), into(agesR), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (ages == agesR);
	assertTrue (lastNames == lastNamesR);
	assertTrue (firstNames == firstNamesR);
	assertTrue (addresses == addressesR);
}


void SQLExecutor::setComplex()
{
	std::string funct = "setComplex()";
	std::set<Person> people;
	people.insert(Person("LN1", "FN1", "ADDR1", 1));
	people.insert(Person("LN2", "FN2", "ADDR2", 2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::set<Person> result;
	try { *_pSession << "SELECT * FROM Person", into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (result == people);
}


void SQLExecutor::setComplexUnique()
{
	std::string funct = "setComplexUnique()";
	std::vector<Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	people.push_back(p1);
	people.push_back(p1);
	people.push_back(p1);
	people.push_back(p1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.push_back(p2);

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 5);

	std::set<Person> result;
	try { *_pSession << "SELECT * FROM Person", into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (result.size() == 2);
	assertTrue (*result.begin() == p1);
	assertTrue (*++result.begin() == p2);
}

void SQLExecutor::multiSetSimple()
{
	std::string funct = "multiSetSimple()";
	std::multiset<std::string> lastNames;
	std::multiset<std::string> firstNames;
	std::multiset<std::string> addresses;
	std::multiset<int> ages;
	std::string tableName("Person");
	lastNames.insert("LN1");
	lastNames.insert("LN2");
	firstNames.insert("FN1");
	firstNames.insert("FN2");
	addresses.insert("ADDR1");
	addresses.insert("ADDR2");
	ages.insert(1);
	ages.insert(2);
	int count = 0;
	std::string result;

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::multiset<std::string> lastNamesR;
	std::multiset<std::string> firstNamesR;
	std::multiset<std::string> addressesR;
	std::multiset<int> agesR;
	try { *_pSession << "SELECT * FROM Person", into(lastNamesR), into(firstNamesR), into(addressesR), into(agesR), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (ages.size() == agesR.size());
	assertTrue (lastNames.size() == lastNamesR.size());
	assertTrue (firstNames.size() == firstNamesR.size());
	assertTrue (addresses.size() == addressesR.size());
}


void SQLExecutor::multiSetComplex()
{
	std::string funct = "multiSetComplex()";
	std::multiset<Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	people.insert(p1);
	people.insert(p1);
	people.insert(p1);
	people.insert(p1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(p2);

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 5);

	std::multiset<Person> result;
	try { *_pSession << "SELECT * FROM Person", into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (result.size() == people.size());
}


void SQLExecutor::mapComplex()
{
	std::string funct = "mapComplex()";
	std::map<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN2", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::map<std::string, Person> result;
	try { *_pSession << "SELECT * FROM Person", into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (result == people);
}


void SQLExecutor::mapComplexUnique()
{
	std::string funct = "mapComplexUnique()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN2", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 5);

	std::map<std::string, Person> result;
	try { *_pSession << "SELECT * FROM Person", into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (result.size() == 2);
}


void SQLExecutor::multiMapComplex()
{
	std::string funct = "multiMapComplex()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN2", p2));
	
	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 5);

	std::multimap<std::string, Person> result;
	try { *_pSession << "SELECT * FROM Person", into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (result.size() == people.size());
}


void SQLExecutor::selectIntoSingle()
{
	std::string funct = "selectIntoSingle()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try { *_pSession << "SELECT * FROM Person", into(result), limit(1), now; }// will return 1 object into one single result
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (result == p1);
}


void SQLExecutor::selectIntoSingleStep()
{
	std::string funct = "selectIntoSingleStep()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	Statement stmt = (*_pSession << "SELECT * FROM Person", into(result), limit(1));
	stmt.execute();
	assertTrue (result == p1);
	assertTrue (!stmt.done());
	stmt.execute();
	assertTrue (result == p2);
	assertTrue (stmt.done());
}


void SQLExecutor::selectIntoSingleFail()
{
	std::string funct = "selectIntoSingleFail()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), limit(2, true), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try
	{
		*_pSession << "SELECT * FROM Person", into(result), limit(1, true), now; // will fail now
		fail("hardLimit is set: must fail");
	}
	catch(Poco::SQL::LimitException&)
	{
	}
}


void SQLExecutor::lowerLimitOk()
{
	std::string funct = "lowerLimitOk()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try
	{
		*_pSession << "SELECT * FROM Person", into(result), lowerLimit(2), now; // will return 2 objects into one single result but only room for one!
		fail("Not enough space for results");
	}
	catch(Poco::Exception&)
	{
	}
}


void SQLExecutor::singleSelect()
{
	std::string funct = "singleSelect()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	Statement stmt = (*_pSession << "SELECT * FROM Person", into(result), limit(1));
	stmt.execute();
	assertTrue (result == p1);
	assertTrue (!stmt.done());
	stmt.execute();
	assertTrue (result == p2);
	assertTrue (stmt.done());
}


void SQLExecutor::lowerLimitFail()
{
	std::string funct = "lowerLimitFail()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try
	{
		*_pSession << "SELECT * FROM Person", into(result), lowerLimit(3), now; // will fail
		fail("should fail. not enough data");
	}
	catch(Poco::Exception&)
	{
	}
}


void SQLExecutor::combinedLimits()
{
	std::string funct = "combinedLimits()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);
	std::vector <Person> result;
	try { *_pSession << "SELECT * FROM Person", into(result), lowerLimit(2), upperLimit(2), now; }// will return 2 objects
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (result.size() == 2);
	assertTrue (result[0] == p1);
	assertTrue (result[1] == p2);
}



void SQLExecutor::ranges()
{
	std::string funct = "range()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);
	std::vector <Person> result;
	try { *_pSession << "SELECT * FROM Person", into(result), range(2, 2), now; }// will return 2 objects
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (result.size() == 2);
	assertTrue (result[0] == p1);
	assertTrue (result[1] == p2);
}


void SQLExecutor::combinedIllegalLimits()
{
	std::string funct = "combinedIllegalLimits()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try
	{
		*_pSession << "SELECT * FROM Person", into(result), lowerLimit(3), upperLimit(2), now;
		fail("lower > upper is not allowed");
	}
	catch(LimitException&)
	{
	}
}


void SQLExecutor::illegalRange()
{
	std::string funct = "illegalRange()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN1", p2));

	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try
	{
		*_pSession << "SELECT * FROM Person", into(result), range(3, 2), now;
		fail("lower > upper is not allowed");
	}
	catch(LimitException&)
	{
	}
}


void SQLExecutor::emptyDB()
{
	std::string funct = "emptyDB()";
	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 0);

	Person result;
	Statement stmt = (*_pSession << "SELECT * FROM Person", into(result), limit(1));
	stmt.execute();
	assertTrue (result.firstName.empty());
	assertTrue (stmt.done());
}


void SQLExecutor::dateTime()
{
	std::string funct = "dateTime()";
	std::string lastName("Bart");
	std::string firstName("Simpson");
	std::string address("Springfield");
	DateTime birthday(1980, 4, 1, 5, 45, 12);
	
	int count = 0;
	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(birthday), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);
	
	DateTime bd;
	assertTrue (bd != birthday);
	try { *_pSession << "SELECT Birthday FROM Person", into(bd), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (bd == birthday);
	
	std::cout << std::endl << RecordSet(*_pSession, "SELECT * FROM Person");
}


void SQLExecutor::date()
{
	std::string funct = "date()";
	std::string lastName("Bart");
	std::string firstName("Simpson");
	std::string address("Springfield");
	Date birthday(1980, 4, 1);
	
	int count = 0;
	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(birthday), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);
	
	Date bd;
	assertTrue (bd != birthday);
	try { *_pSession << "SELECT Birthday FROM Person", into(bd), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (bd == birthday);
	
	std::cout << std::endl << RecordSet(*_pSession, "SELECT * FROM Person");
}


void SQLExecutor::time()
{
	std::string funct = "date()";
	std::string lastName("Bart");
	std::string firstName("Simpson");
	std::string address("Springfield");
	Time birthday(1, 2, 3);
	
	int count = 0;
	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(birthday), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);
	
	Time bd;
	assertTrue (bd != birthday);
	try { *_pSession << "SELECT Birthday FROM Person", into(bd), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (bd == birthday);
	
	std::cout << std::endl << RecordSet(*_pSession, "SELECT * FROM Person");
}


void SQLExecutor::blob(unsigned int bigSize)
{
	std::string funct = "blob()";
	std::string lastName("lastname");
	std::string firstName("firstname");
	std::string address("Address");

	// Poco::SQL::BLOB img("0123456789", 10);
unsigned char BLOBData[ 10 ] = { 254,253,252,251,4,5,6,7,14,15 };
	Poco::SQL::BLOB img(BLOBData, 10);

	int count = 0;
	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(img), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	Poco::SQL::BLOB res;
	assertTrue (res.size() == 0);
	try { *_pSession << "SELECT Image FROM Person", into(res), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (res == img);

	Poco::SQL::BLOB big;
	std::vector<unsigned char> v(bigSize, 250);
	big.assignRaw(&v[0], (std::size_t) v.size());

	assertTrue (big.size() == (std::size_t) bigSize);

	try { *_pSession << "DELETE FROM Person", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	try { *_pSession << "INSERT INTO Person VALUES($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(big), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	try { *_pSession << "SELECT Image FROM Person", into(res), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
//	assertTrue (res == big);
}


void SQLExecutor::clobStmt()
{
	std::string funct = "blobStmt()";
	std::string lastName("lastname");
	std::string firstName("firstname");
	std::string address("Address");
	Poco::SQL::CLOB clob("0123456789", 10);

	int count = 0;
	Statement ins = (*_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(clob));
	ins.execute();
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	Poco::SQL::CLOB res;
	poco_assert (res.size() == 0);
	Statement stmt = (*_pSession << "SELECT Story FROM Person", into(res));
	try { stmt.execute(); }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	poco_assert (res == clob);
}


void SQLExecutor::blobStmt()
{
	std::string funct = "blobStmt()";
	std::string lastName("lastname");
	std::string firstName("firstname");
	std::string address("Address");
unsigned char BLOBData[ 10 ] = { 0,1,2,3,4,5,6,7,14,15 };
	Poco::SQL::BLOB blob(BLOBData, 10);

	int count = 0;
	Statement ins = (*_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(blob));
	ins.execute();
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	Poco::SQL::BLOB res;
	poco_assert (res.size() == 0);
	Statement stmt = (*_pSession << "SELECT Image FROM Person", into(res));
	try { stmt.execute(); }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	poco_assert (res == blob);
}


void SQLExecutor::tuples()
{
	typedef Tuple<int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int> TupleType;
	std::string funct = "tuples()";
	TupleType t(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19);

	try { *_pSession << "INSERT INTO Tuples VALUES ($1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20)", use(t), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	TupleType ret(-10,-11,-12,-13,-14,-15,-16,-17,-18,-19);
	assertTrue (ret != t);
	try { *_pSession << "SELECT * FROM Tuples", into(ret), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (ret == t);
}


void SQLExecutor::tupleVector()
{
	typedef Tuple<int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int> TupleType;
	std::string funct = "tupleVector()";
	TupleType t(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19);
	Tuple<int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int>
		t10(10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29);
	TupleType t100(100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119);
	std::vector<TupleType> v;
	v.push_back(t);
	v.push_back(t10);
	v.push_back(t100);

	try { *_pSession << "INSERT INTO Tuples VALUES ($1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20)", use(v), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Tuples", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (v.size() == (std::size_t) count);

	std::vector<Tuple<int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int> > ret;
	try { *_pSession << "SELECT * FROM Tuples", into(ret), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (ret == v);
}


void SQLExecutor::internalExtraction()
{
	typedef Poco::Int32 IntType;
	std::string funct = "internalExtraction()";

	*_pSession << "DROP TABLE IF EXISTS Vectors", now;
	*_pSession << "CREATE TABLE Vectors (int0 INTEGER, flt0 REAL, str0 VARCHAR)", now;

	std::vector<Tuple<int, double, std::string> > v;
	v.push_back(Tuple<int, double, std::string>(1, 1.5, "3"));
	v.push_back(Tuple<int, double, std::string>(2, 2.5, "4"));
	v.push_back(Tuple<int, double, std::string>(3, 3.5, "5"));
	v.push_back(Tuple<int, double, std::string>(4, 4.5, "6"));

	*_pSession << "INSERT INTO Vectors VALUES ($1,$2,$3)", use(v), now;

	Statement stmt = (*_pSession << "SELECT * FROM Vectors", now);
	RecordSet rset(stmt);
	assertTrue (3 == rset.columnCount());
	assertTrue (4 == rset.rowCount());

	RecordSet rset2(rset);
	assertTrue (3 == rset2.columnCount());
	assertTrue (4 == rset2.rowCount());

	IntType a = 0;
	try { a = rset.value<IntType>(0, 2); }
	catch (Poco::BadCastException& bce) { std::cout << bce.displayText() << std::endl; fail (funct); }

	assertTrue (3 == a);

	int c = rset2.value(0);
	assertTrue (1 == c);

	IntType b = rset2.value<IntType>("int0", 2);
	assertTrue (3 == b);

	double d = 0.;
	try { d = rset.value<double>(1, 0); }
	catch (Poco::BadCastException& bce) { std::cout << bce.displayText() << std::endl; fail (funct); }

	assertTrue (1.5 == d);

	std::string s;
	try { s = rset.value<std::string>(2, 1); }
	catch (Poco::BadCastException& bce) { std::cout << bce.displayText() << std::endl; fail (funct); }

	assertTrue ("4" == s);

	typedef std::deque<IntType> IntDeq;

	const Column<IntDeq>& col = rset.column<IntDeq>(0);
	assertTrue (col[0] == 1);

	try { rset.column<IntDeq>(100); fail("must fail"); }
	catch (Poco::RangeException&) {}

	const Column<IntDeq>& col1 = rset.column<IntDeq>(0);
	assertTrue ("int0" == col1.name());
	Column<IntDeq>::Iterator it = col1.begin();
	Column<IntDeq>::Iterator itEnd = col1.end();
	int counter = 1;
	for (; it != itEnd; ++it, ++counter)
		assertTrue (counter == *it);

	rset = (*_pSession << "SELECT COUNT(*) AS cnt FROM Vectors", now);

	Poco::Int64 big = 0;
	try { big = rset.value<Poco::Int64>(0,0);}
	catch (Poco::BadCastException& bce) { std::cout << bce.displayText() << std::endl; fail (funct); }

	assertTrue (4 == big);

	s = rset.value("cnt", 0).convert<std::string>();
	assertTrue ("4" == s);

	stmt = (*_pSession << "DELETE FROM Vectors", now);
	rset = stmt;

	try { rset.column<IntDeq>(0); fail("must fail"); }
	catch (RangeException&) {}
}


void SQLExecutor::doNull()
{
	std::string funct = "null()";
	
	*_pSession << "INSERT INTO Vectors VALUES ($1, $2, $3)",
						use(Poco::SQL::Keywords::null),
						use(Poco::SQL::Keywords::null),
						use(Poco::SQL::Keywords::null), now;

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Vectors", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	int i0 = 0;
	Statement stmt1 = (*_pSession << "SELECT i0 FROM Vectors", into(i0, Poco::SQL::Position(0), -1));
	try { stmt1.execute(); }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	poco_assert (i0 == -1);

	float flt0 = 0;
	Statement stmt2 = (*_pSession << "SELECT flt0 FROM Vectors", into(flt0, Poco::SQL::Position(0), 3.25f));
	try { stmt2.execute(); }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	poco_assert (flt0 == 3.25);

	std::string str0("string");
	Statement stmt3 = (*_pSession << "SELECT str0 FROM Vectors", into(str0, Poco::SQL::Position(0), std::string("DEFAULT")));
	try { stmt3.execute(); }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	poco_assert (str0 == "DEFAULT");
}


void SQLExecutor::setTransactionIsolation(Session& session, Poco::UInt32 ti)
{
	if (session.hasTransactionIsolation(ti))
	{
		std::string funct = "setTransactionIsolation()";

		try
		{
			Transaction t(session, false);
			t.setIsolation(ti);
			
			assertTrue (ti == t.getIsolation());
			assertTrue (t.isIsolation(ti));
			
			assertTrue (ti == session.getTransactionIsolation());
			assertTrue (session.isTransactionIsolation(ti));
		}
		catch(Poco::Exception& e){ std::cout << funct << ':' << e.displayText() << std::endl;}
	}
	else
	{
		std::cout << "Transaction isolation not supported: ";
		switch (ti)
		{
		case Session::TRANSACTION_READ_COMMITTED:
			std::cout << "READ COMMITTED"; break;
		case Session::TRANSACTION_READ_UNCOMMITTED:
			std::cout << "READ UNCOMMITTED"; break;
		case Session::TRANSACTION_REPEATABLE_READ:
			std::cout << "REPEATABLE READ"; break;
		case Session::TRANSACTION_SERIALIZABLE:
			std::cout << "SERIALIZABLE"; break;
		default:
			std::cout << "UNKNOWN"; break;
		}
		std::cout << std::endl;
	}
}


void SQLExecutor::sessionTransaction(const std::string& connect)
{
	if (!_pSession->canTransact())
	{
		std::cout << "Session not capable of transactions." << std::endl;
		return;
	}

	Session local("postgresql", connect);
	local.setFeature("autoCommit", true);

	std::string funct = "transaction()";
	std::vector<std::string> lastNames;
	std::vector<std::string> firstNames;
	std::vector<std::string> addresses;
	std::vector<int> ages;
	std::string tableName("Person");
	lastNames.push_back("LN1");
	lastNames.push_back("LN2");
	firstNames.push_back("FN1");
	firstNames.push_back("FN2");
	addresses.push_back("ADDR1");
	addresses.push_back("ADDR2");
	ages.push_back(1);
	ages.push_back(2);
	int count = 0, locCount = 0;
	std::string result;

	bool autoCommit = _pSession->getFeature("autoCommit");

	// Next four lines inverted as autoCommit set to true is the normal mode
// autocommit set to false is the same as issuing a "begin" statement
_pSession->setFeature("autoCommit", false);
	assertTrue (_pSession->isTransaction());

	_pSession->setFeature("autoCommit", true);
	assertTrue (!_pSession->isTransaction());

	setTransactionIsolation((*_pSession), Session::TRANSACTION_READ_UNCOMMITTED);
	setTransactionIsolation((*_pSession), Session::TRANSACTION_REPEATABLE_READ);
	setTransactionIsolation((*_pSession), Session::TRANSACTION_SERIALIZABLE);

	setTransactionIsolation((*_pSession), Session::TRANSACTION_READ_COMMITTED);

	_pSession->begin();
	assertTrue (_pSession->isTransaction());
	try { (*_pSession) << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (_pSession->isTransaction());

	local << "SELECT COUNT(*) FROM Person", into(locCount), now;
	assertTrue (0 == locCount);

	try { (*_pSession) << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (2 == count);
	assertTrue (_pSession->isTransaction());
	_pSession->rollback();
	assertTrue (!_pSession->isTransaction());

	local << "SELECT COUNT(*) FROM Person", into(locCount), now;
	assertTrue (0 == locCount);

	try { (*_pSession) << "SELECT count(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (0 == count);
	assertTrue (!_pSession->isTransaction());

	_pSession->begin();
	try { (*_pSession) << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (_pSession->isTransaction());

	local << "SELECT COUNT(*) FROM Person", into(locCount), now;
	assertTrue (0 == locCount);

	_pSession->commit();
	assertTrue (!_pSession->isTransaction());

	local << "SELECT COUNT(*) FROM Person", into(locCount), now;
	assertTrue (2 == locCount);

	try { (*_pSession) << "SELECT count(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (2 == count);

	_pSession->setFeature("autoCommit", autoCommit); // redundant but ok
}


void SQLExecutor::transaction(const std::string& connect)
{
	if (!_pSession->canTransact())
	{
		std::cout << "Session not transaction-capable." << std::endl;
		return;
	}

	Session local("postgresql", connect);
	local.setFeature("autoCommit", true);

	setTransactionIsolation((*_pSession), Session::TRANSACTION_READ_COMMITTED);
	setTransactionIsolation(local, Session::TRANSACTION_READ_COMMITTED);

	std::string funct = "transaction()";
	std::vector<std::string> lastNames;
	std::vector<std::string> firstNames;
	std::vector<std::string> addresses;
	std::vector<int> ages;
	std::string tableName("Person");
	lastNames.push_back("LN1");
	lastNames.push_back("LN2");
	firstNames.push_back("FN1");
	firstNames.push_back("FN2");
	addresses.push_back("ADDR1");
	addresses.push_back("ADDR2");
	ages.push_back(1);
	ages.push_back(2);
	int count = 0, locCount = 0;
	std::string result;

	bool autoCommit = _pSession->getFeature("autoCommit");

	_pSession->setFeature("autoCommit", false);
	assertTrue (_pSession->isTransaction());
	_pSession->setFeature("autoCommit", true);
	assertTrue (!_pSession->isTransaction());

	_pSession->setTransactionIsolation(Session::TRANSACTION_READ_COMMITTED);

	{
		Transaction trans((*_pSession));
		assertTrue (trans.isActive());
		assertTrue (_pSession->isTransaction());
		
		try { (*_pSession) << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
		catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
		
		assertTrue (_pSession->isTransaction());
		assertTrue (trans.isActive());

		try { (*_pSession) << "SELECT COUNT(*) FROM Person", into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
		assertTrue (2 == count);
		assertTrue (_pSession->isTransaction());
		assertTrue (trans.isActive());
	}
	assertTrue (!_pSession->isTransaction());

	try { (*_pSession) << "SELECT count(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (0 == count);
	assertTrue (!_pSession->isTransaction());

	{
		Transaction trans((*_pSession));
		try { (*_pSession) << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
		catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

		local << "SELECT COUNT(*) FROM Person", into(locCount), now;
		assertTrue (0 == locCount);

		assertTrue (_pSession->isTransaction());
		assertTrue (trans.isActive());
		trans.commit();
		assertTrue (!_pSession->isTransaction());
		assertTrue (!trans.isActive());
		local << "SELECT COUNT(*) FROM Person", into(locCount), now;
		assertTrue (2 == locCount);
	}

	try { (*_pSession) << "SELECT count(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (2 == count);

	_pSession->begin();
	try { (*_pSession) << "DELETE FROM Person", now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	local << "SELECT COUNT(*) FROM Person", into(locCount), now;
	assertTrue (2 == locCount);

	try { (*_pSession) << "SELECT count(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (0 == count);
	_pSession->commit();

	local << "SELECT COUNT(*) FROM Person", into(locCount), now;
	assertTrue (0 == locCount);

	std::string sql1 = format("INSERT INTO Person VALUES ('%s','%s','%s',%d)", lastNames[0], firstNames[0], addresses[0], ages[0]);
	std::string sql2 = format("INSERT INTO Person VALUES ('%s','%s','%s',%d)", lastNames[1], firstNames[1], addresses[1], ages[1]);
	std::vector<std::string> sql;
	sql.push_back(sql1);
	sql.push_back(sql2);

	Transaction trans((*_pSession));

	trans.execute(sql1, false);
	try { (*_pSession) << "SELECT count(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (1 == count);
	trans.execute(sql2, false);
	try { (*_pSession) << "SELECT count(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (2 == count);

	local << "SELECT COUNT(*) FROM Person", into(locCount), now;
	assertTrue (0 == locCount);

	trans.rollback();

	local << "SELECT COUNT(*) FROM Person", into(locCount), now;
	assertTrue (0 == locCount);

	try { (*_pSession) << "SELECT count(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (0 == count);

	trans.execute(sql);
	
	local << "SELECT COUNT(*) FROM Person", into(locCount), now;
	assertTrue (2 == locCount);

	try { (*_pSession) << "SELECT count(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (2 == count);

	_pSession->setFeature("autoCommit", autoCommit);
}


void SQLExecutor::reconnect()
{
	std::string funct = "reconnect()";
	std::string lastName = "lastName";
	std::string firstName("firstName");
	std::string address("Address");
	int age = 133132;
	int count = 0;
	std::string result;

	try { (*_pSession) << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(age), now;  }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }

	count = 0;
	try { (*_pSession) << "SELECT COUNT(*) FROM Person", into(count), now;  }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	assertTrue (_pSession->isConnected());
	_pSession->close();
	assertTrue (!_pSession->isConnected());
	try
	{
		(*_pSession) << "SELECT LastName FROM Person", into(result), now;
		fail ("must fail");
	}
	catch(NotConnectedException&){ }
	assertTrue (!_pSession->isConnected());

	_pSession->open();
	assertTrue (_pSession->isConnected());
	try { (*_pSession) << "SELECT Age FROM Person", into(count), now;  }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == age);
	assertTrue (_pSession->isConnected());
}
