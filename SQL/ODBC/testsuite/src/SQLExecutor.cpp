//
// SQLExecutor.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppUnit/TestCase.h"
#include "SQLExecutor.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Nullable.h"
#include "Poco/Any.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/DateTime.h"
#include "Poco/Stopwatch.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Thread.h"
#include "Poco/Logger.h"
#include "Poco/Message.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/SharedPtr.h"
#include "Poco/Exception.h"
#include "Poco/SQL/Date.h"
#include "Poco/SQL/Time.h"
#include "Poco/SQL/LOB.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SQL/RowIterator.h"
#include "Poco/SQL/RowFilter.h"
#include "Poco/SQL/BulkExtraction.h"
#include "Poco/SQL/BulkBinding.h"
#include "Poco/SQL/SQLChannel.h"
#include "Poco/SQL/Transaction.h"
#include "Poco/SQL/ODBC/Connector.h"
#include "Poco/SQL/ODBC/Utility.h"
#include "Poco/SQL/ODBC/Diagnostics.h"
#include "Poco/SQL/ODBC/Error.h"
#include "Poco/SQL/ODBC/Preparator.h"
#include "Poco/SQL/ODBC/ODBCException.h"
#include "Poco/SQL/ODBC/ODBCStatementImpl.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/UTFString.h"
#include "Poco/Environment.h"
#include "Poco/Checksum.h"
#include <sqltypes.h>
#include <iostream>
#include <sstream>
#include <iterator>


using namespace Poco::SQL::Keywords;
using Poco::SQL::Session;
using Poco::SQL::Statement;
using Poco::SQL::RecordSet;
using Poco::SQL::Column;
using Poco::SQL::Row;
using Poco::SQL::RowFilter;
using Poco::SQL::RowIterator;
using Poco::SQL::SQLChannel;
using Poco::SQL::LimitException;
using Poco::SQL::BindingException;
using Poco::SQL::LengthExceededException;
using Poco::SQL::CLOB;
using Poco::SQL::Date;
using Poco::SQL::Time;
using Poco::SQL::Transaction;
using Poco::SQL::NotConnectedException;
using Poco::SQL::ODBC::Utility;
using Poco::SQL::ODBC::Preparator;
using Poco::SQL::ODBC::ConnectionException;
using Poco::SQL::ODBC::StatementException;
using Poco::SQL::ODBC::DataTruncatedException;
using Poco::SQL::ODBC::StatementError;
using Poco::format;
using Poco::Tuple;
using Poco::Nullable;
using Poco::Any;
using Poco::AnyCast;
using Poco::Dynamic::Var;
using Poco::DateTime;
using Poco::Stopwatch;
using Poco::NumberFormatter;
using Poco::AutoPtr;
using Poco::Thread;
using Poco::Logger;
using Poco::Message;
using Poco::NotFoundException;
using Poco::InvalidAccessException;
using Poco::InvalidArgumentException;
using Poco::NotImplementedException;
using Poco::BadCastException;
using Poco::RangeException;
using Poco::TimeoutException;
using Poco::UnicodeConverter;
using Poco::UTF16String;
using Poco::UTF32String;



static
std::string idGen()
{
	std::string host = Poco::Environment::nodeName() + "_" +
#if defined(POCO_OS_FAMILY_WINDOWS)
	Poco::Environment::get("USERNAME");
#else
	Poco::Environment::get("USER", "U");
#endif
	if (host.length() > 12) //Sybase has got 30 char length limit
	{
		Poco::Checksum crc;
		crc.update(host);
		host = Poco::format("%s%LX", host.substr(0, 4), crc.checksum());
	}
	std::replace(host.begin(), host.end(), '.', '_');
	std::replace(host.begin(), host.end(), '-', '_');
	return host;
}

std::string ExecUtil::mangleTable(const std::string& name)
{
	static std::string id = idGen();
	const std::string nm = "pt_" + name + id;
	poco_assert_dbg(nm.length() <= 30);
	return nm;
}

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
		// we choose the lastName as the key
		return lastName;
	}
};


struct RefCountedPerson : public Poco::RefCountedObject
{
	std::string lastName;
	std::string firstName;
	std::string address;
	int age;
	RefCountedPerson(){age = 0;}
	RefCountedPerson(const std::string& ln, const std::string& fn, const std::string& adr, int a):lastName(ln), firstName(fn), address(adr), age(a)
	{
	}
	bool operator==(const Person& other) const
	{
		return lastName == other.lastName && firstName == other.firstName && address == other.address && age == other.age;
	}

	bool operator < (const RefCountedPerson& p) const
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
		// we choose the lastName as the key
		return lastName;
	}

private:
	RefCountedPerson(const RefCountedPerson &);
	RefCountedPerson& operator = (const RefCountedPerson&);
};

