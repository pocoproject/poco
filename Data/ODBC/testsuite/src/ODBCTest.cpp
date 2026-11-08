//
// ODBCTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ODBCTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Any.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Tuple.h"
#include "Poco/DateTime.h"
#include "Poco/Exception.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include "Poco/Data/ODBC/Preparator.h"
#include "Poco/Data/ODBC/Extractor.h"
#include "Poco/Data/ODBC/SessionImpl.h"
#include "Poco/Data/DataException.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/Utility.h"
#include "Poco/Nullable.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/UTFString.h"
#include <sqltypes.h>
#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <typeinfo>
#include <vector>


using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::ConnectionFailedException;
using Poco::Data::CLOB;
using Poco::Data::ODBC::Utility;
using Poco::Data::ODBC::ODBCException;
using Poco::Data::ODBC::ConnectionException;
using Poco::Data::ODBC::StatementException;
using Poco::Data::ODBC::StatementDiagnostics;
using Poco::format;
using Poco::Tuple;
using Poco::Any;
using Poco::AnyCast;
using Poco::Dynamic::Var;
using Poco::DateTime;
using Poco::NotFoundException;


ODBCTest::Drivers ODBCTest::_drivers;


ODBCTest::ODBCTest(const std::string& name,
	SessionPtr pSession,
	ExecPtr pExecutor,
	std::string& rDSN,
	std::string& rUID,
	std::string& rPwd,
	std::string& rConnectString):
	CppUnit::TestCase(name),
	_pSession(pSession),
	_pExecutor(pExecutor),
	_rDSN(rDSN),
	_rUID(rUID),
	_rPwd(rPwd),
	_rConnectString(rConnectString)
{
	_pSession->setFeature("autoCommit", true);
}


ODBCTest::~ODBCTest()
{
}


void ODBCTest::testConnection()
{
	try
	{
		// postgres fails here because of  setTimeout() not supported
		// so we catch and loudly ignore the exception
		_pExecutor->connection(_rConnectString);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}


void ODBCTest::testSession()
{
	_pExecutor->session(_rConnectString, 5);
}


void ODBCTest::testSessionPool()
{
	_pExecutor->sessionPool(_rConnectString, 1, 4, 3, 5);
}


void ODBCTest::testClosedSession()
{
	if (_pSession == nullptr) failmsg ("Test not available.");

	recreatePersonTable();
	_pExecutor->closedSession(_rConnectString, 5);
}


void ODBCTest::testExceptionCopy()
{
	if (_pSession == nullptr) failmsg ("Test not available.");

	_pExecutor->exceptionCopy();
}


void ODBCTest::testSQLGetInfo()
{
	if (!_pSession) failmsg ("Test not available.");

	// Utility::sqlGetInfo() reads the value into a buffer of 1024 bytes, or
	// of the given size. Its results are compared with the values read
	// through a buffer large enough for them. The length reported for a value
	// that does not fit into the buffer, and in Unicode builds for a value of
	// more characters than half the buffer size, exceeds the buffer; such a
	// value must be returned as far as it is in the buffer. A DBMS name of
	// more than 3 bytes read into 4 bytes is such a value with every driver,
	// and so is the SQL_KEYWORDS list of some drivers read into 1024 bytes.
	auto* pImpl = dynamic_cast<Poco::Data::ODBC::SessionImpl*>(session().impl());
	assertNotNullPtr (pImpl);
	const Poco::Data::ODBC::ConnectionHandle& dbc = pImpl->dbc();

	const auto info = [&dbc](SQLUSMALLINT type)
	{
		std::vector<SQLCHAR> buffer(8192, 0);
		SQLSMALLINT length = 0;
		if (Utility::isError(Poco::Data::ODBC::SQLGetInfo(dbc.handle(), type,
			buffer.data(), static_cast<SQLSMALLINT>(buffer.size()), &length)))
		{
			return std::string("unknown");
		}
		const char* pValue = reinterpret_cast<const char*>(buffer.data());
		return std::string(pValue, std::find(pValue, pValue + buffer.size(), '\0'));
	};

	assertEqual (info(SQL_DBMS_NAME), Utility::dbmsName(dbc));
	assertEqual (info(SQL_DBMS_VER), Utility::dbmsVersion(dbc));
	assertEqual (info(SQL_DRIVER_NAME), Utility::driverName(dbc));
	assertEqual (info(SQL_DRIVER_VER), Utility::driverVersion(dbc));
	assertEqual (info(SQL_DRIVER_ODBC_VER), Utility::driverODBCVersion(dbc));
	assertEqual (Utility::dbmsName(dbc), session().dbmsName());

	const std::string dbms = info(SQL_DBMS_NAME);
	const std::string prefix = Utility::sqlGetInfo(dbc, SQL_DBMS_NAME, 4);
	if (dbms.size() < 4)
		assertEqual (dbms, prefix);
	else
	{
		// the 3 bytes before the terminator, or a full buffer
		assertTrue (prefix.size() >= 3 && prefix.size() <= 4);
		assertTrue (0 == dbms.compare(0, prefix.size(), prefix));
	}

	const std::string keywords = info(SQL_KEYWORDS);
	const std::string value = Utility::sqlGetInfo(dbc, SQL_KEYWORDS);
	if (keywords.size() < 1024)
		assertEqual (keywords, value);
	else
	{
		// the 1023 bytes before the terminator, or a full buffer
		assertTrue (value.size() >= 1023 && value.size() <= 1024);
		assertTrue (0 == keywords.compare(0, value.size(), value));
	}

	try
	{
		static_cast<void>(Utility::sqlGetInfo(dbc, SQL_DBMS_NAME, 0));
		fail ("sqlGetInfo() accepted a buffer size of 0");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
}


void ODBCTest::testZeroRows()
{
	if (!_pSession) failmsg ("Test not available.");

	std::string tableName("Person");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->zeroRows();
		i += 2;
	}
}


void ODBCTest::testSimpleAccess()
{
	if (!_pSession) failmsg ("Test not available.");

	std::string tableName("Person");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->simpleAccess();
		i += 2;
	}
}


