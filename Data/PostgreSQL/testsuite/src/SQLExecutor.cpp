//
// SQLExecutor.cpp
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CppUnit/TestCase.h"
#include "SQLExecutor.h"
#include "Poco/String.h"
#include "Poco/ByteOrder.h"
#include "Poco/Format.h"
#include "Poco/Tuple.h"
#include "Poco/Types.h"
#include "Poco/DateTime.h"
#include "Poco/Any.h"
#include "Poco/Exception.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Transaction.h"
#include "Poco/Data/PostgreSQL/PostgreSQLException.h"

#include <iostream>
#include <limits>


using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using Poco::Data::PostgreSQL::PostgreSQLException;
using Poco::Data::PostgreSQL::ConnectionException;
using Poco::Data::PostgreSQL::StatementException;
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
namespace Data {


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


} } // namespace Poco::Data


SQLExecutor::SQLExecutor(const std::string& name, Poco::Data::Session* pSession):
	CppUnit::TestCase(name),
	_pSession(pSession),
	_dataExecutor("Poco::Data SQL Executor", pSession, nullptr, true)
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
	_dataExecutor.simpleAccess();
}


void SQLExecutor::complexType()
{
	_dataExecutor.complexType();
}


void SQLExecutor::simpleAccessVector()
{
	_dataExecutor.simpleAccessVector();
}


void SQLExecutor::complexTypeVector()
{
	_dataExecutor.complexTypeVector();
}


void SQLExecutor::insertVector()
{
	_dataExecutor.insertVector();
}


void SQLExecutor::insertEmptyVector()
{
	_dataExecutor.insertEmptyVector();
}


void SQLExecutor::insertSingleBulk()
{
	_dataExecutor.insertSingleBulk();
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
	_dataExecutor.floats();
}


void SQLExecutor::uuids()
{
	_dataExecutor.uuids();
}


void SQLExecutor::doubles()
{
	_dataExecutor.doubles();
}


void SQLExecutor::insertSingleBulkVec()
{
	_dataExecutor.insertSingleBulkVec();
}


void SQLExecutor::limits()
{
	_dataExecutor.limits();
}


void SQLExecutor::limitZero()
{
	_dataExecutor.limitZero();
}


void SQLExecutor::limitOnce()
{
	_dataExecutor.limitOnce();
}


void SQLExecutor::limitPrepare()
{
	// fails: _dataExecutor.limitPrepare();
	// failed assert: "100 == stmt.execute()"
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
	_dataExecutor.prepare();
}


void SQLExecutor::setSimple()
{
	_dataExecutor.setSimple();
}


void SQLExecutor::setComplex()
{
	_dataExecutor.setComplex();
}


void SQLExecutor::setComplexUnique()
{
	_dataExecutor.setComplexUnique();
}


void SQLExecutor::multiSetSimple()
{
	_dataExecutor.multiSetSimple();
}


void SQLExecutor::multiSetComplex()
{
	_dataExecutor.multiSetComplex();
}


void SQLExecutor::mapComplex()
{
	_dataExecutor.mapComplex();
}


void SQLExecutor::mapComplexUnique()
{
	_dataExecutor.mapComplexUnique();
}


void SQLExecutor::multiMapComplex()
{
	_dataExecutor.multiMapComplex();
}


void SQLExecutor::selectIntoSingle()
{
	_dataExecutor.selectIntoSingle();
}


void SQLExecutor::selectIntoSingleStep()
{
	_dataExecutor.selectIntoSingleStep();
}


void SQLExecutor::selectIntoSingleFail()
{
	_dataExecutor.selectIntoSingleFail();
}


void SQLExecutor::lowerLimitOk()
{
	_dataExecutor.lowerLimitOk();
}


void SQLExecutor::singleSelect()
{
	_dataExecutor.singleSelect();
}


void SQLExecutor::lowerLimitFail()
{
	_dataExecutor.lowerLimitFail();
}


void SQLExecutor::combinedLimits()
{
	_dataExecutor.combinedLimits();
}