#define assertTU(tu, condition) \
	(tu->assertImpl((condition), (#condition), __LINE__, __FILE__))

#define failTU(tu, message) \
	(tu->failImpl(message, __LINE__, __FILE__))

namespace Poco {
namespace SQL {


template <>
class TypeHandler<Person>
{
public:
	static void bind(std::size_t pos,
		const Person& obj,
		AbstractBinder::Ptr pBinder,
		AbstractBinder::Direction dir = AbstractBinder::PD_IN)
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
	TypeHandler(const TypeHandler&);
	TypeHandler& operator=(const TypeHandler&);
};


template <>
class TypeHandler<RefCountedPerson>
{
public:
	static void bind(std::size_t pos,
		const RefCountedPerson& obj,
		AbstractBinder::Ptr pBinder,
		AbstractBinder::Direction dir = AbstractBinder::PD_IN)
	{
		// the table is defined as Person (LastName VARCHAR(30), FirstName VARCHAR, Address VARCHAR, Age INTEGER(3))
		poco_assert_dbg (!pBinder.isNull());
		pBinder->bind(pos++, obj.lastName, dir);
		pBinder->bind(pos++, obj.firstName, dir);
		pBinder->bind(pos++, obj.address, dir);
		pBinder->bind(pos++, obj.age, dir);
	}

	static void prepare(std::size_t pos, RefCountedPerson& obj, AbstractPreparator::Ptr pPrepare)
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

	static void extract(std::size_t pos, RefCountedPerson& obj, const RefCountedPerson& defVal, AbstractExtractor::Ptr pExt)
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
	TypeHandler(const TypeHandler&);
	TypeHandler& operator=(const TypeHandler&);
};


} } // namespace Poco::SQL


const std::string SQLExecutor::MULTI_INSERT =
	"INSERT INTO " +ExecUtil::test_tbl() + " VALUES ('1', 2, 3.5);" +
	"INSERT INTO " +ExecUtil::test_tbl() + " VALUES ('2', 3, 4.5);" +
	"INSERT INTO " +ExecUtil::test_tbl() + " VALUES ('3', 4, 5.5);" +
	"INSERT INTO " +ExecUtil::test_tbl() + " VALUES ('4', 5, 6.5);" +
	"INSERT INTO " +ExecUtil::test_tbl() + " VALUES ('5', 6, 7.5);";

const std::string SQLExecutor::MULTI_SELECT =
	"SELECT * FROM " +ExecUtil::test_tbl() + " WHERE First = '1';" +
	"SELECT * FROM " +ExecUtil::test_tbl() + " WHERE First = '2';" +
	"SELECT * FROM " +ExecUtil::test_tbl() + " WHERE First = '3';" +
	"SELECT * FROM " +ExecUtil::test_tbl() + " WHERE First = '4';" +
	"SELECT * FROM " +ExecUtil::test_tbl() + " WHERE First = '5';";


SQLExecutor::SQLExecutor(const std::string& name, Poco::SQL::Session* pSession, const std::string& connInitSql, const std::string& schemaName) :
	CppUnit::TestCase(name),
	_pSession(pSession),
	_connInitSql(connInitSql),
	_schemaName(schemaName)
{
}


SQLExecutor::~SQLExecutor()
{
}


void SQLExecutor::bareboneODBCTest(const std::string& dbConnString,
	const std::string& tableCreateString,
	SQLExecutor::DataBinding bindMode,
	SQLExecutor::DataExtraction extractMode,
	bool doTime,
	const std::string& blobPlaceholder)
{
	SQLRETURN rc;
	SQLHENV henv = SQL_NULL_HENV;
	SQLHDBC hdbc = SQL_NULL_HDBC;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;

	// Environment begin
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	poco_odbc_check_env (rc, henv);
	rc = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);
	poco_odbc_check_env (rc, henv);

		// Connection begin
		rc = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
		poco_odbc_check_dbc (rc, hdbc);

		SQLCHAR connectOutput[1024] = {0};
		SQLSMALLINT result;
		rc = SQLDriverConnect(hdbc
			, NULL
			,(SQLCHAR*) dbConnString.c_str()
			,(SQLSMALLINT) SQL_NTS
			, connectOutput
			, sizeof(connectOutput)
			, &result
			, SQL_DRIVER_NOPROMPT);
		poco_odbc_check_dbc (rc, hdbc);

		// retrieve datetime type information for this DBMS
		rc = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
		poco_odbc_check_stmt (rc, hstmt);

		rc = SQLGetTypeInfo(hstmt, SQL_TYPE_TIMESTAMP);
		poco_odbc_check_stmt (rc, hstmt);
		
		rc = SQLFetch(hstmt);
		assertTrue (SQL_SUCCEEDED(rc) || SQL_NO_DATA == rc);

		SQLULEN dateTimeColSize = 0;
		SQLSMALLINT dateTimeDecDigits = 0;
		if (SQL_SUCCEEDED(rc))
		{
			SQLLEN ind = 0;
			rc = SQLGetData(hstmt, 3, SQL_C_SLONG, &dateTimeColSize, sizeof(SQLINTEGER), &ind);
			poco_odbc_check_stmt (rc, hstmt);
			rc = SQLGetData(hstmt, 14, SQL_C_SSHORT, &dateTimeDecDigits, sizeof(SQLSMALLINT), &ind);
			poco_odbc_check_stmt (rc, hstmt);

			assertTrue (sizeof(SQL_TIMESTAMP_STRUCT) <= dateTimeColSize);
		}
		else if (SQL_NO_DATA == rc)
			std::cerr << '[' << name() << ']' << " Warning: no SQL_TYPE_TIMESTAMP data type info returned by driver." << std::endl;

		rc = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		poco_odbc_check_stmt (rc, hstmt);

		if (!_connInitSql.empty())
		{
			rc = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
			poco_odbc_check_stmt(rc, hstmt);
			SQLCHAR* pStr = (SQLCHAR*)_connInitSql.c_str();
			SQLExecDirect(hstmt, pStr, (SQLINTEGER)_connInitSql.length());
		}

			// Statement begin
			rc = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			std::string sql = "DROP TABLE " + ExecUtil::test_tbl();
			SQLCHAR* pStr = (SQLCHAR*) sql.c_str();
			SQLExecDirect(hstmt, pStr, (SQLINTEGER) sql.length());
			//no return code check - ignore drop errors

			// create table and go
			sql = tableCreateString;
			pStr = (SQLCHAR*) sql.c_str();
			rc = SQLPrepare(hstmt, pStr, (SQLINTEGER) sql.length());
			poco_odbc_check_stmt (rc, hstmt);

			rc = SQLExecute(hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			sql = format("INSERT INTO " + ExecUtil::test_tbl() + " VALUES(? , ? , %s, ? , ? , ? )", blobPlaceholder);
			pStr = (SQLCHAR*) sql.c_str();
			rc = SQLPrepare(hstmt, pStr, (SQLINTEGER) sql.length());
			poco_odbc_check_stmt (rc, hstmt);

			std::string str[3] = { "11111111111", "222222222222222222222222", "333333333333333333333333333" };
			int fourth = 4;
			float fifth = 1.5;
			SQL_TIMESTAMP_STRUCT sixth;
			sixth.year = 1965;
			sixth.month = 6;
			sixth.day = 18;
			sixth.hour = 5;
			sixth.minute = 34;
			sixth.second = 58;
			// Fraction support is limited to milliseconds due to MS SQL Server limitation
			// see http://support.microsoft.com/kb/263872
			sixth.fraction = 997000000;

			SQLLEN li[3] = { SQL_NTS, SQL_NTS, 0 };
			SQLINTEGER size = (SQLINTEGER) str[0].size();

			if (SQLExecutor::PB_AT_EXEC == bindMode)
				li[0] = SQL_LEN_DATA_AT_EXEC(size);

			rc = SQLBindParameter(hstmt,
				(SQLUSMALLINT) 1,
				SQL_PARAM_INPUT,
				SQL_C_CHAR,
				SQL_LONGVARCHAR,
				(SQLUINTEGER) size,
				0,
				(SQLPOINTER) str[0].c_str(),
				size,
				&li[0]);
			poco_odbc_check_stmt (rc, hstmt);

			size = (SQLINTEGER) str[1].size();
			if (SQLExecutor::PB_AT_EXEC == bindMode)
				li[1] = SQL_LEN_DATA_AT_EXEC(size);
			else li[1] = SQL_NTS;

			rc = SQLBindParameter(hstmt,
				(SQLUSMALLINT) 2,
				SQL_PARAM_INPUT,
				SQL_C_CHAR,
				SQL_LONGVARCHAR,
				(SQLUINTEGER) size,
				0,
				(SQLPOINTER) str[1].c_str(),
				size,
				&li[1]);
			poco_odbc_check_stmt (rc, hstmt);

			size = (SQLINTEGER) str[2].size();
			if (SQLExecutor::PB_AT_EXEC == bindMode)
				li[2] = SQL_LEN_DATA_AT_EXEC(size);
			else li[2] = size;

			rc = SQLBindParameter(hstmt,
				(SQLUSMALLINT) 3,
				SQL_PARAM_INPUT,
				SQL_C_BINARY,
				SQL_LONGVARBINARY,
				(SQLUINTEGER) size,
				0,
				(SQLPOINTER) str[2].data(),
				size,
				&li[2]);
			poco_odbc_check_stmt (rc, hstmt);

			rc = SQLBindParameter(hstmt,
				(SQLUSMALLINT) 4,
				SQL_PARAM_INPUT,
				SQL_C_SLONG,
				SQL_INTEGER,
				0,
				0,
				(SQLPOINTER) &fourth,
				0,
				0);
			poco_odbc_check_stmt (rc, hstmt);

			rc = SQLBindParameter(hstmt,
				(SQLUSMALLINT) 5,
				SQL_PARAM_INPUT,
				SQL_C_FLOAT,
				SQL_REAL,
				0,
				1,
				(SQLPOINTER) &fifth,
				0,
				0);
			poco_odbc_check_stmt (rc, hstmt);

			SQLSMALLINT dataType = 0;
			SQLULEN parameterSize = 0;
			SQLSMALLINT decimalDigits = 0;
			SQLSMALLINT nullable = 0;
			rc = SQLDescribeParam(hstmt, 6, &dataType, &parameterSize, &decimalDigits, &nullable);
			if (SQL_SUCCEEDED(rc))
			{
				if (parameterSize)
					dateTimeColSize = parameterSize;
				if (decimalDigits)
					dateTimeDecDigits = decimalDigits;
			}
			else
				std::cerr << '[' << name() << ']' << " Warning: could not get SQL_TYPE_TIMESTAMP parameter description." << std::endl;

			rc = SQLBindParameter(hstmt,
				(SQLUSMALLINT) 6,
				SQL_PARAM_INPUT,
				SQL_C_TYPE_TIMESTAMP,
				SQL_TYPE_TIMESTAMP,
				dateTimeColSize,
				dateTimeDecDigits,
				(SQLPOINTER) &sixth,
				0,
				0);
			poco_odbc_check_stmt (rc, hstmt);

			rc = SQLExecute(hstmt);
			assertTrue (SQL_NEED_DATA == rc || SQL_SUCCEEDED(rc));

			if (SQL_NEED_DATA == rc)
			{
				SQLPOINTER pParam = 0;
				while (SQL_NEED_DATA == (rc = SQLParamData(hstmt, &pParam)))
				{
					SQLINTEGER dataSize = 0;
					// Data size should be ignored for non-null,
					// non-variable length fields, but SQLite ODBC
					// driver insists on it always being the actual
					// data length

					if (pParam == (SQLPOINTER) str[0].c_str())
						dataSize = (SQLINTEGER) str[0].size();
					else if (pParam == (SQLPOINTER) str[1].c_str())
						dataSize = (SQLINTEGER) str[1].size();
					else if (pParam == (SQLPOINTER) str[2].c_str())
						dataSize = (SQLINTEGER) str[2].size();
					else if (pParam == (SQLPOINTER) &fourth)
						dataSize = (SQLINTEGER) sizeof(fourth);
					else if (pParam == (SQLPOINTER) &fifth)
						dataSize = (SQLINTEGER) sizeof(fifth);
					else if (pParam == (SQLPOINTER) &sixth)
						dataSize = (SQLINTEGER) sizeof(sixth);

					assertTrue (0 != dataSize);
					rc = SQLPutData(hstmt, pParam, dataSize);
					poco_odbc_check_stmt (rc, hstmt);
				}
			}
			poco_odbc_check_stmt (rc, hstmt);

			sql = "SELECT * FROM " + ExecUtil::test_tbl();
			pStr = (SQLCHAR*) sql.c_str();
			rc = SQLPrepare(hstmt, pStr, (SQLINTEGER) sql.length());
			poco_odbc_check_stmt (rc, hstmt);

			char chr[3][50] = {{ 0 }};
			SQLLEN lengths[6] = { 0 };
			fourth = 0;
			fifth = 0.0f;
			std::memset(&sixth, 0, sizeof(sixth));

			if (SQLExecutor::DE_BOUND == extractMode)
			{
				rc = SQLBindCol(hstmt,
					(SQLUSMALLINT) 1,
					SQL_C_CHAR,
					(SQLPOINTER) chr[0],
					(SQLINTEGER) sizeof(chr[0]),
					&lengths[0]);
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLBindCol(hstmt,
					(SQLUSMALLINT) 2,
					SQL_C_CHAR,
					(SQLPOINTER) chr[1],
					(SQLINTEGER) sizeof(chr[1]),
					&lengths[1]);
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLBindCol(hstmt,
					(SQLUSMALLINT) 3,
					SQL_C_BINARY,
					(SQLPOINTER) chr[2],
					(SQLINTEGER) sizeof(chr[2]),
					&lengths[2]);
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLBindCol(hstmt,
					(SQLUSMALLINT) 4,
					SQL_C_SLONG,
					(SQLPOINTER) &fourth,
					(SQLINTEGER) 0,
					&lengths[3]);
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLBindCol(hstmt,
					(SQLUSMALLINT) 5,
					SQL_C_FLOAT,
					(SQLPOINTER) &fifth,
					(SQLINTEGER) 0,
					&lengths[4]);
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLBindCol(hstmt,
					(SQLUSMALLINT) 6,
					SQL_C_TYPE_TIMESTAMP,
					(SQLPOINTER) &sixth,
					(SQLINTEGER) 0,
					&lengths[5]);
				poco_odbc_check_stmt (rc, hstmt);
			}
			
			rc = SQLExecute(hstmt);
			poco_odbc_check_stmt (rc, hstmt);
			rc = SQLFetch(hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			if (SQLExecutor::DE_MANUAL == extractMode)
			{
				SQLLEN len = lengths[0] = 0;
				while (SQL_SUCCESS_WITH_INFO == (rc = SQLGetData(hstmt,
					(SQLUSMALLINT) 1,
					SQL_C_CHAR,
					chr[0] + len,
					sizeof(chr[0]) - len,
					&lengths[0])))
				{
					len += lengths[0];
					if (!lengths[0] || len >= sizeof(chr[1]))
						break;
				}
				poco_odbc_check_stmt (rc, hstmt);

				len = lengths[1] = 0;
				while (SQL_SUCCESS_WITH_INFO == (rc = SQLGetData(hstmt,
					(SQLUSMALLINT) 2,
					SQL_C_CHAR,
					chr[1] + len,
					sizeof(chr[1]) - len,
					&lengths[1])))
				{
					len += lengths[1];
					if (!lengths[1] || len >= sizeof(chr[1]))
						break;
				}
				poco_odbc_check_stmt (rc, hstmt);

				len = lengths[2] = 0;
				while (SQL_SUCCESS_WITH_INFO == (rc = SQLGetData(hstmt,
					(SQLUSMALLINT) 3,
					SQL_C_BINARY,
					chr[2] + len,
					sizeof(chr[2]) - len,
					&lengths[2])))
				{
					len += lengths[1];
					if (!lengths[2] || len >= sizeof(chr[2]))
						break;
				}
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLGetData(hstmt,
					(SQLUSMALLINT) 4,
					SQL_C_SLONG,
					&fourth,
					0,
					&lengths[3]);
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLGetData(hstmt,
					(SQLUSMALLINT) 5,
					SQL_C_FLOAT,
					&fifth,
					0,
					&lengths[4]);
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLGetData(hstmt,
					(SQLUSMALLINT) 6,
					SQL_C_TYPE_TIMESTAMP,
					&sixth,
					0,
					&lengths[5]);
				poco_odbc_check_stmt (rc, hstmt);
			}

			assertTrue (0 == std::strncmp(str[0].c_str(), chr[0], str[0].size()));
			assertTrue (0 == std::strncmp(str[1].c_str(), chr[1], str[1].size()));
			assertTrue (0 == std::strncmp(str[2].c_str(), chr[2], str[2].size()));
			assertTrue (4 == fourth);
			assertTrue (1.5 == fifth);

			assertTrue (1965 == sixth.year);
			assertTrue (6 == sixth.month);
			assertTrue (18 == sixth.day);
			if (doTime)
			{
				assertTrue (5 == sixth.hour);
				assertTrue (34 == sixth.minute);
				if (sixth.fraction) // MySQL rounds fraction
				{
					assertTrue (58 == sixth.second);
					assertTrue (997000000 == sixth.fraction);
				}
				else
				{
					assertTrue (59 == sixth.second);
				}
			}

			rc = SQLCloseCursor(hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			sql = "DROP TABLE " + ExecUtil::test_tbl();
			pStr = (SQLCHAR*) sql.c_str();
			rc = SQLExecDirect(hstmt, pStr, (SQLINTEGER) sql.length());
			poco_odbc_check_stmt (rc, hstmt);

			rc = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			poco_odbc_check_stmt (rc, hstmt);

		// Connection end
		rc = SQLDisconnect(hdbc);
		poco_odbc_check_dbc (rc, hdbc);
		rc = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		poco_odbc_check_dbc (rc, hdbc);

	// Environment end
	rc = SQLFreeHandle(SQL_HANDLE_ENV, henv);
	poco_odbc_check_env (rc, henv);
}


void SQLExecutor::bareboneODBCMultiResultTest(const std::string& dbConnString,
	const std::string& tableCreateString,
	SQLExecutor::DataBinding bindMode,
	SQLExecutor::DataExtraction extractMode,
	const std::string& insert,
	const std::string& select)
{
	SQLRETURN rc;
	SQLHENV henv = SQL_NULL_HENV;
	SQLHDBC hdbc = SQL_NULL_HDBC;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;

	// Environment begin
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	poco_odbc_check_stmt (rc, hstmt);
	rc = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);
	poco_odbc_check_stmt (rc, hstmt);

		// Connection begin
		rc = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
		poco_odbc_check_stmt (rc, hstmt);

		SQLCHAR connectOutput[512] = {0};
		SQLSMALLINT result;
		rc = SQLDriverConnect(hdbc
			, NULL
			,(SQLCHAR*) dbConnString.c_str()
			,(SQLSMALLINT) SQL_NTS
			, connectOutput
			, sizeof(connectOutput)
			, &result
			, SQL_DRIVER_NOPROMPT);
		poco_odbc_check_stmt (rc, hstmt);

			// Statement begin
			rc = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			std::string sql = "DROP TABLE " + ExecUtil::test_tbl();
			SQLCHAR* pStr = (SQLCHAR*) sql.c_str();
			SQLExecDirect(hstmt, pStr, (SQLINTEGER) sql.length());
			//no return code check - ignore drop errors

			// create table and go
			sql = tableCreateString;
			pStr = (SQLCHAR*) sql.c_str();
			rc = SQLPrepare(hstmt, pStr, (SQLINTEGER) sql.length());
			poco_odbc_check_stmt (rc, hstmt);

			rc = SQLExecute(hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			// insert multiple rows
			pStr = (SQLCHAR*) insert.c_str();
			rc = SQLPrepare(hstmt, pStr, (SQLINTEGER) insert.length());
			poco_odbc_check_stmt (rc, hstmt);
			rc = SQLExecute(hstmt);
			poco_odbc_check_stmt (rc, hstmt);
			do
			{
				SQLLEN rowCount = 0;
				SQLRowCount(hstmt, &rowCount);
				assertTrue (1 == rowCount);
			} while (SQL_NO_DATA != SQLMoreResults(hstmt));

			// make sure all five rows made it in
			sql = "select count(*) from " + ExecUtil::test_tbl();
			int count = 0;
			SQLLEN length = 0;
			pStr = (SQLCHAR*) sql.c_str();
			rc = SQLPrepare(hstmt, pStr, (SQLINTEGER) sql.length());
			poco_odbc_check_stmt (rc, hstmt);
			if (SQLExecutor::DE_BOUND == extractMode)
			{
				rc = SQLBindCol(hstmt,
						(SQLUSMALLINT) 1,
						SQL_C_SLONG,
						(SQLPOINTER) &count,
						(SQLINTEGER) 0,
						&length);
				poco_odbc_check_stmt (rc, hstmt);
			}

			rc = SQLExecute(hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			rc = SQLFetch(hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			if (SQLExecutor::DE_MANUAL == extractMode)
			{
				rc = SQLGetData(hstmt,
					(SQLUSMALLINT) 1,
					SQL_C_SLONG,
					&count,
					0,
					&length);
				poco_odbc_check_stmt (rc, hstmt);
			}
			assertTrue (5 == count);

			rc = SQLCloseCursor(hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			// select multiple rows
			pStr = (SQLCHAR*) select.c_str();
			rc = SQLPrepare(hstmt, pStr, (SQLINTEGER) select.length());
			poco_odbc_check_stmt (rc, hstmt);

			char chr[5] = { 0 };
			SQLLEN lengths[3] = { 0 };
			int second = 0;
			float third = 0.0f;
			
			if (SQLExecutor::DE_BOUND == extractMode)
			{
				rc = SQLBindCol(hstmt,
					(SQLUSMALLINT) 1,
					SQL_C_CHAR,
					(SQLPOINTER) chr,
					(SQLINTEGER) 4,
					&lengths[0]);
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLBindCol(hstmt,
					(SQLUSMALLINT) 2,
					SQL_C_SLONG,
					(SQLPOINTER) &second,
					(SQLINTEGER) 0,
					&lengths[1]);
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLBindCol(hstmt,
					(SQLUSMALLINT) 3,
					SQL_C_FLOAT,
					(SQLPOINTER) &third,
					(SQLINTEGER) 0,
					&lengths[2]);
				poco_odbc_check_stmt (rc, hstmt);
			}
			
			rc = SQLExecute(hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			char one = 0x31;
			int two = 2;
			float three = 3.5;
			count = 0;

			do
			{
				rc = SQLFetch(hstmt);
				poco_odbc_check_stmt (rc, hstmt);

				if (SQLExecutor::DE_MANUAL == extractMode)
				{
					rc = SQLGetData(hstmt,
						(SQLUSMALLINT) 1,
						SQL_C_CHAR,
						chr,
						4,
						&lengths[0]);
					poco_odbc_check_stmt (rc, hstmt);

					rc = SQLGetData(hstmt,
						(SQLUSMALLINT) 2,
						SQL_C_SLONG,
						&second,
						0,
						&lengths[1]);
					poco_odbc_check_stmt (rc, hstmt);

					rc = SQLGetData(hstmt,
						(SQLUSMALLINT) 3,
						SQL_C_FLOAT,
						&third,
						0,
						&lengths[2]);
					poco_odbc_check_stmt (rc, hstmt);
				}

				assertTrue (one++ == chr[0]);
				assertTrue (two++ == second);
				assertTrue (three == third);
				three += 1.0;

				++count;
			} while (SQL_NO_DATA != SQLMoreResults(hstmt));

			assertTrue (5 == count);

			sql = "DROP TABLE " + ExecUtil::test_tbl();
			pStr = (SQLCHAR*) sql.c_str();
			rc = SQLExecDirect(hstmt, pStr, (SQLINTEGER) sql.length());
			poco_odbc_check_stmt (rc, hstmt);

			rc = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			poco_odbc_check_stmt (rc, hstmt);

		// Connection end
		rc = SQLDisconnect(hdbc);
		poco_odbc_check_stmt (rc, hstmt);
		rc = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		poco_odbc_check_stmt (rc, hstmt);

	// Environment end
	rc = SQLFreeHandle(SQL_HANDLE_ENV, henv);
	poco_odbc_check_stmt (rc, hstmt);
}


void SQLExecutor::execute(const std::string& sql)
{
	try { session() << sql, now;  }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (sql); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (sql); }
}


void SQLExecutor::zeroRows()
{
	Statement stmt = (session() << "SELECT * FROM " << ExecUtil::person() << " WHERE 0 = 1");
	assertTrue (0 == stmt.execute());
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastName), use(firstName), use(address), use(age), now;  }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	count = 0;
	try {
	  session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now;  }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 1);

	try { session() << "SELECT LastName FROM " << ExecUtil::person(), into(result), now;  }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (lastName == result);

	try { session() << "SELECT Age FROM " << ExecUtil::person(), into(count), now;  }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == age);
}


void SQLExecutor::complexType()
{
	std::string funct = "complexType()";
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);

	try { session() << "INSERT INTO " << ExecUtil::person() << " VALUES (?,?,?,?)", use(p1), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(p2), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	Person c1;
	try { session() << "SELECT * FROM " << ExecUtil::person() <<" WHERE LastName = 'LN1'", into(c1), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (c1 == p1);
}


void SQLExecutor::complexTypeTuple()
{
	std::string funct = "complexTypeTuple()";
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);

	Tuple<Person,Person> t(p1,p2);
	try { *_pSession << "INSERT INTO " << ExecUtil::person() <<" VALUES(?,?,?,?,?,?,?,?)", use(t), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	Tuple<Person,Person> ret;
	assertTrue (ret != t);
	try { *_pSession << "SELECT * FROM " << ExecUtil::person(), into(ret), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (ret == t);
}


void SQLExecutor::simpleAccessVector()
{
	std::string funct = "simpleAccessVector()";
	std::vector<std::string> lastNames;
	std::vector<std::string> firstNames;
	std::vector<std::string> addresses;
	std::vector<int> ages;
	std::string tableName(ExecUtil::person());
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::vector<std::string> lastNamesR;
	std::vector<std::string> firstNamesR;
	std::vector<std::string> addressesR;
	std::vector<int> agesR;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(lastNamesR), into(firstNamesR), into(addressesR), into(agesR), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::vector<Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (result == people);
}


void SQLExecutor::sharedPtrComplexTypeVector()
{
	std::string funct = "sharedPtrComplexTypeVector()";
	std::vector<Poco::SharedPtr<Person> > people;
	people.push_back(new Person("LN1", "FN1", "ADDR1", 1));
	people.push_back(new Person("LN2", "FN2", "ADDR2", 2));

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::vector<Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (2 == result.size());
	assertTrue (result[0] == *people[0]);
	assertTrue (result[1] == *people[1]);
}


void SQLExecutor::autoPtrComplexTypeVector()
{
	std::string funct = "sharedPtrComplexTypeVector()";
	std::vector<Poco::AutoPtr<RefCountedPerson> > people;
	people.push_back(new RefCountedPerson("LN1", "FN1", "ADDR1", 1));
	people.push_back(new RefCountedPerson("LN2", "FN2", "ADDR2", 2));

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::vector<Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (2 == result.size());
	assertTrue (result[0].address == people[0]->address);
	assertTrue (result[0].age == people[0]->age);
	assertTrue (result[0].firstName == people[0]->firstName);
	assertTrue (result[0].lastName == people[0]->lastName);
	assertTrue (result[1].address == people[1]->address);
	assertTrue (result[1].age == people[1]->age);
	assertTrue (result[1].firstName == people[1]->firstName);
	assertTrue (result[1].lastName == people[1]->lastName);
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
		Statement stmt((session() << "INSERT INTO " << ExecUtil::strings() << " VALUES (?)", use(str)));
		try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assertTrue (count == 0);

		try { stmt.execute(); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assertTrue (count == 4);
	}
	count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 4);
}


void SQLExecutor::insertEmptyVector()
{
	std::string funct = "insertEmptyVector()";
	std::vector<std::string> str;

	try
	{
		session() << "INSERT INTO " << ExecUtil::strings() << " VALUES (?)", use(str), now;
		fail("empty collections should not work");
	}
	catch (Poco::Exception&)
	{
	}
}


void SQLExecutor::bigStringVector()
{
	std::string funct = "bigStringVector()";
	std::vector<std::string> str;
	str.push_back(std::string(10000, 'a'));

	{
		Statement stmt((session() << "INSERT INTO " << ExecUtil::strings() << " VALUES (?)", use(str)));
		try { stmt.execute(); fail("must throw"); }
		catch (LengthExceededException&) { }
	}

	str.clear();
	str.push_back(std::string(30, 'a'));
	str.push_back(std::string(30, 'b'));
	str.push_back(std::string(30, 'c'));
	str.push_back(std::string(30, 'd'));
	int count = 100;
	{
		Statement stmt((session() << "INSERT INTO " << ExecUtil::strings() << " VALUES (?)", use(str)));
		try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
		catch (ConnectionException& ce) { std::cout << ce.toString() << std::endl; fail(funct); }
		catch (StatementException& se) { std::cout << se.toString() << std::endl; fail(funct); }
		assertTrue (count == 0);

		try { stmt.execute(); }
		catch (StatementException& se) { std::cout << se.toString() << std::endl; fail(funct); }

		try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
		catch (ConnectionException& ce) { std::cout << ce.toString() << std::endl; fail(funct); }
		catch (StatementException& se) { std::cout << se.toString() << std::endl; fail(funct); }
		assertTrue (count == 4);
	}
	count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
	catch (ConnectionException& ce) { std::cout << ce.toString() << std::endl; fail(funct); }
	catch (StatementException& se) { std::cout << se.toString() << std::endl; fail(funct); }
	assertTrue (count == 4);
}


void SQLExecutor::simpleAccessList()
{
	std::string funct = "simpleAccessList()";
	std::list<std::string> lastNames;
	std::list<std::string> firstNames;
	std::list<std::string> addresses;
	std::list<int> ages;
	const std::string tableName(ExecUtil::person());
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::list<std::string> lastNamesR;
	std::list<std::string> firstNamesR;
	std::list<std::string> addressesR;
	std::list<int> agesR;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(lastNamesR), into(firstNamesR), into(addressesR), into(agesR), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (ages == agesR);
	assertTrue (lastNames == lastNamesR);
	assertTrue (firstNames == firstNamesR);
	assertTrue (addresses == addressesR);
}


void SQLExecutor::complexTypeList()
{
	std::string funct = "complexTypeList()";
	std::list<Person> people;
	people.push_back(Person("LN1", "FN1", "ADDR1", 1));
	people.push_back(Person("LN2", "FN2", "ADDR2", 2));

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::list<Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (result == people);
}


void SQLExecutor::insertList()
{
	std::string funct = "insertList()";
	std::list<std::string> str;
	str.push_back("s1");
	str.push_back("s2");
	str.push_back("s3");
	str.push_back("s3");
	int count = 100;

	{
		Statement stmt((session() << "INSERT INTO " << ExecUtil::strings() << " VALUES (?)", use(str)));
		try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assertTrue (count == 0);

		try { stmt.execute(); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assertTrue (count == 4);
	}
	count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 4);
}


void SQLExecutor::insertEmptyList()
{
	std::string funct = "insertEmptyList()";
	std::list<std::string> str;

	try
	{
		session() << "INSERT INTO " << ExecUtil::strings() << " VALUES (?)", use(str), now;
		fail("empty collections should not work");
	}
	catch (Poco::Exception&)
	{
	}
}


void SQLExecutor::simpleAccessDeque()
{
	std::string funct = "simpleAccessDeque()";
	std::deque<std::string> lastNames;
	std::deque<std::string> firstNames;
	std::deque<std::string> addresses;
	std::deque<int> ages;
	const std::string tableName(ExecUtil::person());
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::deque<std::string> lastNamesR;
	std::deque<std::string> firstNamesR;
	std::deque<std::string> addressesR;
	std::deque<int> agesR;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(lastNamesR), into(firstNamesR), into(addressesR), into(agesR), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (ages == agesR);
	assertTrue (lastNames == lastNamesR);
	assertTrue (firstNames == firstNamesR);
	assertTrue (addresses == addressesR);
}


void SQLExecutor::complexTypeDeque()
{
	std::string funct = "complexTypeDeque()";
	std::deque<Person> people;
	people.push_back(Person("LN1", "FN1", "ADDR1", 1));
	people.push_back(Person("LN2", "FN2", "ADDR2", 2));

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::deque<Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (result == people);
}


void SQLExecutor::insertDeque()
{
	std::string funct = "insertDeque()";
	std::deque<std::string> str;
	str.push_back("s1");
	str.push_back("s2");
	str.push_back("s3");
	str.push_back("s3");
	int count = 100;

	{
		Statement stmt((session() << "INSERT INTO " << ExecUtil::strings() << " VALUES (?)", use(str)));
		try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assertTrue (count == 0);

		try { stmt.execute(); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assertTrue (count == 4);
	}
	count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 4);
}


void SQLExecutor::insertEmptyDeque()
{
	std::string funct = "insertEmptyDeque()";
	std::deque<std::string> str;

	try
	{
		session() << "INSERT INTO " << ExecUtil::strings() << " VALUES (?)", use(str), now;
		fail("empty collections should not work");
	}
	catch (Poco::Exception&)
	{
	}
}


void SQLExecutor::affectedRows(const std::string& whereClause)
{
	std::vector<std::string> str;
	str.push_back("s1");
	str.push_back("s2");
	str.push_back("s3");
	str.push_back("s3");
	int count = 100;

	Statement stmt1((session() << "INSERT INTO " << ExecUtil::strings() << " VALUES(?)", use(str)));
	session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now;
	assertTrue (count == 0);
	assertTrue (4 == stmt1.execute());
	session() << "SELECT COUNT(*) FROM " << ExecUtil::strings(), into(count), now;
	assertTrue (count == 4);

	Statement stmt2(session() << "UPDATE " << ExecUtil::strings() << " SET str = 's4' WHERE str = 's3'");
	assertTrue (2 == stmt2.execute());

	Statement stmt3(session() << "DELETE FROM " << ExecUtil::strings() << " WHERE str = 's1'");
	assertTrue (1 == stmt3.execute());

	std::string sql;
	format(sql, "DELETE FROM %s %s", ExecUtil::strings(), whereClause);
	Statement stmt4(session() << sql);
	assertTrue (3 == stmt4.execute());
}


void SQLExecutor::insertSingleBulk()
{
	std::string funct = "insertSingleBulk()";
	int x = 0;
	Statement stmt((session() << "INSERT INTO " << ExecUtil::ints() << " VALUES (?)", use(x)));

	for (x = 0; x < 100; ++x)
	{
		std::size_t i = stmt.execute();
		assertTrue (1 == i);
	}
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::ints(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 100);

	try { session() << "SELECT SUM(str) FROM " << ExecUtil::ints(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == ((0+99)*100/2));
}


void SQLExecutor::insertSingleBulkNullableVec()
{
	std::string funct = "insertSingleBulkNullableVec()";
	
	std::vector<Nullable<std::string>> dataString;
	std::vector<Nullable<UTF16String>> dataWString;
	std::vector<Nullable<Poco::Int32>> dataInt;
	std::vector<Nullable<float>> dataFloat;
	std::vector<Nullable<CLOB>> dataBlob;
	std::vector<Nullable<DateTime>> dataDateTime;
	std::vector<Nullable<Date>> dataDate;
	std::vector<Nullable<Time>> dataTime;
	
	DateTime date;
	int sum = 0;
	for (int x = 0; x < 100; ++x)
		if (x % 2 == 0)
		{
			dataString.push_back("String" + std::to_string(x));
			UTF16String utf = Poco::UnicodeConverter::to<UTF16String>("WString" + std::to_string(x));
			dataWString.push_back(utf);
			dataInt.push_back(x);
			dataFloat.push_back(x * 1.5f);
			dataBlob.push_back(CLOB(std::string("Blob" + std::to_string(x))));
			date += Poco::Timespan(2, 0, 2, 0, 0);
			dataDateTime.push_back(date);
			dataDate.push_back(Date(date));
			dataTime.push_back(Time(date));
			sum += x;
		}
		else
		{
			dataString.push_back(Nullable<std::string>());
			dataWString.push_back(Nullable<UTF16String>());
			dataInt.push_back(Nullable<int>());
			dataFloat.push_back(Nullable<float>());
			dataBlob.push_back(Nullable<CLOB>());
			dataDateTime.push_back(Nullable<DateTime>());
			dataDate.push_back(Nullable<Date>());
			dataTime.push_back(Nullable<Time>());
		}

	try
	{
		Statement stmt((session() << "INSERT INTO " << ExecUtil::nullabletest() << " VALUES (?,?,?,?,?,?,?,?)", use(dataWString, bulk), use(dataString, bulk), use(dataInt, bulk), use(dataFloat, bulk), use(dataBlob, bulk), use(dataDateTime, bulk), use(dataDate, bulk), use(dataTime, bulk)));
		stmt.execute();
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(1) FROM " << ExecUtil::nullabletest(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assert (count == 100);

	try { session() << "SELECT SUM(EmptyInteger) FROM " << ExecUtil::nullabletest(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assert (count == sum);

	try
	{
		Statement stat(session());
		stat << "SELECT convert(varbinary, EmptyString), convert(varbinary, EmptyUniString) FROM " << ExecUtil::nullabletest(), now;
		RecordSet rs(stat);
		size_t ndx = 0;
		for (bool cont = rs.moveFirst(); cont; cont = rs.moveNext())
		{
			Nullable<std::string> str = dataString[ndx];
			Poco::SQL::BLOB blob = rs.value<Poco::SQL::BLOB>(0);
			if (str.isNull())
				assert(blob.size() == 0);
			else
				assert(blob.size() == str.value().length());

			Nullable<UTF16String> wstr = dataWString[ndx];
			blob = rs.value<Poco::SQL::BLOB>(1);
			if (wstr.isNull())
				assert(blob.size() == 0);
			else
				assert(blob.size() == wstr.value().length() * 2);

			ndx++;
		}
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
}


void SQLExecutor::floats()
{
	std::string funct = "floats()";
	float data = 1.5f;
	float ret = 0.0f;

	try { session() << "INSERT INTO " << ExecUtil::floats() << " VALUES (?)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::floats(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 1);

	try { session() << "SELECT str FROM " << ExecUtil::floats(), into(ret), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (ret == data);
}


void SQLExecutor::doubles()
{
	std::string funct = "doubles()";
	double data = 1.5;
	double ret = 0.0;

	try { session() << "INSERT INTO " << ExecUtil::doubles() << " VALUES (?)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::doubles(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 1);

	try { session() << "SELECT str FROM " << ExecUtil::doubles(), into(ret), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (ret == data);
}


void SQLExecutor::insertSingleBulkVec()
{
	std::string funct = "insertSingleBulkVec()";
	std::vector<int> data;
	
	for (int x = 0; x < 100; ++x)
		data.push_back(x);

	Statement stmt((session() << "INSERT INTO " << ExecUtil::ints() << " VALUES (?)", use(data)));
	stmt.execute();

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::ints(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	assertTrue (count == 100);
	try { session() << "SELECT SUM(str) FROM " << ExecUtil::ints(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::ints() << " VALUES (?)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	std::vector<int> retData;
	try { session() << "SELECT * FROM " << ExecUtil::ints(), into(retData), limit(50), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::ints() << " VALUES (?)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	std::vector<int> retData;
	try { session() << "SELECT * FROM " << ExecUtil::ints(), into(retData), limit(0), now; }// stupid test, but at least we shouldn't crash
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::ints() << " VALUES (?)", use(data), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	std::vector<int> retData;
	Statement stmt = (session() << "SELECT * FROM " << ExecUtil::ints(), into(retData), limit(50), now);
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

	try
	{
		Statement stmt = (session() << "INSERT INTO " << ExecUtil::ints() << " VALUES (?)", use(data));
		assertTrue (100 == stmt.execute());
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	std::vector<int> retData;
	Statement stmt = (session() << "SELECT * FROM " << ExecUtil::ints(), into(retData), limit(50));
	assertTrue (retData.size() == 0);
	assertTrue (!stmt.done());

	try { stmt.execute(); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (!stmt.done());
	assertTrue (retData.size() == 50);

	try { stmt.execute(); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (stmt.done());
	assertTrue (retData.size() == 100);

	try { stmt.execute(); }// will restart execution!
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

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
		Statement stmt((session() << "INSERT INTO " << ExecUtil::ints() << " VALUES (?)", use(data)));
	}

	// stmt should not have been executed when destroyed
	int count = 100;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::ints(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 0);
}


void SQLExecutor::doBulkPerformance(Poco::UInt32 size)
{
	std::string funct = "doBulkPerformance()";
	std::vector<int> ints(size, 1);
	std::vector<std::string> strings(size, "abc");
	std::vector<double> floats(size, .5);
	std::vector<DateTime> dateTimes(size);
	
	Stopwatch sw;
	try
	{
		sw.start();
		session() << "INSERT INTO "<< ExecUtil::misctest() <<" (First, Third, Fourth, Fifth) VALUES (?,?,?,?)",
			use(strings),
			use(ints),
			use(floats),
			use(dateTimes), now;
		sw.stop();
	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	double time = sw.elapsed() / 1000.0;

	try { session() << "DELETE FROM "<< ExecUtil::misctest(), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	try
	{
		sw.restart();
		session() << "INSERT INTO "<< ExecUtil::misctest() <<" (First, Third, Fourth, Fifth) VALUES (?,?,?,?)",
			use(strings, bulk),
			use(ints, bulk),
			use(floats, bulk),
			use(dateTimes, bulk), now;
		sw.stop();
	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	
	double bulkTime = sw.elapsed() / 1000.0;

	double speedup;
	if (0.0 == bulkTime)
	{
		if (0.0 == time) speedup = 1.0;
		else speedup = time;
	}
	else
		speedup = time / bulkTime;

	std::cout << "INSERT => Size:" << size
		<< ", Time: " << time
		<< ", Bulk Time: " << bulkTime
		<< " [ms], Speedup: " << speedup
		<< 'x' << std::endl;

	ints.clear();
	strings.clear();
	floats.clear();
	dateTimes.clear();

	try
	{
		sw.restart();
		session() << "SELECT First, Third, Fourth, Fifth FROM "<< ExecUtil::misctest(),
			into(strings),
			into(ints),
			into(floats),
			into(dateTimes),
			now;
		sw.stop();
	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	
	time = sw.elapsed() / 1000.0;

	assertTrue (ints.size() == size);

	ints.clear();
	strings.clear();
	floats.clear();
	dateTimes.clear();
	
	try
	{
		sw.restart();
		session() << "SELECT First, Third, Fourth, Fifth FROM "<< ExecUtil::misctest(),
			into(strings, bulk(size)),
			into(ints, bulk(size)),
			into(floats, bulk(size)),
			into(dateTimes, bulk(size)),
			now;
		sw.stop();
	} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	
	bulkTime = sw.elapsed() / 1000.0;

	assertTrue (ints.size() == size);

	if (0.0 == bulkTime)
	{
		if (0.0 == time) speedup = 1.0;
		else speedup = time;
	}
	else
		speedup = time / bulkTime;

	std::cout << "SELECT => Size:" << size
		<< ", Time: " << time
		<< ", Bulk Time: " << bulkTime
		<< " [ms], Speedup: " << speedup
		<< 'x' << std::endl;
}


void SQLExecutor::setSimple()
{
	std::string funct = "setSimple()";
	std::set<std::string> lastNames;
	std::set<std::string> firstNames;
	std::set<std::string> addresses;
	std::set<int> ages;
	std::string tableName(ExecUtil::person());
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::set<std::string> lastNamesR;
	std::set<std::string> firstNamesR;
	std::set<std::string> addressesR;
	std::set<int> agesR;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(lastNamesR), into(firstNamesR), into(addressesR), into(agesR), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::set<Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 5);

	std::set<Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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
	std::string tableName(ExecUtil::person());
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::multiset<std::string> lastNamesR;
	std::multiset<std::string> firstNamesR;
	std::multiset<std::string> addressesR;
	std::multiset<int> agesR;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(lastNamesR), into(firstNamesR), into(addressesR), into(agesR), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 5);

	std::multiset<Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);

	std::map<std::string, Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 5);

	std::map<std::string, Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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
	
	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 5);

	std::multimap<std::string, Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (result.size() == people.size());
}


void SQLExecutor::selectIntoSingle()
{
	std::string funct = "selectIntoSingle()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN2", p2));

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try { session() << "SELECT * FROM " << ExecUtil::person() << " ORDER BY LastName", into(result), limit(1), now; }// will return 1 object into one single result
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (result == p1);
}


void SQLExecutor::selectIntoSingleStep()
{
	std::string funct = "selectIntoSingleStep()";
	std::multimap<std::string, Person> people;
	Person p1("LN1", "FN1", "ADDR1", 1);
	Person p2("LN2", "FN2", "ADDR2", 2);
	people.insert(std::make_pair("LN1", p1));
	people.insert(std::make_pair("LN2", p2));

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	Statement stmt = (session() << "SELECT * FROM " << ExecUtil::person(), into(result), limit(1));
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
	people.insert(std::make_pair("LN2", p2));

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), limit(2, true), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try
	{
		session() << "SELECT * FROM " << ExecUtil::person(), into(result), limit(1, true), now; // will fail now
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try
	{
		session() << "SELECT * FROM " << ExecUtil::person(), into(result), lowerLimit(2), now; // will return 2 objects into one single result but only room for one!
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	Statement stmt = (session() << "SELECT * FROM " << ExecUtil::person(), into(result), limit(1));
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try
	{
		session() << "SELECT * FROM " << ExecUtil::person(), into(result), lowerLimit(3), now; // will fail
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);
	std::vector <Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), lowerLimit(2), upperLimit(2), now; }// will return 2 objects
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);
	std::vector <Person> result;
	try { session() << "SELECT * FROM " << ExecUtil::person(), into(result), range(2, 2), now; }// will return 2 objects
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try
	{
		session() << "SELECT * FROM " << ExecUtil::person(), into(result), lowerLimit(3), upperLimit(2), now;
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(people), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 2);
	Person result;
	try
	{
		session() << "SELECT * FROM " << ExecUtil::person(), into(result), range(3, 2), now;
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
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 0);

	Person result;
	Statement stmt = (session() << "SELECT * FROM " << ExecUtil::person(), into(result), limit(1));
	stmt.execute();
	assertTrue (result.firstName.empty());
	assertTrue (stmt.done());
}


struct LobTester
{

	template <typename TargetBlb, typename SrcBlb>
	static TargetBlb convertBlob(const SrcBlb& s)
	{
		TargetBlb t;
		for (typename SrcBlb::Iterator i = s.begin(); i != s.end(); ++i)
		{
			typename TargetBlb::ValueType v = static_cast<typename TargetBlb::ValueType>(*i);
			t.appendRaw(&v, 1);
		}
		return t;
	}

	template <typename ContType, typename BlobType>
	static void doTest(SQLExecutor* tc, Session& sess, const std::string& blobPlaceholder, int bigSize, const std::string& funct)
	{
		const std::string lastName("lastname");
		const std::string firstName("firstname");
		const std::string address("Address");
		const std::string tblName(ExecUtil::person());

		sess << "DELETE FROM " << ExecUtil::person(), now;
		ContType blobs;
		for (size_t n = 0; n < 2; ++n)
		{
			BlobType img;
			const size_t sz = 10;
			for (size_t c = 0; c < sz; ++c)
			{
				typename BlobType::ValueType v = static_cast<typename BlobType::ValueType>('0' + c + sz * n);
				img.appendRaw(&v, 1);
			}
			blobs.push_back(img);
		}

		try {
			char rn = '1';
			for (typename ContType::const_iterator it = blobs.begin(); it != blobs.end(); ++it, ++rn)
			{
				sess << format("INSERT INTO %s VALUES (?,?,?,%s)", tblName, blobPlaceholder),
					bind(lastName + rn), useRef(firstName), useRef(address), useRef(*it), now;
			}
		}
		catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; failTU(tc, funct); }
		catch (StatementException& se){ std::cout << se.toString() << std::endl; failTU(tc, funct); }

		int count = 0;
		try { sess << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
		catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; failTU(tc, funct); }
		catch (StatementException& se){ std::cout << se.toString() << std::endl; failTU(tc, funct); }
		assertTU (tc, count == blobs.size());

		ContType resV;
		assertTU (tc, resV.size() == 0);
		try { sess << "SELECT Image FROM " << ExecUtil::person() << " ORDER BY LastName", into(resV), now; }
		catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; failTU(tc, funct); }
		catch (StatementException& se){ std::cout << se.toString() << std::endl; failTU(tc, funct); }
		bool r = resV == blobs;
		assertTU (tc, r);

		try {
			ContType resV2;
			Statement stat(sess);
			stat << "SELECT Image FROM " << ExecUtil::person() << " ORDER BY LastName", now;
			RecordSet rs(stat);
			Poco::SQL::ODBC::ODBCMetaColumn::ColumnDataType tp = rs.columnType(0);
			for (bool cont = rs.moveFirst(); cont; cont = rs.moveNext())
			{
				switch (tp)
				{
				case Poco::SQL::ODBC::ODBCMetaColumn::FDT_BLOB:
					resV2.push_back(convertBlob<BlobType>(rs.value<Poco::SQL::BLOB>(0)));
					break;
				case Poco::SQL::ODBC::ODBCMetaColumn::FDT_CLOB:
					resV2.push_back(convertBlob<BlobType>(rs.value<Poco::SQL::CLOB>(0)));
					break;
				default:
					failTU(tc, funct);
				}
				
			}
		  bool r = resV2 == blobs;
			assertTU(tc, r);
		}
		catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; failTU(tc, funct); }
		catch (StatementException& se){ std::cout << se.toString() << std::endl; failTU(tc, funct); }

		BlobType big;
		typename BlobType::Container v(bigSize);
		for (size_t p = 0; p < bigSize; ++p)
			v[p] = static_cast<typename BlobType::ValueType>(p);
		big.assignRaw(&v[0], v.size());

		assertTU (tc, big.size() == bigSize);

		try { sess << "DELETE FROM " << ExecUtil::person(), now; }
		catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; failTU(tc, funct); }
		catch (StatementException& se){ std::cout << se.toString() << std::endl; failTU(tc, funct); }

		try {
			sess << format("INSERT INTO %s VALUES (?,?,?,%s)", tblName, blobPlaceholder),
				useRef(lastName), useRef(firstName), bind(address), use(big), now;
		}
		catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; failTU(tc, funct); }
		catch (StatementException& se){ std::cout << se.toString() << std::endl; failTU(tc, funct); }

		BlobType res;
		try { sess << "SELECT Image FROM " << ExecUtil::person(), into(res), now; }
		catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; failTU(tc, funct); }
		catch (StatementException& se){ std::cout << se.toString() << std::endl; failTU(tc, funct); }
		assertTU (tc, res == big);

	}
};


void SQLExecutor::blob(int bigSize, const std::string& blobPlaceholder)
{
	const std::string funct = "blob()";

	LobTester::doTest< std::vector<CLOB>, CLOB>(this, session(), blobPlaceholder, bigSize, funct + "-1");
	LobTester::doTest< std::vector<Poco::SQL::BLOB>, Poco::SQL::BLOB>(this, session(), blobPlaceholder, bigSize, funct + "-2");

	LobTester::doTest< std::list<CLOB>, CLOB>(this, session(), blobPlaceholder, bigSize, funct + "-3");
	LobTester::doTest< std::list<Poco::SQL::BLOB>, Poco::SQL::BLOB>(this, session(), blobPlaceholder, bigSize, funct + "-4");

	LobTester::doTest< std::deque<CLOB>, CLOB>(this, session(), blobPlaceholder, bigSize, funct + "-5");
	LobTester::doTest< std::deque<Poco::SQL::BLOB>, Poco::SQL::BLOB>(this, session(), blobPlaceholder, bigSize, funct + "-6");
}


void SQLExecutor::blobStmt()
{
	std::string funct = "blobStmt()";
	std::string lastName("lastname");
	std::string firstName("firstname");
	std::string address("Address");
	CLOB blob("0123456789", 10);

	int count = 0;
	Statement ins = (session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastName), use(firstName), use(address), use(blob));
	ins.execute();
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 1);

	CLOB res;
	poco_assert (res.size() == 0);
	Statement stmt = (session() << "SELECT Image FROM " << ExecUtil::person(), into(res));
	try { stmt.execute(); }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	poco_assert (res == blob);
}


void SQLExecutor::dateTime()
{
	std::string funct = "dateTime()";
	std::string lastName("lastname");
	std::string firstName("firstname");
	std::string address("Address");

	DateTime born(1965, 6, 18, 5, 35, 1);
	int count = 0;
	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastName), use(firstName), use(address), use(born), now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail(funct); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail(funct); }
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail(funct); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail(funct); }
	assertTrue (count == 1);

	DateTime res;
	try { session() << "SELECT Born FROM " << ExecUtil::person(), into(res), now; }
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail(funct); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail(funct); }
	assertTrue (res == born);

	Statement stmt = (session() << "SELECT Born FROM " << ExecUtil::person(), now);
	RecordSet rset(stmt);

	res = rset["Born"].convert<DateTime>();
	assertTrue (res == born);
}


void SQLExecutor::date()
{
	std::string funct = "date()";
	std::string lastName("lastname");
	std::string firstName("firstname");
	std::string address("Address");

	Date bornDate(1965, 6, 18);
	int count = 0;
	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)",
		use(lastName),
		use(firstName),
		use(address),
		use(bornDate),
		now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 1);

	Date d;
	Time t;
	try { session() << "SELECT BornDate FROM " << ExecUtil::person(), into(d), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (d == bornDate);

	Statement stmt = (session() << "SELECT BornDate FROM " << ExecUtil::person(), now);
	RecordSet rset(stmt);

	DateTime dt1 = rset["BornDate"].convert<DateTime>();

	Date d2(dt1);
	assertTrue (d2 == bornDate);
}


void SQLExecutor::time()
{
	std::string funct = "time()";
	std::string lastName("lastname");
	std::string firstName("firstname");
	std::string address("Address");

	Time bornTime (5, 35, 1);
	int count = 0;
	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)",
		use(lastName),
		use(firstName),
		use(address),
		use(bornTime),
		now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 1);

	Date d;
	Time t;
	try { session() << "SELECT BornTime FROM " << ExecUtil::person(), into(t), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (t == bornTime);

	Statement stmt = (session() << "SELECT BornTime FROM " << ExecUtil::person(), now);
	RecordSet rset(stmt);

	DateTime dt2 = rset["BornTime"].convert<DateTime>();
	Time t2(dt2);
	assertTrue (t2 == bornTime);
}


void SQLExecutor::tuples()
{
	typedef Tuple<int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int> TupleType;
	std::string funct = "tuples()";
	TupleType t(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19);

	try { session() << "INSERT INTO " << ExecUtil::tuples() << " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)", use(t), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	TupleType ret(-10,-11,-12,-13,-14,-15,-16,-17,-18,-19);
	assertTrue (ret != t);
	try { session() << "SELECT * FROM " << ExecUtil::tuples(), into(ret), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
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

	try { session() << "INSERT INTO " << ExecUtil::tuples() << " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)", use(v), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::tuples(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (v.size() == count);

	std::vector<Tuple<int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int> > ret;
	try { session() << "SELECT * FROM " << ExecUtil::tuples(), into(ret), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (ret == v);
}


void SQLExecutor::internalExtraction()
{
	typedef int IntType;
	using Poco::SQL::RecordSet;
	using Poco::SQL::Column;
	using Poco::SQL::Statement;
	using Poco::UTF16String;
	using Poco::Tuple;
	using Poco::BadCastException;
	using Poco::RangeException;
	using Poco::SQL::ODBC::ConnectionException;
	using Poco::SQL::ODBC::StatementException;
	using namespace Poco::SQL::Keywords;

	std::string funct = "internalExtraction()";
	std::vector<Poco::Tuple<int, double, std::string> > v;
	v.push_back(Poco::Tuple<int, double, std::string>(1, 1.5, "3"));
	v.push_back(Poco::Tuple<int, double, std::string>(2, 2.5, "4"));
	v.push_back(Poco::Tuple<int, double, std::string>(3, 3.5, "5"));
	v.push_back(Poco::Tuple<int, double, std::string>(4, 4.5, "6"));

	try { session() << "INSERT INTO " << ExecUtil::vectors() << " VALUES (?,?,?)", use(v), now; }
	catch (ConnectionException& ce) { std::cout << ce.toString() << std::endl; fail(funct); }
	catch (StatementException& se) { std::cout << se.toString() << std::endl; fail(funct); }

	try
	{
		Statement stmt = (session() << "SELECT * FROM " << ExecUtil::vectors(), now);
		RecordSet rset(stmt);

		assertTrue (3 == rset.columnCount());
		assertTrue (4 == rset.rowCount());

		int curVal = 3;
		do
		{
			assertTrue (rset["str0"] == curVal);
			++curVal;
		} while (rset.moveNext());

		rset.moveFirst();
		assertTrue (rset["str0"] == "3");
		rset.moveLast();
		assertTrue (rset["str0"] == "6");

		RecordSet rset2(rset);
		assertTrue (3 == rset2.columnCount());
		assertTrue (4 == rset2.rowCount());

		IntType i;
		try {
			i = rset.value<IntType>(0, 0);
			assertTrue (1 == i);
		}
		catch (Poco::BadCastException& ex)
		{
			std::cout << ex.displayText() << std::endl;
		}
		std::string s = rset.value(0, 0).convert<std::string>();
		assertTrue ("1" == s);

		IntType a = rset.value<IntType>(0, 2);
		assertTrue (3 == a);

		try
		{
			double d = rset.value<double>(1, 1);
			assertTrue (2.5 == d);
		}
		catch (BadCastException&)
		{
			float f = rset.value<float>(1, 1);
			assertTrue (2.5 == f);
		}

		try
		{
			s = rset.value<std::string>(2, 2);
		}
		catch (BadCastException&)
		{
			UTF16String us = rset.value<Poco::UTF16String>(2, 2);
			Poco::UnicodeConverter::convert(us, s);
		}
		assertTrue ("5" == s);

		i = rset.value("str0", 2);
		assertTrue (5 == i);

		const Column<std::deque<IntType> >& col = rset.column<std::deque<IntType> >(0);
		typename Column<std::deque<IntType> >::Iterator it = col.begin();
		typename Column<std::deque<IntType> >::Iterator end = col.end();
		for (int i = 1; it != end; ++it, ++i)
			assertTrue (*it == i);

		rset = (session() << "SELECT COUNT(*) AS cnt FROM " << ExecUtil::vectors(), now);

		//various results for COUNT(*) are received from different drivers
		try
		{
			//this is what most drivers will return
			int i = rset.value<int>(0, 0);
			assertTrue (4 == i);
		}
		catch (BadCastException&)
		{
			try
			{
				//this is for Oracle
				double i = rset.value<double>(0, 0);
				assertTrue (4 == int(i));
			}
			catch (BadCastException&)
			{
				//this is for PostgreSQL
				Poco::Int64 big = rset.value<Poco::Int64>(0, 0);
				assertTrue (4 == big);
			}
		}

		s = rset.value("cnt", 0).convert<std::string>();
		assertTrue ("4" == s);

		try { rset.column<std::deque<IntType> >(100); fail("must fail"); }
		catch (RangeException&) {}

		try { rset.value<std::string>(0, 0); fail("must fail"); }
		catch (BadCastException&) {}

		stmt = (session() << "DELETE FROM " << ExecUtil::vectors(), now);
		rset = stmt;

		try { rset.column<std::deque<IntType> >(0); fail("must fail"); }
		catch (RangeException&) {}
	}
	catch (ConnectionException& ce) { std::cout << ce.toString() << std::endl; fail(funct); }
	catch (StatementException& se) { std::cout << se.toString() << std::endl; fail(funct); }
}


void SQLExecutor::filter(const std::string& query, const std::string& intFldName)
{
	std::string funct = "filter()";
	std::vector<Tuple<int, double, std::string> > v;
	v.push_back(Tuple<int, double, std::string>(1, 1.5f, "3"));
	v.push_back(Tuple<int, double, std::string>(2, 2.5f, "4"));
	v.push_back(Tuple<int, double, std::string>(3, 3.5f, "5"));
	v.push_back(Tuple<int, double, std::string>(4, 4.5f, "6"));

	try { session() << "INSERT INTO " << ExecUtil::vectors() << " VALUES (?,?,?)", use(v), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	try
	{
		Statement stmt = (session() << query, now);
		RecordSet rset(stmt);
		assertTrue (rset.totalRowCount() == 4);
		RowFilter::Ptr pRF = new RowFilter(rset);
		assertTrue (pRF->isEmpty());
		pRF->add(intFldName, RowFilter::VALUE_EQUAL, 1);
		assertTrue (!pRF->isEmpty());

		Var da;
		try
		{
			da = rset.value(0, 1);
			fail ("must fail");
		} catch (InvalidAccessException&)
		{
			da = rset.value(0, 1, false);
			assertTrue (2 == da);
			da = rset.value(0, 0);
			assertTrue (1 == da);
		}

		assertTrue (rset.rowCount() == 1);
		assertTrue (rset.moveFirst());
		assertTrue (1 == rset[intFldName]);
		assertTrue (!rset.moveNext());
		pRF->add("flt0", RowFilter::VALUE_LESS_THAN_OR_EQUAL, 3.5f);
		assertTrue (rset.rowCount() == 3);
		assertTrue (rset.moveNext());
		assertTrue (2.5 == rset["flt0"]);
		assertTrue (rset.moveNext());
		assertTrue (3.5 == rset["flt0"]);
		assertTrue (!rset.moveNext());
		pRF->add("str0", RowFilter::VALUE_EQUAL, 6);
		assertTrue (rset.rowCount() == 4);
		assertTrue (rset.moveLast());
		assertTrue ("6" == rset["str0"]);
		pRF->remove("flt0");
		assertTrue (rset.rowCount() == 2);
		assertTrue (rset.moveFirst());
		assertTrue ("3" == rset["str0"]);
		assertTrue (rset.moveNext());
		assertTrue ("6" == rset["str0"]);
		pRF->remove(intFldName);
		pRF->remove("str0");
		assertTrue (pRF->isEmpty());
		pRF->add("str0", "!=", 3);
		assertTrue (rset.rowCount() == 3);

		RowFilter::Ptr pRF1 = new RowFilter(pRF, RowFilter::OP_AND);
		pRF1->add(intFldName, "==", 2);
		assertTrue (rset.rowCount() == 1);
		pRF1->add(intFldName, "<", 2);
		assertTrue (rset.rowCount() == 1);
		pRF1->add(intFldName, ">", 3);
		assertTrue (rset.rowCount() == 2);
		pRF->removeFilter(pRF1);
		pRF->remove("str0");
		assertTrue (pRF->isEmpty());
		assertTrue (rset.rowCount() == 4);
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
}


void SQLExecutor::internalBulkExtraction()
{
	std::string funct = "internalBulkExtraction()";
	int size = 100;
	std::vector<std::string> lastName(size);
	std::vector<std::string> firstName(size);
	std::vector<std::string> address(size);
	std::vector<int> age(size);

	for (int i = 0; i < size; ++i)
	{
		lastName[i] = "LN" + NumberFormatter::format(i);
		firstName[i] = "FN" + NumberFormatter::format(i);
		address[i] = "Addr" + NumberFormatter::format(i);
		age[i] = i;
	}

	try
	{
		session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)",
			use(lastName, bulk),
			use(firstName, bulk),
			use(address, bulk),
			use(age, bulk),
			now;
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	try
	{
		Statement stmt = (session() << "SELECT * FROM " << ExecUtil::person(), bulk(size), now);
		RecordSet rset(stmt);
		assertTrue (size == rset.rowCount());
		assertTrue ("LN0" == rset["LastName"]);
		assertTrue (0 == rset["Age"]);
		rset.moveNext();
		assertTrue ("LN1" == rset["LastName"]);
		assertTrue (1 == rset["Age"]);
		rset.moveLast();
		assertTrue (std::string("LN") + NumberFormatter::format(size - 1) == rset["LastName"]);
		assertTrue (size - 1 == rset["Age"]);
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	try
	{
		Statement stmt = (session() << "SELECT * FROM " << ExecUtil::person(), limit(size), bulk, now);
		RecordSet rset(stmt);
		assertTrue (size == rset.rowCount());
		assertTrue ("LN0" == rset["LastName"]);
		assertTrue (0 == rset["Age"]);
		rset.moveLast();
		assertTrue (std::string("LN") + NumberFormatter::format(size - 1) == rset["LastName"]);
		assertTrue (size - 1 == rset["Age"]);
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
}


void SQLExecutor::internalBulkExtractionUTF16()
{
	std::string funct = "internalBulkExtractionUTF16()";
	int size = 100;
	std::vector<UTF16String> lastName(size);
	std::vector<UTF16String> firstName(size);
	std::vector<UTF16String> address(size);
	std::vector<int> age(size);

	for (int i = 0; i < size; ++i)
	{
		lastName[i] = Poco::UnicodeConverter::to<UTF16String>("LN" + NumberFormatter::format(i));
		firstName[i] = Poco::UnicodeConverter::to<UTF16String>("FN" + NumberFormatter::format(i));
		address[i] = Poco::UnicodeConverter::to<UTF16String>("Addr" + NumberFormatter::format(i));
		age[i] = i;
	}

	try
	{
		session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)",
			use(lastName, bulk),
			use(firstName, bulk),
			use(address, bulk),
			use(age, bulk),
			now;
	}
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail(funct); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail(funct); }

	try
	{
		Statement stmt = (session() << "SELECT * FROM " << ExecUtil::person(), bulk(size), now);
		RecordSet rset(stmt);
		assertTrue (size == rset.rowCount());
		assertTrue (Poco::UnicodeConverter::to<UTF16String>("LN0") == rset["LastName"]);
		assertTrue (0 == rset["Age"]);
		rset.moveNext();
		assertTrue (Poco::UnicodeConverter::to<UTF16String>("LN1") == rset["LastName"]);
		assertTrue (1 == rset["Age"]);
		rset.moveLast();
		assertTrue (std::string("LN") + NumberFormatter::format(size - 1) == rset["LastName"]);
		assertTrue (size - 1 == rset["Age"]);
	}
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail(funct); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail(funct); }

	try
	{
		Statement stmt = (session() << "SELECT * FROM " << ExecUtil::person(), limit(size), bulk, now);
		RecordSet rset(stmt);
		assertTrue (size == rset.rowCount());
		assertTrue ("LN0" == rset["LastName"]);
		assertTrue (0 == rset["Age"]);
		rset.moveLast();
		assertTrue (std::string("LN") + NumberFormatter::format(size - 1) == rset["LastName"]);
		assertTrue (size - 1 == rset["Age"]);
	}
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail(funct); }
	catch (StatementException& se){ std::cout << se.toString() << std::endl; fail(funct); }
}


void SQLExecutor::internalStorageType()
{
	std::string funct = "internalStorageType()";
	std::vector<Statement::Manipulator> manips;
	manips.push_back(list);
	manips.push_back(deque);
	manips.push_back(vector);

	std::vector<Tuple<int, double, std::string> > v;
	v.push_back(Tuple<int, double, std::string>(1, 1.5f, "3"));
	v.push_back(Tuple<int, double, std::string>(2, 2.5f, "4"));
	v.push_back(Tuple<int, double, std::string>(3, 3.5f, "5"));
	v.push_back(Tuple<int, double, std::string>(4, 4.5f, "6"));

	try { session() << "INSERT INTO " << ExecUtil::vectors() << " VALUES (?,?,?)", use(v), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	try
	{
		std::vector<Statement::Manipulator>::iterator it = manips.begin();
		std::vector<Statement::Manipulator>::iterator end = manips.end();

		for (; it != end; ++it)
		{
			Statement stmt = (session() << "SELECT * FROM " << ExecUtil::vectors(), *it, now);
			RecordSet rset(stmt);

			assertTrue (3 == rset.columnCount());
			assertTrue (4 == rset.rowCount());

			int curVal = 3;
			do
			{
				assertTrue (rset["str0"] == curVal);
				++curVal;
			} while (rset.moveNext());

			rset.moveFirst();
			assertTrue (rset["str0"] == "3");
			rset.moveLast();
			assertTrue (rset["str0"] == "6");

			try
			{
				stmt = (session() << "SELECT * FROM " << ExecUtil::vectors(), now, *it);
				fail ("must fail");
			}
			catch(InvalidAccessException&){}

			try
			{
				stmt = (session() << "SELECT * FROM " << ExecUtil::vectors(), into(v), now, *it);
				fail ("must fail");
			}
			catch(InvalidAccessException&){}

			try
			{
				stmt = (session() << "SELECT * FROM " << ExecUtil::vectors(), into(v), *it, now);
				fail ("must fail");
			}
			catch(InvalidAccessException&){}
		}
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
}


void SQLExecutor::notNulls(const std::string& sqlState)
{
	try
	{
		session() << "INSERT INTO "<< ExecUtil::nulltest() << " (i,r,v) VALUES (?,?,?)", use(null), use(null), use(null), now;
		fail ("must fail");
	}
	catch (StatementException& se)
	{
		//double check if we're failing for the right reason
		//default sqlState value is "23502"; some drivers report "HY???" codes
		if (se.diagnostics().fields().size())
		{
			std::string st = se.diagnostics().sqlState(0);
			if (sqlState != st)
				std::cerr << '[' << name() << ']' << " Warning: expected SQL state [" << sqlState <<
					"], received [" << se.diagnostics().sqlState(0) << "] instead." << std::endl;
		}
	}
}


void SQLExecutor::nulls(bool emptyStrIsSpace)
{
	std::string funct = "nulls()";

	try { session() << "INSERT INTO " << ExecUtil::nulltest() << " (i,r,v) VALUES (?,?,?)", use(Poco::SQL::DATA_NULL_INTEGER), use(Poco::SQL::DATA_NULL_FLOAT), use(Poco::SQL::DATA_NULL_STRING), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	RecordSet rs(session(), "SELECT * FROM " + ExecUtil::nulltest());
	assertTrue (1 == rs.rowCount());
	rs.moveFirst();
	assertTrue (rs.isNull("i"));
	assertTrue (rs["i"] != 0);
	assertTrue (rs.isNull("r"));
	assertTrue (rs.isNull("v"));
	assertTrue (rs["v"] != "");
	assertTrue (rs.nvl<int>("i") == 0);
	assertTrue (rs.nvl("i", -1) == -1);
	assertTrue (rs.nvl<double>("r") == double());
	assertTrue (rs.nvl("r", -1.5) == -1.5);
	assertTrue (rs.nvl<std::string>("v") == "");
	assertTrue (rs.nvl("v", "123") == "123");
	try { session() << "DELETE FROM " << ExecUtil::nulltest(), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	int i = 1;
	double f = 1.5;
	std::string s = "123";

	try { session() << "INSERT INTO " << ExecUtil::nulltest() << " (i, r, v) VALUES (?,?,?)", use(i), use(f), use(s), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	rs = (session() << "SELECT * FROM " << ExecUtil::nulltest(), now);
	assertTrue (1 == rs.rowCount());
	rs.moveFirst();
	assertTrue (!rs.isNull("i"));
	assertTrue (rs["i"] == 1);
	assertTrue (!rs.isNull("v"));
	assertTrue (!rs.isNull("r"));
	assertTrue (rs["v"] == "123");
	assertTrue (rs.nvl<int>("i") == 1);
	assertTrue (rs.nvl("i", -1) == 1);
	assertTrue (rs.nvl<double>("r") == 1.5);
	assertTrue (rs.nvl("r", -1.5) == 1.5);
	assertTrue (rs.nvl<std::string>("v") == "123");
	assertTrue (rs.nvl("v", "456") == "123");
	try { session() << "UPDATE " << ExecUtil::nulltest() << " SET v = ? WHERE i = ?", use(Poco::SQL::DATA_NULL_STRING), use(i), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	i = 2;
	f = 3.4;
	try { session() << "INSERT INTO " << ExecUtil::nulltest() << " (i, r, v) VALUES (?,?,?)", use(i), use(Poco::SQL::DATA_NULL_FLOAT), use(Poco::SQL::DATA_NULL_STRING), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	rs = (session() << "SELECT i, r, v FROM " << ExecUtil::nulltest() << " ORDER BY i ASC", now);
	assertTrue (2 == rs.rowCount());
	rs.moveFirst();
	assertTrue (!rs.isNull("i"));
	assertTrue (rs["i"] == 1);
	assertTrue (!rs.isNull("r"));
	assertTrue (rs.isNull("v"));
	assertTrue (rs["v"] != "");

	assertTrue (rs.moveNext());
	assertTrue (!rs.isNull("i"));
	assertTrue (rs["i"] == 2);
	assertTrue (rs.isNull("r"));
	assertTrue (rs.isNull("v"));
	assertTrue (rs["v"] != "");

	try { session() << "DELETE FROM " << ExecUtil::nulltest(), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	try { session() << "INSERT INTO " << ExecUtil::nulltest() << " (v) VALUES (?)", bind(""), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	bool esin = session().getFeature("emptyStringIsNull");
	session().setFeature("emptyStringIsNull", true);

	try
	{
		session().setFeature("forceEmptyString", true);
		fail ("must fail");
	} catch (InvalidAccessException&) { }

	bool fes = session().getFeature("forceEmptyString");
	session().setFeature("forceEmptyString", false);

	RecordSet rs1(session(), "SELECT v FROM " + ExecUtil::nulltest());
	assertTrue (1 == rs1.rowCount());
	rs1.moveFirst();
	if (!emptyStrIsSpace)
	{
		assertTrue (rs1.isNull("v"));
		assertTrue (!(rs["v"] == ""));
	}

	session().setFeature("emptyStringIsNull", false);
	session().setFeature("forceEmptyString", true);
	RecordSet rs2(session(), "SELECT v FROM " + ExecUtil::nulltest());
	assertTrue (1 == rs2.rowCount());
	rs2.moveFirst();
	assertTrue (!rs2.isNull("v"));
	if (!emptyStrIsSpace)
	{
		assertTrue ((rs2["v"] == ""));
	}

	try
	{
		session().setFeature("emptyStringIsNull", true);
		fail ("must fail");
	} catch (InvalidAccessException&) { }

	session().setFeature("emptyStringIsNull", esin);
	session().setFeature("forceEmptyString", fes);
}


void SQLExecutor::rowIterator()
{
	std::string funct = "rowIterator()";
	std::vector<Tuple<int, double, std::string> > v;
	v.push_back(Tuple<int, double, std::string>(1, 1.5f, "3"));
	v.push_back(Tuple<int, double, std::string>(2, 2.5f, "4"));
	v.push_back(Tuple<int, double, std::string>(3, 3.5f, "5"));
	v.push_back(Tuple<int, double, std::string>(4, 4.5f, "6"));

	try { session() << "DELETE FROM " << ExecUtil::vectors(), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	RecordSet rset0(session(), "SELECT * FROM " + ExecUtil::vectors());
	assertTrue (rset0.begin() == rset0.end());

	try { session() << "INSERT INTO " << ExecUtil::vectors() << " VALUES (?,?,?)", use(v), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	RecordSet rset(session(), "SELECT * FROM " + ExecUtil::vectors());

	std::ostringstream osLoop;
	RecordSet::Iterator it = rset.begin();
	RecordSet::Iterator end = rset.end();
	for (int i = 1; it != end; ++it, ++i)
	{
		assertTrue (it->get(0) == i);
		osLoop << *it;
	}
	assertTrue (!osLoop.str().empty());

	std::ostringstream osCopy;
	std::copy(rset.begin(), rset.end(), std::ostream_iterator<Row>(osCopy));
	assertTrue (osLoop.str() == osCopy.str());

	RowFilter::Ptr pRF = new RowFilter(rset);
	assertTrue (pRF->isEmpty());
	pRF->add("str0", RowFilter::VALUE_EQUAL, "3");
	assertTrue (!pRF->isEmpty());
	it = rset.begin();
	end = rset.end();
	for (int i = 1; it != end; ++it, ++i)
	{
		assertTrue (it->get(0) == i);
		assertTrue (1 == i);
	}
}


void SQLExecutor::stdVectorBool()
{
	std::string funct = "stdVectorBool()";

	bool b = false;
	try { session() << "INSERT INTO BoolTest VALUES (?)", use(b), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	b = true;
	session() << "SELECT * FROM BoolTest", into(b), now;
	assertTrue (false == b);
	session() << "DELETE FROM BoolTest", now;

	b = true;
	try { session() << "INSERT INTO BoolTest VALUES (?)", use(b), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	b = false;
	session() << "SELECT * FROM BoolTest", into(b), now;
	assertTrue (true == b);
	session() << "DELETE FROM BoolTest", now;

	std::vector<bool> v;
	v.push_back(true);
	v.push_back(false);
	v.push_back(false);
	v.push_back(true);

	try { session() << "INSERT INTO BoolTest VALUES (?)", use(v), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	v.clear();
	session() << "SELECT * FROM BoolTest", into(v), now;

	assertTrue (4 == v.size());
	std::vector<bool>::iterator it = v.begin();
	std::vector<bool>::iterator end = v.end();
	int t = 0;
	for (; it != end; ++it)
		t += *it ? 1 : 0;
	assertTrue (2 == t);

	try { session() << "SELECT * FROM BoolTest WHERE b = ?", out(v), now; fail("must fail"); }
	catch (BindingException&) { }

	try { session() << "SELECT * FROM BoolTest WHERE b = ?", io(v), now; fail("must fail"); }
	catch (BindingException&) { }

	RecordSet rset(session(), "SELECT * FROM BoolTest");

	t = 0;
	for (int i = 0; i < 4; ++i)
		t += rset.value<bool>(0, i) ? 1 : 0;
	assertTrue (2 == t);
}


void SQLExecutor::asynchronous(int rowCount)
{
	Session tmp = session();
	if (!_connInitSql.empty()) tmp << _connInitSql, now;

	std::vector<int> data(rowCount);
	Statement stmt = (tmp << "INSERT INTO " << ExecUtil::ints() << " VALUES(?)", use(data));
	Statement::Result result = stmt.executeAsync();
	assertTrue (!stmt.isAsync());
	result.wait();
	
	Statement stmt1 = (tmp << "SELECT * FROM " << ExecUtil::ints(), into(data), async, now);
	assertTrue (stmt1.isAsync());
	assertTrue (stmt1.wait() == rowCount);

	// +++ if this part of the test case fails, increase the rowCount until achieved
	//  that first execute is still executing when the second one is called
	stmt1.execute();
	try {
		stmt1.execute();
		fail ("execute() must fail");
	} catch (InvalidAccessException&)
	{
		stmt1.wait();
		stmt1.execute();
		stmt1.wait();
	}
	// ---

	stmt = tmp << "SELECT * FROM " << ExecUtil::ints(), into(data), async, now;
	assertTrue (stmt.isAsync());
	stmt.wait();
	assertTrue (stmt.execute() == 0);
	
	// +++ if this part of the test case fails, increase the rowCount until achieved
	//  that first execute is still executing when the second one is called
	try {
		result = stmt.executeAsync();
		fail ("executeAsync() must fail");
	} catch (InvalidAccessException&)
	{
		assertTrue (stmt.isAsync());
		stmt.wait();
		result = stmt.executeAsync();
	}
	// ---

	assertTrue (stmt.wait() == rowCount);
	assertTrue (result.data() == rowCount);
	stmt.setAsync(false);
	assertTrue (!stmt.isAsync());
	assertTrue (stmt.execute() == rowCount);

	stmt = tmp << "SELECT * FROM " << ExecUtil::ints(), into(data), sync, now;
	assertTrue (!stmt.isAsync());
	assertTrue (stmt.wait() == 0);
	assertTrue (stmt.execute() == rowCount);
	result = stmt.executeAsync();
	assertTrue (!stmt.isAsync());
	result.wait();
	assertTrue (result.data() == rowCount);

	assertTrue (0 == rowCount % 10);
	int step = (int) (rowCount/10);
	data.clear();
	Statement stmt2 = (tmp << "SELECT * FROM " << ExecUtil::ints(), into(data), async, limit(step));
	assertTrue (data.size() == 0);
	assertTrue (!stmt2.done());
	std::size_t rows = 0;
	
	for (int i = 0; !stmt2.done(); i += step)
	{
		stmt2.execute();
		rows = stmt2.wait();
		assertTrue (step == rows);
		assertTrue (step + i == data.size());
	}
	assertTrue (stmt2.done());
	assertTrue (rowCount == data.size());

	stmt2 = tmp << "SELECT * FROM " << ExecUtil::ints(), reset;
	assertTrue (!stmt2.isAsync());
	assertTrue ("deque" == stmt2.getStorage());
	assertTrue (stmt2.execute() == rowCount);
}


void SQLExecutor::any()
{
	Any i = 42;
	Any f = 42.5;
	std::string ss("42");
	Any s = ss;
#ifdef POCO_ODBC_UNICODE
	UTF16String us;
	Poco::UnicodeConverter::convert(ss, us);
	s = us;
#endif
	Session tmp = session();
	if (!_connInitSql.empty()) tmp << _connInitSql, now;

	tmp << "INSERT INTO " << ExecUtil::anys() << " VALUES (?, ?, ?)", use(i), use(f), use(s), now;

	int count = 0;
	tmp << "SELECT COUNT(*) FROM " << ExecUtil::anys(), into(count), now;
	assertTrue (1 == count);

	i = 0;
	f = 0.0;
#ifdef POCO_ODBC_UNICODE
	s = UTF16String();
#else
	s = std::string();
#endif
	tmp << "SELECT * FROM " << ExecUtil::anys(), into(i), into(f), into(s), now;
	assertTrue (AnyCast<int>(i) == 42);
	assertTrue (AnyCast<double>(f) == 42.5);
#ifdef POCO_ODBC_UNICODE
	// drivers may behave differently here
	try
	{
		assertTrue (AnyCast<UTF16String>(s) == us);
	}
	catch (BadCastException&)
	{
		assertTrue (AnyCast<std::string>(s) == "42");
	}
#else
	assertTrue (AnyCast<std::string>(s) == "42");
#endif
}


void SQLExecutor::dynamicAny()
{
	Var i = 42;
	Var f = 42.5;
	Var s = "42";

	Session tmp = session();
	if (!_connInitSql.empty()) tmp << _connInitSql, now;

	tmp << "INSERT INTO " << ExecUtil::anys() << " VALUES (?, ?, ?)", use(i), use(f), use(s), now;

	int count = 0;
	tmp << "SELECT COUNT(*) FROM " << ExecUtil::anys(), into(count), now;
	assertTrue (1 == count);

	i = 0;
	f = 0.0;
	s = std::string("");
	tmp << "SELECT * FROM " << ExecUtil::anys(), into(i), into(f), into(s), now;
	assertTrue (42 == i);
	assertTrue (42.5 == f);
	assertTrue ("42" == s);
}


void SQLExecutor::multipleResults(const std::string& sql)
{
	typedef Tuple<std::string, std::string, std::string, Poco::UInt32> Person;
	std::vector<Person> people;
	people.push_back(Person("Simpson", "Homer", "Springfield", 42));
	people.push_back(Person("Simpson", "Marge", "Springfield", 38));
	people.push_back(Person("Simpson", "Bart", "Springfield", 10));
	people.push_back(Person("Simpson", "Lisa", "Springfield", 8));
	people.push_back(Person("Simpson", "Maggie", "Springfield", 3));
	session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?, ?, ?, ?)", use(people), now;

	Person pHomer;
	int aHomer = 42, aLisa = 8;
	Poco::UInt32 aBart = 0;

	Poco::UInt32 pos1 = 1;
	int pos2 = 2;
	std::vector<Person> people2;
	Statement stmt(session());
	stmt << sql, into(pHomer, from(0)), use(aHomer)
		, into(aBart, pos1)
		, into(people2, from(pos2)), use(aLisa), use(aHomer);

	assertTrue (4 == stmt.execute());
	assertTrue (Person("Simpson", "Homer", "Springfield", 42) == pHomer);
	assertTrue (10 == aBart);
	assertTrue (2 == people2.size());
	assertTrue (Person("Simpson", "Lisa", "Springfield", 8) == people2[0]);
	assertTrue (Person("Simpson", "Homer", "Springfield", 42) == people2[1]);
}

typedef Tuple<std::string, std::string, std::string, Poco::UInt32> PersonMRT;

struct ReadPerson
{

	template <typename T>
	static PersonMRT rd(const T& rs)
	{
		PersonMRT pHomer;
		pHomer.set<0>(rs.value(0));
		pHomer.set<1>(rs.value(1));
		pHomer.set<2>(rs.value(2));
		pHomer.set<3>(rs.value(3));
		return pHomer;
	}

	template <typename Rdr>
	static void compare(SQLExecutor* tc, const Poco::Dynamic::Var& val,const Rdr& rdr)
	{
		if (val.type() == typeid(PersonMRT))
		{
			const PersonMRT p = rd(rdr);
			assertTU(tc, p == val.extract<PersonMRT>());
		}
		else
		{
			const Poco::Dynamic::Var val1 = rdr.value(0);
			assertTU(tc, val == val1);
		}
	}

  struct RSReader
  {
    RSReader(RecordSet& rs, size_t rowNo) :_rs(rs), _rowNo(rowNo)
    {}
    RecordSet& _rs;
    size_t _rowNo;
    Var value(size_t col) const
    {
      return _rs.value(col, _rowNo);
    }
  };

  struct ITReader
  {
    ITReader(const RowIterator& it) :_it(it)
    {}
    const RowIterator& _it;
    Var value(size_t col) const
    {
      return (*_it)[col];
    }
  };

  struct RSReaderCur
  {
    RSReaderCur(RecordSet& rs) :_rs(rs)
    {}
    RecordSet& _rs;
    Var value(size_t col) const
    {
      return _rs.value(col);
    }
  };

};

void SQLExecutor::multipleResultsNoProj(const std::string& sql)
{
	std::vector<PersonMRT> people;
	const PersonMRT Homer("Simpson", "Homer", "Springfield", 42);
	const int BartAge = 10;
	const int HomerAge = 42;
	const int LisaAge = 8;
	people.push_back(Homer);
	people.push_back(PersonMRT("Simpson", "Marge", "Springfield", 38));
	const std::string BartName("Bart");
	people.push_back(PersonMRT("Simpson", BartName, "Springfield", BartAge));
	const PersonMRT Lisa = PersonMRT("Simpson", "Lisa", "Springfield", LisaAge);
	people.push_back(Lisa);
	people.push_back(PersonMRT("Simpson", "Maggie", "Springfield", 3));
	session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?, ?, ?, ?)", use(people), now;
	Poco::SQL::Statement stmt(session());
	stmt << sql, useRef(HomerAge), useRef(BartName), useRef(LisaAge), useRef(HomerAge);


	const size_t rowsToGet = stmt.execute();
	assertTrue (3 == stmt.dataSetCount());
	stmt.firstDataSet();
	std::vector<Poco::Dynamic::Var> vals;
	vals.push_back(Poco::Dynamic::Var(Homer));
	vals.push_back(Poco::Dynamic::Var(BartAge));
	vals.push_back(Poco::Dynamic::Var(Lisa));
	vals.push_back(Poco::Dynamic::Var(Homer));

	std::vector<Poco::Dynamic::Var>::const_iterator valIt = vals.begin();
	size_t rowCnt = 0;
	for (size_t dsNo = 0; dsNo < stmt.dataSetCount(); dsNo = stmt.nextDataSet())
	{
		Poco::SQL::RecordSet rs(stmt);
		bool r = rs.moveFirst();
		RowIterator rowIt = rs.begin();
		for (size_t rowNo = 0; r; ++rowNo, r = rs.moveNext(), ++valIt, ++rowIt, ++rowCnt)
		{
			ReadPerson::compare(this, *valIt, ReadPerson::RSReader(rs, rowNo));
			ReadPerson::compare(this, *valIt, ReadPerson::ITReader(rowIt));
			ReadPerson::compare(this, *valIt, ReadPerson::RSReaderCur(rs));
		}
		assertTrue (rowIt == rs.end());
		if (!stmt.hasMoreDataSets())
			break;
	}
	assertTrue (rowCnt == rowsToGet);
	assertTrue (vals.end() == valIt);
	//// now check that limit() works as well
	for (size_t lim = 1; lim <= vals.size(); ++lim)
	{
		Poco::SQL::Statement stmt(session());
		stmt << sql, useRef(HomerAge), useRef(BartName), useRef(LisaAge), useRef(HomerAge), limit(lim);
		std::vector<Poco::Dynamic::Var>::const_iterator valIt = vals.begin();

		while (!stmt.done())
		{
			stmt.execute();
			for (bool doId = true; doId;)
			{
				Poco::SQL::RecordSet rs(stmt);

				RowIterator rIt = rs.begin();
				bool mf = rs.moveFirst();
				for (size_t row = 0; row < rs.rowCount(); ++row, ++rIt, mf = rs.moveNext(), ++valIt)
				{
					assertTrue (mf);
          ReadPerson::compare(this, *valIt, ReadPerson::RSReader(rs, row));
          ReadPerson::compare(this, *valIt, ReadPerson::ITReader(rIt));
          ReadPerson::compare(this, *valIt, ReadPerson::RSReaderCur(rs));
				}
				assertTrue (rIt == rs.end());

				doId = rs.rowCount() < lim && stmt.hasMoreDataSets() && stmt.nextDataSet() > 0;
			}
		}
	}

}


void SQLExecutor::sqlChannel(const std::string& connect)
{
	try
	{
		AutoPtr<SQLChannel> pChannel = new SQLChannel(Poco::SQL::ODBC::Connector::KEY, connect, "TestSQLChannel");
		pChannel->setProperty("table", schemaTable(ExecUtil::pocolog())); // has to be the first, as otherwise "table" won't take effect
		pChannel->setProperty("archive", schemaTable(ExecUtil::pocolog_a()));
		pChannel->setProperty("keep", "2 seconds");

		Message msgInf("InformationSource", "a Informational async message", Message::PRIO_INFORMATION);
		pChannel->log(msgInf);
		Message msgWarn("WarningSource", "b Warning async message", Message::PRIO_WARNING);
		pChannel->log(msgWarn);
		pChannel->wait();

		pChannel->setProperty("async", "false");
		Message msgInfS("InformationSource", "c Informational sync message", Message::PRIO_INFORMATION);
		pChannel->log(msgInfS);
		Message msgWarnS("WarningSource", "d Warning sync message", Message::PRIO_WARNING);
		pChannel->log(msgWarnS);

		RecordSet rs(session(), "SELECT * FROM " + ExecUtil::pocolog() + " ORDER by Text");
		assertTrue (4 == rs.rowCount());
		assertTrue ("InformationSource" == rs["Source"]);
		assertTrue ("a Informational async message" == rs["Text"]);
		rs.moveNext();
		assertTrue ("WarningSource" == rs["Source"]);
		assertTrue ("b Warning async message" == rs["Text"]);
		rs.moveNext();
		assertTrue ("InformationSource" == rs["Source"]);
		assertTrue ("c Informational sync message" == rs["Text"]);
		rs.moveNext();
		assertTrue ("WarningSource" == rs["Source"]);
		assertTrue ("d Warning sync message" == rs["Text"]);

		Thread::sleep(3000);

		Message msgInfA("InformationSource", "e Informational sync message", Message::PRIO_INFORMATION);
		pChannel->log(msgInfA);
		Message msgWarnA("WarningSource", "f Warning sync message", Message::PRIO_WARNING);
		pChannel->log(msgWarnA);

		RecordSet rs1(session(), "SELECT * FROM " + ExecUtil::pocolog_a());
		assertTrue (4 == rs1.rowCount());

		pChannel->setProperty("keep", "");
		assertTrue ("forever" == pChannel->getProperty("keep"));
		RecordSet rs2(session(), "SELECT * FROM " + ExecUtil::pocolog() + " ORDER by Text");
		assertTrue (2 == rs2.rowCount());
		assertTrue ("InformationSource" == rs2["Source"]);
		assertTrue ("e Informational sync message" == rs2["Text"]);
		rs2.moveNext();
		assertTrue ("WarningSource" == rs2["Source"]);
		assertTrue ("f Warning sync message" == rs2["Text"]);
		
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("sqlChannel()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("sqlChannel()"); }
}


void SQLExecutor::sqlLogger(const std::string& connect)
{
	try
	{
		Logger& root = Logger::root();
		SQLChannel::Ptr ch = new SQLChannel(Poco::SQL::ODBC::Connector::KEY, connect, "TestSQLChannel");
		ch->setProperty("table", schemaTable(ExecUtil::pocolog()));
		root.setChannel(ch);
		root.setLevel(Message::PRIO_INFORMATION);

		root.information("a Informational message");
		root.warning("b Warning message");
		root.debug("Debug message");

		Thread::sleep(100);
		RecordSet rs(session(), "SELECT * FROM " + ExecUtil::pocolog() + " ORDER by Text");
		assertTrue (2 == rs.rowCount());
		assertTrue ("TestSQLChannel" == rs["Source"]);
		assertTrue ("a Informational message" == rs["Text"]);
		rs.moveNext();
		assertTrue ("TestSQLChannel" == rs["Source"]);
		assertTrue ("b Warning message" == rs["Text"]);
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("sqlLogger()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("sqlLogger()"); }
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
		std::cerr << '[' << name() << ']' << " Warning, transaction isolation not supported: ";
		switch (ti)
		{
		case Session::TRANSACTION_READ_COMMITTED:
			std::cerr << "READ COMMITTED"; break;
		case Session::TRANSACTION_READ_UNCOMMITTED:
			std::cerr << "READ UNCOMMITTED"; break;
		case Session::TRANSACTION_REPEATABLE_READ:
			std::cerr << "REPEATABLE READ"; break;
		case Session::TRANSACTION_SERIALIZABLE:
			std::cerr << "SERIALIZABLE"; break;
		default:
			std::cerr << "UNKNOWN"; break;
		}
		std::cerr << std::endl;
	}
}


void SQLExecutor::sessionTransaction(const std::string& connect)
{
	if (!session().canTransact())
	{
		std::cout << "Session not capable of transactions." << std::endl;
		return;
	}

	Session local("odbc", connect);
	if (!_connInitSql.empty()) local << _connInitSql, now;
	local.setFeature("autoCommit", true);

	std::string funct = "transaction()";
	std::vector<std::string> lastNames;
	std::vector<std::string> firstNames;
	std::vector<std::string> addresses;
	std::vector<int> ages;
	const std::string tableName(ExecUtil::person());
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

	bool autoCommit = session().getFeature("autoCommit");

	session().setFeature("autoCommit", true);
	assertTrue (!session().isTransaction());
	session().setFeature("autoCommit", false);
	assertTrue (!session().isTransaction());

	setTransactionIsolation(session(), Session::TRANSACTION_READ_UNCOMMITTED);
	setTransactionIsolation(session(), Session::TRANSACTION_REPEATABLE_READ);
	setTransactionIsolation(session(), Session::TRANSACTION_SERIALIZABLE);
	setTransactionIsolation(session(), Session::TRANSACTION_READ_COMMITTED);

	session().begin();
	assertTrue (session().isTransaction());
	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (session().isTransaction());

	Statement stmt = (local << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(locCount), async, now);

	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (2 == count);
	assertTrue (session().isTransaction());
	session().rollback();
	assertTrue (!session().isTransaction());

	stmt.wait();
	assertTrue (0 == locCount || 2 == locCount); // the stmt might have executed before or after the rollback

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (0 == count);
	assertTrue (!session().isTransaction());

	session().begin();
	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (session().isTransaction());

	Statement stmt1 = (local << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(locCount), async, now);

	session().commit();
	assertTrue (!session().isTransaction());

	stmt1.wait();
	assertTrue (2 == locCount);

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (2 == count);

	session().setFeature("autoCommit", autoCommit);
}


void SQLExecutor::transaction(const std::string& connect)
{
	if (!session().canTransact())
	{
		std::cout << "Session not transaction-capable." << std::endl;
		return;
	}

	Session local("odbc", connect);
	local.setFeature("autoCommit", true);
	if (!_connInitSql.empty()) local << _connInitSql, now;

	setTransactionIsolation(session(), Session::TRANSACTION_READ_COMMITTED);
	if (local.hasTransactionIsolation(Session::TRANSACTION_READ_UNCOMMITTED))
		setTransactionIsolation(local, Session::TRANSACTION_READ_UNCOMMITTED);
	else if (local.hasTransactionIsolation(Session::TRANSACTION_READ_COMMITTED))
		setTransactionIsolation(local, Session::TRANSACTION_READ_COMMITTED);

	std::string funct = "transaction()";
	std::vector<std::string> lastNames;
	std::vector<std::string> firstNames;
	std::vector<std::string> addresses;
	std::vector<int> ages;
	const std::string tableName(ExecUtil::person());
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

	bool autoCommit = session().getFeature("autoCommit");

	session().setFeature("autoCommit", true);
	assertTrue (!session().isTransaction());
	session().setFeature("autoCommit", false);
	assertTrue (!session().isTransaction());
	session().setTransactionIsolation(Session::TRANSACTION_READ_COMMITTED);

	{
		Transaction trans(session());
		assertTrue (trans.isActive());
		assertTrue (session().isTransaction());
		
		try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		
		assertTrue (session().isTransaction());
		assertTrue (trans.isActive());

		try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assertTrue (2 == count);
		assertTrue (session().isTransaction());
		assertTrue (trans.isActive());
	}
	assertTrue (!session().isTransaction());

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (0 == count);
	assertTrue (!session().isTransaction());

	{
		Transaction trans(session());
		try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastNames), use(firstNames), use(addresses), use(ages), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		Statement stmt1 = (local << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(locCount), async, now);

		assertTrue (session().isTransaction());
		assertTrue (trans.isActive());
		trans.commit();
		assertTrue (!session().isTransaction());
		assertTrue (!trans.isActive());

		stmt1.wait();
		assertTrue (2 == locCount);
	}

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (2 == count);

	try { session() << "DELETE FROM " << ExecUtil::person(), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	Statement stmt1 = (local << "SELECT count(*) FROM " << ExecUtil::person(), into(locCount), async, now);

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (0 == count);
	try
	{
		stmt1.wait();
		if (local.getTransactionIsolation() == Session::TRANSACTION_READ_UNCOMMITTED)
			assertTrue (0 == locCount);
	} catch (TimeoutException&)
	{ std::cerr << '[' << name() << ']' << " Warning: async query timed out." << std::endl; }
	session().commit();
	// repeat for those that don't support uncommitted read isolation
	if (local.getTransactionIsolation() == Session::TRANSACTION_READ_COMMITTED)
	{
		stmt1.wait();
		local << "SELECT count(*) FROM " << ExecUtil::person(), into(locCount), now;
		assertTrue (0 == locCount);
	}

	std::string sql1 = format("INSERT INTO %s VALUES ('%s','%s','%s',%d)", tableName, lastNames[0], firstNames[0], addresses[0], ages[0]);
	std::string sql2 = format("INSERT INTO %s VALUES ('%s','%s','%s',%d)", tableName, lastNames[1], firstNames[1], addresses[1], ages[1]);
	std::vector<std::string> sql;
	sql.push_back(sql1);
	sql.push_back(sql2);

	Transaction trans(session());

	trans.execute(sql1, false);
	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (1 == count);
	trans.execute(sql2, false);
	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (2 == count);

	Statement stmt2 = (local << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(locCount), async, now);

	trans.rollback();

	stmt2.wait();
	assertTrue (0 == locCount);

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (0 == count);

	trans.execute(sql);
	
	Statement stmt3 = (local << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(locCount), now);
	assertTrue (2 == locCount);

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (2 == count);

	session().setFeature("autoCommit", autoCommit);
}


struct TestCommitTransactor
{
	void operator () (Session& session) const
	{
		session << "INSERT INTO " << ExecUtil::person() <<" VALUES ('lastName','firstName','address',10)", now;
	}
};


struct TestRollbackTransactor
{
	void operator () (Session& session) const
	{
		session << "INSERT INTO " << ExecUtil::person() <<" VALUES ('lastName','firstName','address',10)", now;
		throw Poco::Exception("test");
	}
};


void SQLExecutor::transactor()
{
	std::string funct = "transaction()";
	int count = 0;

	bool autoCommit = session().getFeature("autoCommit");
	session().setFeature("autoCommit", false);
	session().setTransactionIsolation(Session::TRANSACTION_READ_COMMITTED);

	TestCommitTransactor ct;
	Transaction t1(session(), ct);

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (1 == count);

	try { session() << "DELETE FROM " << ExecUtil::person(), now; session().commit();}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	
	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (0 == count);

	try
	{
		TestRollbackTransactor rt;
		Transaction t(session(), rt);
		fail ("must fail");
	} catch (Poco::Exception&) { }

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (0 == count);

	try
	{
		TestRollbackTransactor rt;
		Transaction t(session());
		t.transact(rt);
		fail ("must fail");
	} catch (Poco::Exception&) { }

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (0 == count);

	try
	{
		TestRollbackTransactor rt;
		Transaction t(session(), false);
		t.transact(rt);
		fail ("must fail");
	} catch (Poco::Exception&) { }

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (0 == count);

	try
	{
		TestRollbackTransactor rt;
		Transaction t(session(), true);
		t.transact(rt);
		fail ("must fail");
	} catch (Poco::Exception&) { }

	try { session() << "SELECT count(*) FROM " << ExecUtil::person(), into(count), now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (0 == count);

	session().setFeature("autoCommit", autoCommit);
}


void SQLExecutor::nullable()
{
  Statement stat(session());
  try {
		Nullable<int> nint;
		session() << "INSERT INTO " << ExecUtil::nullabletest() <<
		" (EmptyString, EmptyInteger, EmptyFloat, EmptyDateTime) VALUES(?, ?, ?, ?)", bind(Nullable<std::string>()), useRef(nint), bind(Nullable<double>()), bind(Nullable<DateTime>()), now;
	}
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("nullable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("nullable()"); }

	Nullable<int> i = 1;
	Nullable<double> f = 1.5;
	Nullable<std::string> s = std::string("abc");
	Nullable<DateTime> d = DateTime();

	assertTrue (!i.isNull());
	assertTrue (!f.isNull());
	assertTrue (!s.isNull());
	assertTrue (!d.isNull());

	session() << "SELECT EmptyString, EmptyInteger, EmptyFloat, EmptyDateTime FROM " << ExecUtil::nullabletest(), into(s), into(i), into(f), into(d), now;

	assertTrue (i.isNull());
	assertTrue (f.isNull());
	assertTrue (s.isNull());
	assertTrue (d.isNull());

	RecordSet rs(session(), "SELECT * FROM " + ExecUtil::nullabletest());

	rs.moveFirst();
	assertTrue (rs.isNull("EmptyString"));
	assertTrue (rs.isNull("EmptyInteger"));
	assertTrue (rs.isNull("EmptyFloat"));
	assertTrue (rs.isNull("EmptyDateTime"));
	
	Var di = 1;
	Var df = 1.5;
	Var ds = "abc";
	Var dd = DateTime();

	assertTrue (!di.isEmpty());
	assertTrue (!df.isEmpty());
	assertTrue (!ds.isEmpty());
	assertTrue (!dd.isEmpty());
	
	Statement stmt = (session() << "SELECT EmptyString, EmptyInteger, EmptyFloat, EmptyDateTime FROM " << ExecUtil::nullabletest(), into(ds), into(di), into(df), into(dd), now);

	assertTrue (di.isEmpty());
	assertTrue (df.isEmpty());
	assertTrue (ds.isEmpty());
	assertTrue (dd.isEmpty());
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

	try { session() << "INSERT INTO " << ExecUtil::person() <<" VALUES (?,?,?,?)", use(lastName), use(firstName), use(address), use(age), now;  }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

	count = 0;
	try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now;  }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == 1);

	assertTrue (session().isConnected());
	session().close();
	assertTrue (!session().isConnected());
	try
	{
		session() << "SELECT LastName FROM " << ExecUtil::person(), into(result), now;
		fail ("must fail");
	}
	catch(NotConnectedException&){ }
	assertTrue (!session().isConnected());

	session().open();
	if (!_connInitSql.empty()) session() << _connInitSql, now;
	assertTrue (session().isConnected());
	try { session() << "SELECT Age FROM " << ExecUtil::person(), into(count), now;  }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	assertTrue (count == age);
	assertTrue (session().isConnected());
}


void SQLExecutor::unicode(const std::string& dbConnString)
{
	const unsigned char supp[] = { 0x41, 0x42, 0xf0, 0x90, 0x82, 0xa4, 0xf0, 0xaf, 0xa6, 0xa0, 0xf0, 0xaf, 0xa8, 0x9d, 0x00 };
	std::string text((const char*) supp);

	UTF16String wtext;
	Poco::UnicodeConverter::convert(text, wtext);
	session() << "INSERT INTO UnicodeTable VALUES (?)", use(wtext), now;
	wtext.clear();
	text.clear();
	session() << "SELECT str FROM UnicodeTable", into(wtext), now;
	Poco::UnicodeConverter::convert(wtext, text);
	assertTrue (text == std::string((const char*)supp));
}


void SQLExecutor::insertStatReuse()
{
	const std::string funct = "insertStatReuse()";
	Statement stat(session());
	try {
		Var lastName;
		std::string firstName("zzz");
		Any address;
		Nullable<int> age(0);
		stat << "INSERT INTO " << ExecUtil::person() << "(LastName, FirstName, Address, Age) VALUES (?,?,?,?)", use(lastName), use(firstName), use(address), use(age);
		stat.insertHint();
		for (int i = 1; i < 5; ++i)
		{
			lastName = Var("Last Name " + NumberFormatter::format(i));
			firstName = "First Name " + NumberFormatter::format(i);
			address = "Address" + NumberFormatter::format(i);
			age = 10 + i;
			stat.execute();
		}
		std::vector<int> rowCnt;
		session() << "SELECT count(*) FROM " << ExecUtil::person() << " AS p "
			<< " WHERE p.LastName LIKE 'Last%' AND p.FirstName LIKE 'First%' AND p.Address LIKE 'Address%' AND p.Age>10"
			<< " GROUP BY p.LastName, p.FirstName, p.Address, p.Age", into(rowCnt), now;
		assertTrue (4 == rowCnt.size());
		size_t sum = 0;
		for (size_t i = 0; i < rowCnt.size(); ++i)
			sum += rowCnt[i];
		assertTrue (4 == sum);
	}
	catch (ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail(funct); }
}