void ODBCTest::testComplexType()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->complexType();
		i += 2;
	}
}


void ODBCTest::testComplexTypeTuple()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTupleTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->complexTypeTuple();
		i += 2;
	}
}


void ODBCTest::testSimpleAccessVector()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->simpleAccessVector();
		i += 2;
	}
}


void ODBCTest::testComplexTypeVector()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->complexTypeVector();
		i += 2;
	}
}


void ODBCTest::testSharedPtrComplexTypeVector()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->sharedPtrComplexTypeVector();
		i += 2;
	}
}


void ODBCTest::testAutoPtrComplexTypeVector()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->autoPtrComplexTypeVector();
		i += 2;
	}
}


void ODBCTest::testInsertVector()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->insertVector();
		i += 2;
	}
}


void ODBCTest::testInsertEmptyVector()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->insertEmptyVector();
		i += 2;
	}
}


void ODBCTest::testSimpleAccessList()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->simpleAccessList();
		i += 2;
	}
}


void ODBCTest::testComplexTypeList()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->complexTypeList();
		i += 2;
	}
}


void ODBCTest::testInsertList()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->insertList();
		i += 2;
	}
}


void ODBCTest::testInsertEmptyList()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->insertEmptyList();
		i += 2;
	}
}


void ODBCTest::testSimpleAccessDeque()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->simpleAccessDeque();
		i += 2;
	}
}


void ODBCTest::testComplexTypeDeque()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->complexTypeDeque();
		i += 2;
	}
}


void ODBCTest::testInsertDeque()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->insertDeque();
		i += 2;
	}
}


void ODBCTest::testInsertEmptyDeque()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->insertEmptyDeque();
		i += 2;
	}
}


void ODBCTest::testAffectedRows()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->affectedRows();
		i += 2;
	}
}


void ODBCTest::testInsertSingleBulk()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->insertSingleBulk();
		i += 2;
	}
}


void ODBCTest::testInsertSingleBulkVec()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->insertSingleBulkVec();
		i += 2;
	}
}


void ODBCTest::testLimit()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->limits();
		i += 2;
	}
}


void ODBCTest::testLimitZero()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->limitZero();
		i += 2;
	}
}


void ODBCTest::testLimitOnce()
{
	if (!_pSession) failmsg ("Test not available.");

	recreateIntsTable();
	_pExecutor->limitOnce();

}


void ODBCTest::testLimitPrepare()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->limitPrepare();
		i += 2;
	}
}



void ODBCTest::testPrepare()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->prepare();
		i += 2;
	}
}


void ODBCTest::testNullBulk()
{
	if (!_pSession) failmsg ("Test not available.");

	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);

	recreatePersonBLOBTable();
	_pExecutor->nullBulk();
}


void ODBCTest::testBulk()
{
	if (!_pSession) failmsg ("Test not available.");

	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);

	recreateMiscTable();
	_pExecutor->doBulk<std::vector<int>,
		std::vector<std::string>,
		std::vector<CLOB>,
		std::vector<double>,
		std::vector<DateTime>>(100);

	recreateMiscTable();
	_pExecutor->doBulk<std::deque<int>,
		std::deque<std::string>,
		std::deque<CLOB>,
		std::deque<double>,
		std::deque<DateTime>>(100);

	recreateMiscTable();
	_pExecutor->doBulk<std::list<int>,
		std::list<std::string>,
		std::list<CLOB>,
		std::list<double>,
		std::list<DateTime>>(100);
}


void ODBCTest::testBulkPerformance()
{
	if (!_pSession) failmsg ("Test not available.");

	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);

	recreateMiscTable();
	_pExecutor->doBulkPerformance(1000);
}


void ODBCTest::testSetSimple()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->setSimple();
		i += 2;
	}
}


void ODBCTest::testSetComplex()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->setComplex();
		i += 2;
	}
}


void ODBCTest::testSetComplexUnique()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->setComplexUnique();
		i += 2;
	}
}

void ODBCTest::testMultiSetSimple()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->multiSetSimple();
		i += 2;
	}
}


void ODBCTest::testMultiSetComplex()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->multiSetComplex();
		i += 2;
	}
}


void ODBCTest::testMapComplex()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->mapComplex();
		i += 2;
	}
}


void ODBCTest::testMapComplexUnique()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->mapComplexUnique();
		i += 2;
	}
}


void ODBCTest::testMultiMapComplex()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->multiMapComplex();
		i += 2;
	}
}


void ODBCTest::testSelectIntoSingle()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->selectIntoSingle();
		i += 2;
	}
}


void ODBCTest::testSelectIntoSingleStep()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->selectIntoSingleStep();
		i += 2;
	}
}


void ODBCTest::testSelectIntoSingleFail()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->selectIntoSingleFail();
		i += 2;
	}
}


void ODBCTest::testLowerLimitOk()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->lowerLimitOk();
		i += 2;
	}
}


void ODBCTest::testSingleSelect()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->singleSelect();
		i += 2;
	}
}


void ODBCTest::testLowerLimitFail()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->lowerLimitFail();
		i += 2;
	}
}


void ODBCTest::testCombinedLimits()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->combinedLimits();
		i += 2;
	}
}



void ODBCTest::testRange()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->ranges();
		i += 2;
	}
}


void ODBCTest::testCombinedIllegalLimits()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->combinedIllegalLimits();
		i += 2;
	}
}



void ODBCTest::testIllegalRange()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->illegalRange();
		i += 2;
	}
}


void ODBCTest::testEmptyDB()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->emptyDB();
		i += 2;
	}
}


void ODBCTest::testBLOB()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonBLOBTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->blob();
		i += 2;
	}
}