void SQLExecutor::ranges()
{
	_dataExecutor.ranges();
}


void SQLExecutor::combinedIllegalLimits()
{
	_dataExecutor.combinedIllegalLimits();
}


void SQLExecutor::illegalRange()
{
	_dataExecutor.illegalRange();
}


void SQLExecutor::emptyDB()
{
	_dataExecutor.emptyDB();
}


void SQLExecutor::dateTime()
{
	// fails because Person as defined here has a birthday but not a born column
	//_dataExecutor.dateTime();

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
	// fails because Person as defined here has a birthday but not a bornDate column
	//_dataExecutor.date();

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
	// fails because Person as defined here has a birthday but not a bornTime column
	//_dataExecutor.time();
	std::string funct = "time()";
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

	// Poco::Data::BLOB img("0123456789", 10);
unsigned char BLOBData[ 10 ] = { 254,253,252,251,4,5,6,7,14,15 };
	Poco::Data::BLOB img(BLOBData, 10);

	int count = 0;
	try { *_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(img), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	Poco::Data::BLOB res;
	assertTrue (res.size() == 0);
	try { *_pSession << "SELECT Image FROM Person", into(res), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (res == img);

	Poco::Data::BLOB big;
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
	Poco::Data::CLOB clob("0123456789", 10);

	int count = 0;
	Statement ins = (*_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(clob));
	ins.execute();
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	Poco::Data::CLOB res;
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
	Poco::Data::BLOB blob(BLOBData, 10);

	int count = 0;
	Statement ins = (*_pSession << "INSERT INTO Person VALUES ($1,$2,$3,$4)", use(lastName), use(firstName), use(address), use(blob));
	ins.execute();
	try { *_pSession << "SELECT COUNT(*) FROM Person", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	Poco::Data::BLOB res;
	poco_assert (res.size() == 0);
	Statement stmt = (*_pSession << "SELECT Image FROM Person", into(res));
	try { stmt.execute(); }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	poco_assert (res == blob);
}


void SQLExecutor::tuples()
{
	_dataExecutor.tuples();
}


void SQLExecutor::tupleVector()
{
	_dataExecutor.tupleVector();
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
						use(Poco::Data::Keywords::null),
						use(Poco::Data::Keywords::null),
						use(Poco::Data::Keywords::null), now;

	int count = 0;
	try { *_pSession << "SELECT COUNT(*) FROM Vectors", into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	assertTrue (count == 1);

	int i0 = 0;
	Statement stmt1 = (*_pSession << "SELECT i0 FROM Vectors", into(i0, Poco::Data::Position(0), -1));
	try { stmt1.execute(); }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	poco_assert (i0 == -1);

	float flt0 = 0;
	Statement stmt2 = (*_pSession << "SELECT flt0 FROM Vectors", into(flt0, Poco::Data::Position(0), 3.25f));
	try { stmt2.execute(); }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	poco_assert (flt0 == 3.25);

	std::string str0("string");
	Statement stmt3 = (*_pSession << "SELECT str0 FROM Vectors", into(str0, Poco::Data::Position(0), std::string("DEFAULT")));
	try { stmt3.execute(); }
	catch(ConnectionException& ce){ std::cout << ce.displayText() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.displayText() << std::endl; fail (funct); }
	poco_assert (str0 == "DEFAULT");
}


void SQLExecutor::setTransactionIsolation(Session& session, Poco::UInt32 ti)
{
	_dataExecutor.setTransactionIsolation(session, ti);
}


void SQLExecutor::sessionTransaction(const std::string& connect)
{
	_dataExecutor.sessionTransaction("postgresql", connect);
}



void SQLExecutor::sessionTransactionNoAutoCommit(const std::string& connect)
{
	_dataExecutor.sessionTransactionNoAutoCommit("postgresql", connect);
}


void SQLExecutor::transaction(const std::string& connect)
{
	_dataExecutor.transaction("postgresql", connect);
}


void SQLExecutor::reconnect()
{
	_dataExecutor.reconnect();
}