void ODBCTest::testBLOBContainer()
{
	for (int i = 0; i < 8;)
	{
		session().setFeature("autoBind", bindValue(i));
		session().setFeature("autoExtract", bindValue(i+1));
		recreatePersonBLOBTable();
		_pExecutor->blobContainer<std::vector<std::string>, std::vector<CLOB> >(10);
		recreatePersonBLOBTable();
		_pExecutor->blobContainer<std::deque<std::string>, std::deque<CLOB> >(10);
		recreatePersonBLOBTable();
		_pExecutor->blobContainer<std::list<std::string>, std::list<CLOB> >(10);
		i += 2;
	}
}


void ODBCTest::testBulkRowsetBackstop()
{
	if (!_pSession) failmsg ("Test not available.");

	// Statement rejects mixing bulk and non-bulk extractions. This adds the
	// non-bulk extraction directly to the implementation, to check the backstop
	// in ODBCStatementImpl::doPrepare(), where the rowset size is set. The table
	// has a single row, so even without the backstop nothing is written past
	// the one-row buffer.
	class MixedStatement: public Poco::Data::Statement
	{
	public:
		explicit MixedStatement(Poco::Data::Session& session): Poco::Data::Statement(session)
		{
		}

		void addUnchecked(Poco::Data::AbstractExtraction::Ptr pExtraction)
		{
			impl()->addExtract(pExtraction);
		}
	};

	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);
	dropObject("TABLE", "BulkRowsetBackstop");
	session() << "CREATE TABLE BulkRowsetBackstop (Id INTEGER, Payload VARCHAR(30))", now;
	session() << "INSERT INTO BulkRowsetBackstop VALUES (1, 'a')", now;

	std::vector<int> ids;
	std::string payload;
	MixedStatement stmt(session());
	stmt << "SELECT Id, Payload FROM BulkRowsetBackstop", into(ids, bulk(4));
	stmt.addUnchecked(into(payload));
	try
	{
		stmt.execute();
		fail ("bulk and non-bulk extractions executed together");
	}
	catch (Poco::InvalidAccessException&)
	{
	}
	dropObject("TABLE", "BulkRowsetBackstop");
}


void ODBCTest::testBulkBindingBackstops()
{
	if (!_pSession) failmsg ("Test not available.");

	// The row count shows how many parameter sets the driver processed: it
	// reads SQL_ATTR_PARAMSET_SIZE values from every bound parameter buffer.
	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);
	dropObject("TABLE", "BulkBindingBackstops");
	session() << "CREATE TABLE BulkBindingBackstops (A INTEGER, B INTEGER)", now;
	int count = 0;

	// re-executed with grown containers: Binder::reset() runs once per bulk
	// binding, and the arrays are bound again at their new size
	std::vector<int> a(2, 1);
	std::vector<int> b(2, 1);
	{
		Poco::Data::Statement insert = (session() << "INSERT INTO BulkBindingBackstops VALUES (?, ?)", use(a, bulk), use(b, bulk));
		insert.execute();
		a.assign(4, 2);
		b.assign(4, 2);
		insert.execute();
	}
	session() << "SELECT COUNT(*) FROM BulkBindingBackstops", into(count), now;
	assertEqual (6, count);

	// shrunk after the statement was built: the sizes are compared when the
	// containers are bound, not when use() ran
	{
		Poco::Data::Statement insert = (session() << "INSERT INTO BulkBindingBackstops VALUES (?, ?)", use(a, bulk), use(b, bulk));
		b.resize(2);
		try
		{
			insert.execute();
			fail ("bulk bindings of different sizes executed together");
		}
		catch (Poco::InvalidArgumentException&)
		{
		}
	}
	session() << "SELECT COUNT(*) FROM BulkBindingBackstops", into(count), now;
	assertEqual (6, count);

	// scalars bound after the bulk bindings were removed are one parameter
	// set, whatever the earlier array size
	b.assign(4, 3);
	{
		Poco::Data::Statement insert = (session() << "INSERT INTO BulkBindingBackstops VALUES (?, ?)", use(a, bulk), use(b, bulk));
		insert.execute();
		insert.removeBind("");
		int x = 4;
		insert.addBind(use(x));
		insert.addBind(use(x));
		insert.execute();
	}
	session() << "SELECT COUNT(*) FROM BulkBindingBackstops", into(count), now;
	assertEqual (11, count);

	// bulk bindings replaced after an execution: no binding that is left holds
	// the Binder, so it must not keep the earlier parameter-set size for the
	// smaller containers. boundSQL() leaves the new bindings with its own
	// binder until the statement is compiled again.
	a.assign(16, 4);
	b.assign(16, 4);
	{
		std::vector<int> c(2, 5);
		std::vector<int> d(2, 5);
		Poco::Data::Statement insert = (session() << "INSERT INTO BulkBindingBackstops VALUES (?, ?)", use(a, bulk), use(b, bulk));
		insert.execute();
		insert.removeBind("");
		insert.addBind(use(c, bulk));
		insert.addBind(use(d, bulk));
		static_cast<void>(Poco::Data::Utility::boundSQL(insert));
		insert.execute();
	}
	session() << "SELECT COUNT(*) FROM BulkBindingBackstops", into(count), now;
	assertEqual (29, count);

	// the same without boundSQL()
	{
		std::vector<int> c(2, 6);
		std::vector<int> d(2, 6);
		Poco::Data::Statement insert = (session() << "INSERT INTO BulkBindingBackstops VALUES (?, ?)", use(a, bulk), use(b, bulk));
		insert.execute();
		insert.removeBind("");
		insert.addBind(use(c, bulk));
		insert.addBind(use(d, bulk));
		insert.execute();
	}
	session() << "SELECT COUNT(*) FROM BulkBindingBackstops", into(count), now;
	assertEqual (47, count);

	// fewer parameters bound again than the statement has markers: the driver
	// must not keep the earlier parameter buffers, so the execution fails
	{
		std::vector<int> c(2, 7);
		Poco::Data::Statement insert = (session() << "INSERT INTO BulkBindingBackstops VALUES (?, ?)", use(a, bulk), use(b, bulk));
		insert.execute();
		insert.removeBind("");
		insert.addBind(use(c, bulk));
		try
		{
			insert.execute();
			fail ("executed with a parameter marker that was not bound again");
		}
		catch (Poco::Data::DataException&)
		{
		}
	}
	session() << "SELECT COUNT(*) FROM BulkBindingBackstops", into(count), now;
	assertEqual (63, count);

	// the same for scalars, after removing one named binding
	{
		int x = 8;
		int y = 8;
		Poco::Data::Statement insert = (session() << "INSERT INTO BulkBindingBackstops VALUES (?, ?)", use(x, "x"), use(y, "y"));
		insert.execute();
		insert.removeBind("y");
		try
		{
			insert.execute();
			fail ("executed with a parameter marker that was not bound again");
		}
		catch (Poco::Data::DataException&)
		{
		}
	}
	session() << "SELECT COUNT(*) FROM BulkBindingBackstops", into(count), now;
	assertEqual (64, count);

	// bindings that are not bulk bindings bind one parameter array per row for
	// a vector of vectors: the arrays of a row must have the same size
	{
		std::vector<std::vector<int>> x(1, std::vector<int>(4, 9));
		std::vector<std::vector<int>> y(1, std::vector<int>(2, 9));
		Poco::Data::Statement insert = (session() << "INSERT INTO BulkBindingBackstops VALUES (?, ?)", use(x), use(y));
		try
		{
			insert.execute();
			fail ("parameter arrays of different sizes executed together");
		}
		catch (Poco::InvalidArgumentException&)
		{
		}
	}
	session() << "SELECT COUNT(*) FROM BulkBindingBackstops", into(count), now;
	assertEqual (64, count);

	// and every row is executed with the size of its own arrays
	{
		std::vector<std::vector<int>> x(2, std::vector<int>(3, 10));
		std::vector<std::vector<int>> y(2, std::vector<int>(3, 10));
		Poco::Data::Statement insert = (session() << "INSERT INTO BulkBindingBackstops VALUES (?, ?)", use(x), use(y));
		insert.execute();
	}
	session() << "SELECT COUNT(*) FROM BulkBindingBackstops", into(count), now;
	assertEqual (70, count);
	dropObject("TABLE", "BulkBindingBackstops");
}


void ODBCTest::testParameterArraySize()
{
	// The Binder allocates bulk parameter buffers with Utility::allocArray():
	// rows times row length is checked before it is computed, so it can not wrap
	// around, and a failed allocation throws instead of leaving a null buffer
	// for the rows to be copied to.
	const std::size_t maxSize = std::numeric_limits<std::size_t>::max();

	assertEqual (std::size_t(24), Utility::arraySize(3, 4, 2));
	assertEqual (std::size_t(0), Utility::arraySize(0, 4, 2));
	assertEqual (std::size_t(0), Utility::arraySize(3, 0, 2));
	assertEqual (std::size_t(0), Utility::arraySize(3, 4, 0));
	assertEqual (maxSize, Utility::arraySize(maxSize, 1, 1));
	assertEqual (maxSize - 1, Utility::arraySize(maxSize / 2, 2, 1));
	assertEqual (maxSize - 1, Utility::arraySize(1, maxSize / 2, 2));

	try
	{
		static_cast<void>(Utility::arraySize(maxSize / 2 + 1, 2, 1));
		fail ("array size wrapped around");
	}
	catch (Poco::Data::LengthExceededException&)
	{
	}

	try
	{
		static_cast<void>(Utility::arraySize(2, maxSize / 2 + 1, 1));
		fail ("array size wrapped around");
	}
	catch (Poco::Data::LengthExceededException&)
	{
	}

	// rows times row length fits, the size in bytes does not
	try
	{
		static_cast<void>(Utility::arraySize(maxSize / 4 + 1, 2, 2));
		fail ("array size wrapped around");
	}
	catch (Poco::Data::LengthExceededException&)
	{
	}

	// a size that needs more than 32 bits
	if constexpr (sizeof(std::size_t) > 4)
	{
		assertEqual (std::size_t(65536) * 65537, Utility::arraySize(65536, 65537, 1));
	}
	else
	{
		try
		{
			static_cast<void>(Utility::arraySize(65536, 65537, 1));
			fail ("array size wrapped around");
		}
		catch (Poco::Data::LengthExceededException&)
		{
		}
	}

	// rejected before anything is allocated
	try
	{
		std::free(Utility::allocArray(maxSize / 2 + 1, 2, 1));
		fail ("array size wrapped around");
	}
	catch (Poco::Data::LengthExceededException&)
	{
	}

	// an empty array gets one element, whatever calloc() returns for a zero size
	void* pEmpty = Utility::allocArray(0, 0, 2);
	assertTrue (nullptr != pEmpty);
	std::free(pEmpty);

	unsigned char* pArray = static_cast<unsigned char*>(Utility::allocArray(3, 4, 2));
	assertTrue (nullptr != pArray);
	for (std::size_t i = 0; i < 24; ++i)
		assertTrue (0 == pArray[i]);
	std::free(pArray);
}


void ODBCTest::testBulkFullWidthStrings()
{
	if (!_pSession) failmsg ("Test not available.");

	// Bulk bound strings that fill their element of the parameter array, in the
	// first and in the last row: no terminator follows them. Every row is passed
	// with its length, so no value runs on into the next row or past the end of
	// the array.
	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);
	dropObject("TABLE", "BulkFullWidthStrings");
	session() << "CREATE TABLE BulkFullWidthStrings (Id INTEGER, Code CHAR(3), Txt VARCHAR(20))", now;

	std::vector<int> ids = {0, 1, 2};
	// as long as the column: the element length, if the driver reports the column size
	std::vector<std::string> codes = {"USD", "EUR", "JPY"};
	// as long as maxFieldSize: the element length, if the driver reports a larger size
	std::vector<std::string> texts = {"12345678", "abc", "ABCDEFGH"};
	const Poco::Any maxFieldSize = session().getProperty("maxFieldSize");
	session().setProperty("maxFieldSize", Poco::Any(std::size_t(8)));
	try
	{
		session() << "INSERT INTO BulkFullWidthStrings VALUES (?, ?, ?)",
			use(ids, bulk), use(codes, bulk), use(texts, bulk), now;
	}
	catch (...)
	{
		session().setProperty("maxFieldSize", maxFieldSize);
		throw;
	}
	// the columns of the query are bound with room for the terminator
	session().setProperty("maxFieldSize", maxFieldSize);

	std::vector<std::string> codesOut;
	std::vector<std::string> textsOut;
	session() << "SELECT Code, Txt FROM BulkFullWidthStrings ORDER BY Id", into(codesOut), into(textsOut), now;
	assertTrue (codesOut == codes);
	assertTrue (textsOut == texts);
	dropObject("TABLE", "BulkFullWidthStrings");
}


void ODBCTest::testBLOBStmt()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonBLOBTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->blobStmt();
		i += 2;
	}
}


void ODBCTest::testManualBLOB()
{
	if (!_pSession) failmsg ("Test not available.");

	// Manual extraction reads BLOB values in chunks with SQLGetData(), as CLOB
	// values. The values are longer than the BLOB object (19 bytes) and than
	// a chunk of 1024 bytes (2000 bytes); the first checks use the default
	// maxFieldSize of 1024 bytes.
	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", false);
	recreatePersonBLOBTable();
	std::vector<unsigned char> bytes(2000);
	for (std::size_t i = 0; i < bytes.size(); ++i)
		bytes[i] = static_cast<unsigned char>(i % 251);
	Poco::Data::BLOB small(&bytes[0], 19);
	Poco::Data::BLOB large(bytes);
	session() << "INSERT INTO Person VALUES ('a', 'small', 'x', ?)", use(small), now;
	session() << "INSERT INTO Person VALUES ('b', 'large', 'x', ?)", use(large), now;
	session() << "INSERT INTO Person (LastName, FirstName, Address) VALUES ('c', 'null', 'x')", now;

	const Poco::Any maxFieldSize = session().getProperty("maxFieldSize");
	try
	{
		session().setProperty("maxFieldSize", Poco::Any(std::size_t(1024)));
		Poco::Data::BLOB blob;
		session() << "SELECT Image FROM Person WHERE LastName = 'a'", into(blob), now;
		assertTrue (blob == small);

		Poco::Nullable<Poco::Data::BLOB> nullable;
		session() << "SELECT Image FROM Person WHERE LastName = 'a'", into(nullable), now;
		assertFalse (nullable.isNull());
		assertTrue (nullable.value() == small);
		session() << "SELECT Image FROM Person WHERE LastName = 'c'", into(nullable), now;
		assertTrue (nullable.isNull());

		Any any;
		session() << "SELECT Image FROM Person WHERE LastName = 'a'", into(any), now;
		assertTrue (AnyCast<Poco::Data::BLOB>(any) == small);
		Var var;
		session() << "SELECT Image FROM Person WHERE LastName = 'a'", into(var), now;
		assertTrue (var.extract<Poco::Data::BLOB>() == small);

		try
		{
			session() << "SELECT Image FROM Person WHERE LastName = 'b'", into(blob), now;
			fail ("BLOB longer than maxFieldSize extracted");
		}
		catch (Poco::Data::DataException& ex)
		{
			assertTrue (ex.message().find("exceeds maximum value") != std::string::npos);
		}

		session().setProperty("maxFieldSize", Poco::Any(std::size_t(4096)));
		session() << "SELECT Image FROM Person WHERE LastName = 'b'", into(blob), now;
		assertTrue (blob == large);

		Poco::Data::RecordSet rs(session(), "SELECT LastName, Image FROM Person ORDER BY LastName");
		assertTrue (3 == rs.rowCount());
		assertTrue (rs.value<Poco::Data::BLOB>(1, 0) == small);
		assertTrue (rs.value<Poco::Data::BLOB>(1, 1) == large);
		assertTrue (rs.isNull(1, 2));
	}
	catch (...)
	{
		session().setProperty("maxFieldSize", maxFieldSize);
		throw;
	}
	session().setProperty("maxFieldSize", maxFieldSize);
}


template <typename S>
void ODBCTest::checkManualString(const std::string& query, std::size_t length, std::size_t maxFieldSize)
{
	S str;
	if (length * sizeof(typename S::value_type) <= maxFieldSize)
	{
		session() << query, into(str), now;
		assertTrue (str == S(length, 'x'));
	}
	else
	{
		try
		{
			session() << query, into(str), now;
			fail ("string longer than maxFieldSize extracted");
		}
		catch (Poco::Data::DataException& ex)
		{
			assertTrue (ex.message().find("exceeds maximum value") != std::string::npos);
		}
	}
}


void ODBCTest::testManualStringChunks()
{
	if (!_pSession) failmsg ("Test not available.");

	// Manual extraction reads strings in chunks with SQLGetData() into a
	// buffer of 1024 bytes, which holds 1023 characters or 511 UTF-16 code
	// units and the terminator. The lengths are around one and two chunks;
	// a value of 1024 bytes fits into the default maxFieldSize of 1024 bytes.
	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", false);
	dropObject("TABLE", "ManualStringChunks");
	session() << "CREATE TABLE ManualStringChunks (Id INTEGER, Str VARCHAR(4000))", now;
	const std::size_t lengths[] = {511, 512, 513, 1023, 1024, 1025, 2047};
	for (std::size_t length: lengths)
	{
		session() << format("INSERT INTO ManualStringChunks VALUES (%z, '%s')", length, std::string(length, 'x')), now;
	}

	const Poco::Any maxFieldSize = session().getProperty("maxFieldSize");
	try
	{
		for (std::size_t fieldSize: {std::size_t(1024), std::size_t(4096)})
		{
			session().setProperty("maxFieldSize", Poco::Any(fieldSize));
			for (std::size_t length: lengths)
			{
				const std::string query = format("SELECT Str FROM ManualStringChunks WHERE Id = %z", length);
				checkManualString<std::string>(query, length, fieldSize);
				checkManualString<Poco::UTF16String>(query, length, fieldSize);
			}
		}
	}
	catch (...)
	{
		session().setProperty("maxFieldSize", maxFieldSize);
		throw;
	}
	session().setProperty("maxFieldSize", maxFieldSize);
	dropObject("TABLE", "ManualStringChunks");
}


void ODBCTest::testRecordSet()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonDateTimeTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->recordSet();
		i += 2;
	}
}


void ODBCTest::testRecordSetBeforeExecute()
{
	if (_pSession == nullptr) failmsg ("Test not available.");

	recreatePersonTable();
	_pExecutor->recordSetBeforeExecute();
}


void ODBCTest::testDateTime()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonDateTimeTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->dateTime();
		i += 2;
	}
}


void ODBCTest::testDate()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonDateTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->date();
		i += 2;
	}
}


void ODBCTest::testTime()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTimeTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->time();
		i += 2;
	}
}


void ODBCTest::testFloat()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateFloatsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->floats();
		i += 2;
	}
}


void ODBCTest::testDouble()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateFloatsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->doubles();
		i += 2;
	}
}


void ODBCTest::testUUID()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateUUIDsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->uuids();
		i += 2;
	}
}


void ODBCTest::testTuple()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateTuplesTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->tuples();
		i += 2;
	}
}


void ODBCTest::testTupleVector()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateTuplesTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->tupleVector();
		i += 2;
	}
}


void ODBCTest::testInternalExtraction()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateVectorsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->internalExtraction();
		i += 2;
	}
}


void ODBCTest::testFilter()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateVectorsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->filter();
		i += 2;
	}
}


void ODBCTest::testInternalBulkExtraction()
{
	if (!_pSession) failmsg ("Test not available.");

	recreatePersonTable();
	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);
#ifdef POCO_ODBC_UNICODE
	_pExecutor->internalBulkExtractionUTF16();
#else
	_pExecutor->internalBulkExtraction();
#endif
}


void ODBCTest::testInternalStorageType()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateVectorsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->internalStorageType();
		i += 2;
	}
}


void ODBCTest::testNull()
{
	if (!_pSession) failmsg ("Test not available.");

	// test for NOT NULL violation exception
	for (int i = 0; i < 8;)
	{
		recreateNullsTable("NOT NULL");
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->notNulls();
		i += 2;
	}

	// test for null insertion
	for (int i = 0; i < 8;)
	{
		recreateNullsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->nulls();
		i += 2;
	}
}


void ODBCTest::testRowIterator()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateVectorsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->rowIterator();
		i += 2;
	}
}


void ODBCTest::testStdVectorBool()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateBoolTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->stdVectorBool();
		i += 2;
	}
}


void ODBCTest::testAsync()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->asynchronous(2000);
		i += 2;
	}
}


void ODBCTest::testAny()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateAnysTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->any();

		i += 2;
	}
}


void ODBCTest::testDynamicAny()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateAnysTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->dynamicAny();

		i += 2;
	}
}


namespace
{
	DateTime toDateTime(const Any& any)
		// Returns the value of a DATE or TIMESTAMP column extracted into any.
		// Drivers can describe DATE columns as timestamps.
	{
		if (any.type() == typeid(Poco::Data::Date))
		{
			const Poco::Data::Date& date = Poco::RefAnyCast<Poco::Data::Date>(any);
			return DateTime(date.year(), date.month(), date.day());
		}
		return AnyCast<DateTime>(any);
	}
}


void ODBCTest::testAnyDateTime()
{
	if (!_pSession) failmsg ("Test not available.");

	// Bound extraction into Poco::Any and Poco::Dynamic::Var binds DATE and
	// TIMESTAMP columns as the ODBC structures the extraction of
	// Poco::Data::Date and Poco::DateTime reads. Containers that are not
	// bulk-extracted are extracted row by row in the same way.
	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);
	recreateNullableTable();
	DateTime dateTime(2001, 2, 3, 4, 5, 6);
	Poco::Data::Date date(2001, 2, 3);
	session() << "INSERT INTO NullableTest (EmptyInteger, EmptyDateTime, EmptyDate) VALUES (1, ?, ?)",
		use(dateTime), use(date), now;
	session() << "INSERT INTO NullableTest (EmptyInteger) VALUES (2)", now;

	Any anyDateTime;
	Any anyDate;
	session() << "SELECT EmptyDateTime, EmptyDate FROM NullableTest WHERE EmptyInteger = 1",
		into(anyDateTime), into(anyDate), now;
	assertTrue (toDateTime(anyDateTime) == dateTime);
	assertTrue (Poco::Data::Date(toDateTime(anyDate)) == date);

	Var varDateTime;
	Var varDate;
	session() << "SELECT EmptyDateTime, EmptyDate FROM NullableTest WHERE EmptyInteger = 1",
		into(varDateTime), into(varDate), now;
	assertTrue (varDateTime.convert<DateTime>() == dateTime);
	assertTrue (Poco::Data::Date(varDate.convert<DateTime>()) == date);

	Poco::Nullable<Any> nullableAny;
	Poco::Nullable<Var> nullableVar;
	session() << "SELECT EmptyDateTime, EmptyDate FROM NullableTest WHERE EmptyInteger = 1",
		into(nullableAny), into(nullableVar), now;
	assertFalse (nullableAny.isNull());
	assertTrue (toDateTime(nullableAny.value()) == dateTime);
	assertFalse (nullableVar.isNull());
	assertTrue (Poco::Data::Date(nullableVar.value().convert<DateTime>()) == date);
	session() << "SELECT EmptyDateTime, EmptyDate FROM NullableTest WHERE EmptyInteger = 2",
		into(nullableAny), into(nullableVar), now;
	assertTrue (nullableAny.isNull());
	assertTrue (nullableVar.isNull());

	std::vector<Any> anyDateTimes;
	std::vector<Var> varDates;
	session() << "SELECT EmptyDateTime, EmptyDate FROM NullableTest ORDER BY EmptyInteger",
		into(anyDateTimes), into(varDates), now;
	assertTrue (2 == anyDateTimes.size());
	assertTrue (toDateTime(anyDateTimes[0]) == dateTime);
	assertTrue (anyDateTimes[1].empty());
	assertTrue (2 == varDates.size());
	assertTrue (Poco::Data::Date(varDates[0].convert<DateTime>()) == date);
	assertTrue (varDates[1].isEmpty());
}


void ODBCTest::testAnyTime()
{
	if (!_pSession) failmsg ("Test not available.");

	// TIME columns are bound as the ODBC structure the extraction of
	// Poco::Data::Time reads.
	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);
	recreatePersonTimeTable();
	Poco::Data::Time timeOfDay(4, 5, 6);
	session() << "INSERT INTO Person VALUES ('a', 'b', 'c', ?)", use(timeOfDay), now;
	session() << "INSERT INTO Person (LastName) VALUES ('n')", now;

	Any any;
	Var var;
	session() << "SELECT BornTime, BornTime FROM Person WHERE LastName = 'a'", into(any), into(var), now;
	assertTrue (AnyCast<Poco::Data::Time>(any) == timeOfDay);
	assertTrue (var.extract<Poco::Data::Time>() == timeOfDay);

	Poco::Nullable<Any> nullableAny;
	Poco::Nullable<Var> nullableVar;
	session() << "SELECT BornTime, BornTime FROM Person WHERE LastName = 'n'", into(nullableAny), into(nullableVar), now;
	assertTrue (nullableAny.isNull());
	assertTrue (nullableVar.isNull());
}


void ODBCTest::testAnyUUID()
{
	if (!_pSession) failmsg ("Test not available.");

	// GUID columns are bound as the 16-byte buffer the extraction of
	// Poco::UUID reads.
	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);
	recreateUUIDsTable();
	Poco::UUID uuid("49cf6461-9b62-4163-9659-5472ef73153d");
	session() << "INSERT INTO Strings VALUES (?)", use(uuid), now;
	session() << "INSERT INTO Strings VALUES (NULL)", now;

	Any any;
	Var var;
	session() << "SELECT str, str FROM Strings WHERE str IS NOT NULL", into(any), into(var), now;
	assertTrue (AnyCast<Poco::UUID>(any) == uuid);
	assertTrue (var.extract<Poco::UUID>() == uuid);

	Poco::Nullable<Any> nullableAny;
	Poco::Nullable<Var> nullableVar;
	session() << "SELECT str, str FROM Strings WHERE str IS NULL", into(nullableAny), into(nullableVar), now;
	assertTrue (nullableAny.isNull());
	assertTrue (nullableVar.isNull());
}


template <typename C>
void ODBCTest::checkBulkAnyRejected(const std::string& query)
{
	C values;
	try
	{
		session() << query, into(values, bulk(2)), now;
		fail ("bulk extraction into a container of Poco::Any or Poco::Dynamic::Var executed");
	}
	catch (Poco::NotImplementedException&)
	{
	}
}


void ODBCTest::testAnyBulk()
{
	if (!_pSession) failmsg ("Test not available.");

	// Bulk extraction into containers of Poco::Any and Poco::Dynamic::Var is
	// not supported. The statement is rejected before any column is bound.
	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);
	recreateNullableTable();
	session() << "INSERT INTO NullableTest (EmptyInteger) VALUES (1)", now;
	session() << "INSERT INTO NullableTest (EmptyInteger) VALUES (2)", now;
	const std::string query("SELECT EmptyInteger FROM NullableTest");
	checkBulkAnyRejected<std::vector<Any>>(query);
	checkBulkAnyRejected<std::deque<Any>>(query);
	checkBulkAnyRejected<std::list<Any>>(query);
	checkBulkAnyRejected<std::vector<Var>>(query);
	checkBulkAnyRejected<std::deque<Var>>(query);
	checkBulkAnyRejected<std::list<Var>>(query);
}


void ODBCTest::testAnyBoundType()
{
	if (!_pSession) failmsg ("Test not available.");

	// Bound extraction into Poco::Any and Poco::Dynamic::Var reads the buffer as
	// the type the Preparator bound it for, which the driver described before
	// the execution. A driver can describe a column differently after the
	// fetch; the SQLite driver does so for COUNT(*). Here the Extractor
	// describes the column of a second statement, an integer, while the buffer
	// was bound for the character column of the first one.
	_pSession->setFeature("autoBind", true);
	_pSession->setFeature("autoExtract", true);
	recreateNullableTable();
	session() << "INSERT INTO NullableTest (EmptyString, EmptyInteger) VALUES ('abc', 7)", now;

	auto* pImpl = dynamic_cast<Poco::Data::ODBC::SessionImpl*>(session().impl());
	assertNotNullPtr (pImpl);
	const Poco::Data::ODBC::StatementHandle stringStmt(pImpl->dbc());
	const Poco::Data::ODBC::StatementHandle integerStmt(pImpl->dbc());

	Poco::Data::ODBC::Preparator::Ptr pPreparator =
		new Poco::Data::ODBC::Preparator(stringStmt, "SELECT EmptyString FROM NullableTest", 1024);
	assertTrue (1 == pPreparator->columns());
	pPreparator->prepare(0, Var());
	assertFalse (Utility::isError(SQLExecute(stringStmt)));
	assertFalse (Utility::isError(SQLFetch(stringStmt)));
	// closes the cursor; the fetched value stays in the bound buffer
	assertFalse (Utility::isError(SQLFreeStmt(stringStmt, SQL_CLOSE)));

	Poco::Data::ODBC::Preparator integerPreparator(integerStmt, "SELECT EmptyInteger FROM NullableTest", 1024);
	Poco::Data::ODBC::Extractor extractor(integerStmt, pPreparator);

	Var var;
	assertTrue (extractor.extract(0, var));
	assertTrue (var.convert<std::string>() == "abc");
	Any any;
	assertTrue (extractor.extract(0, any));
	// drivers can describe character columns as wide character columns
	std::string str;
	if (any.type() == typeid(Poco::UTF16String))
		Poco::UnicodeConverter::convert(AnyCast<Poco::UTF16String>(any), str);
	else
		str = AnyCast<std::string>(any);
	assertTrue (str == "abc");

	// typed extraction checks the type of the bound buffer
	Poco::Int32 integer = 0;
	try
	{
		extractor.extract(0, integer);
		fail ("Int32 extracted from a character buffer");
	}
	catch (Poco::BadCastException&)
	{
	}
	Poco::Data::Date date;
	try
	{
		extractor.extract(0, date);
		fail ("Date extracted from a character buffer");
	}
	catch (Poco::BadCastException&)
	{
	}
	Poco::Data::Time timeOfDay;
	try
	{
		extractor.extract(0, timeOfDay);
		fail ("Time extracted from a character buffer");
	}
	catch (Poco::BadCastException&)
	{
	}
	DateTime dateTime;
	try
	{
		extractor.extract(0, dateTime);
		fail ("DateTime extracted from a character buffer");
	}
	catch (Poco::BadCastException&)
	{
	}
}


void ODBCTest::testMultipleResults()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->multipleResults();

		i += 2;
	}
}


void ODBCTest::testSQLChannel()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateLogTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->sqlChannel(_rConnectString);

		i += 2;
	}
}


void ODBCTest::testSQLLogger()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateLogTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->sqlLogger(_rConnectString);

		i += 2;
	}
}


void ODBCTest::testAutoCommit()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->autoCommit(_rConnectString);
		i += 2;
	}
}


void ODBCTest::testTransactionIsolation()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->transactionIsolation();
		i += 2;
	}
}


void ODBCTest::testSessionTransaction()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->sessionTransaction(_rConnectString);
		i += 2;
	}
}


void ODBCTest::testSessionTransactionNoAutoCommit()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->sessionTransactionNoAutoCommit(_rConnectString);
		i += 2;
	}
}


void ODBCTest::testTransaction()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->transaction(_rConnectString, _readUncommitted);
		i += 2;
	}
}


void ODBCTest::testTransactor()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->transactor();
		i += 2;
	}
}


void ODBCTest::testNullable()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateNullableTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->nullable();
		i += 2;
	}
}


void ODBCTest::testStdOptional()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateNullableStringTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->stdOptional();
		i += 2;
	}
}


void ODBCTest::testStdTupleWithOptional()
{
	if (!_pSession) failmsg ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateNullableStringTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->stdTupleWithOptional();
		i += 2;
	}
}


void ODBCTest::testUnicode()
{
#if defined (POCO_ODBC_UNICODE)
	if (!_pSession) failmsg("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateUnicodeTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i + 1));
		_pExecutor->unicode(_rConnectString);
		i += 2;
	}
#else
	std::cout << "Not an UNICODE build, skipping." << std::endl;
#endif
}


void ODBCTest::testEncoding()
{
#if defined (POCO_ODBC_UNICODE)
	if (!_pSession) failmsg("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateEncodingTables();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i + 1));
		_pExecutor->encoding(_rConnectString);
		i += 2;
	}
#else
	std::cout << "Not an UNICODE build, skipping." << std::endl;
#endif
}


void ODBCTest::testReconnect()
{
	if (!_pSession) failmsg ("Test not available.");

	std::string tableName("Person");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValue(i));
		_pSession->setFeature("autoExtract", bindValue(i+1));
		_pExecutor->reconnect();
		i += 2;
	}
}


bool ODBCTest::canConnect(const std::string& driver,
	std::string& dsn,
	std::string& uid,
	std::string& pwd,
	std::string& dbConnString,
	const std::string& db,
	bool quiet)
{
	Utility::DriverMap::iterator itDrv = _drivers.begin();
	for (; itDrv != _drivers.end(); ++itDrv)
	{
		if (((itDrv->first).find(driver) != std::string::npos))
		{
			std::cout << "Driver found: " << itDrv->first
				<< " (" << itDrv->second << ')' << std::endl;
			break;
		}
	}

	if (_drivers.end() == itDrv)
	{
		dsn = "";
		uid = "";
		pwd = "";
		dbConnString = "";
		if (!quiet)
			std::cout << driver << " driver NOT found, tests not available." << std::endl;
		return false;
	}

	Utility::DSNMap dataSources;
	(void) Utility::dataSources(dataSources);
	if (dataSources.size() > 0)
	{
		Utility::DSNMap::iterator itDSN = dataSources.begin();
		std::cout << dataSources.size() << " DSNs found, enumerating ..." << std::endl;
		for (; itDSN != dataSources.end(); ++itDSN)
		{
			if (itDSN->first == dsn && itDSN->second == driver)
			{
				std::cout << "DSN found: " << itDSN->first
					<< " (" << itDSN->second << ')' << std::endl;

				dbConnString = format("DSN=%s;UID=%s;PWD=%s;", dsn, uid, pwd);
				if (!db.empty())
					format(dbConnString, "DATABASE=%s;", db);

				return true;
			}
		}
	}
	else
		std::cout << "No DSNs found, will attempt DSN-less connection ..." << std::endl;

	dsn = "";
	return true;
}


void ODBCTest::setUp()
{
}


void ODBCTest::tearDown()
{
}


ODBCTest::SessionPtr ODBCTest::init(const std::string& driver,
	std::string& dsn,
	std::string& uid,
	std::string& pwd,
	std::string& dbConnString,
	const std::string& db,
	const std::string& dbEncoding,
	bool quiet)
{
	Utility::drivers(_drivers);
	if (!canConnect(driver, dsn, uid, pwd, dbConnString, db, quiet)) return nullptr;

	try
	{
		std::cout << "Connecting to [" << dbConnString << ']' << std::endl;
		SessionPtr ptr = new Session(Poco::Data::ODBC::Connector::KEY, dbConnString, 5);
		if (!dbEncoding.empty())
			ptr->setProperty("dbEncoding", dbEncoding);
		return ptr;
	}catch (ConnectionFailedException& ex)
	{
		std::cout << ex.displayText() << std::endl;
		return nullptr;
	}
}
