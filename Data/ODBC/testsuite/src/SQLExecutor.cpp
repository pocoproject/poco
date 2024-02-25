//
// SQLExecutor.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CppUnit/TestCase.h"
#include "SQLExecutor.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Tuple.h"
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
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/RowIterator.h"
#include "Poco/Data/RowFilter.h"
#include "Poco/Data/BulkExtraction.h"
#include "Poco/Data/BulkBinding.h"
#include "Poco/Data/SQLChannel.h"
#include "Poco/Data/Transaction.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Data/ODBC/Error.h"
#include "Poco/Data/ODBC/Preparator.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/UTFString.h"
#include <sqltypes.h>
#include <iostream>
#include <sstream>
#include <iterator>


using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::SessionPool;
using Poco::Data::Statement;
using Poco::Data::RecordSet;
using Poco::Data::Column;
using Poco::Data::Row;
using Poco::Data::RowFilter;
using Poco::Data::RowIterator;
using Poco::Data::SQLChannel;
using Poco::Data::LimitException;
using Poco::Data::BindingException;
using Poco::Data::CLOB;
using Poco::Data::Date;
using Poco::Data::Time;
using Poco::Data::Transaction;
using Poco::Data::NotConnectedException;
using Poco::Data::ODBC::Utility;
using Poco::Data::ODBC::Preparator;
using Poco::Data::ODBC::ConnectionException;
using Poco::Data::ODBC::StatementException;
using Poco::Data::ODBC::DataTruncatedException;
using Poco::Data::ODBC::ConnectionError;
using Poco::Data::ODBC::StatementError;
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


const std::string SQLExecutor::MULTI_INSERT =
	"INSERT INTO Test VALUES ('1', 2, 3.5);"
	"INSERT INTO Test VALUES ('2', 3, 4.5);"
	"INSERT INTO Test VALUES ('3', 4, 5.5);"
	"INSERT INTO Test VALUES ('4', 5, 6.5);"
	"INSERT INTO Test VALUES ('5', 6, 7.5);";

const std::string SQLExecutor::MULTI_SELECT =
	"SELECT * FROM Test WHERE First = '1';"
	"SELECT * FROM Test WHERE First = '2';"
	"SELECT * FROM Test WHERE First = '3';"
	"SELECT * FROM Test WHERE First = '4';"
	"SELECT * FROM Test WHERE First = '5';";


SQLExecutor::SQLExecutor(const std::string& name, Poco::Data::Session* pSession, Poco::Data::Session* pEncSession):
	CppUnit::TestCase(name),
	_pSession(pSession),
	_pEncSession(pEncSession),
	_dataExecutor("Poco::Data SQL Executor", pSession, pEncSession)
{
}


SQLExecutor::~SQLExecutor()
{
}


Poco::Data::Session& SQLExecutor::session(bool enc)
{
	if (!enc)
	{
		poco_check_ptr(_pSession);
		return *_pSession;
	}
	else
	{
		poco_check_ptr(_pEncSession);
		return *_pEncSession;
	}
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
		SQLSMALLINT dateTimeDecDigits = -1;
		if (SQL_SUCCEEDED(rc))
		{
			SQLLEN ind = 0;
			rc = SQLGetData(hstmt, 3, SQL_C_SLONG, &dateTimeColSize, sizeof(SQLINTEGER), &ind);
			poco_odbc_check_stmt (rc, hstmt);
			rc = SQLGetData(hstmt, 15, SQL_C_SSHORT, &dateTimeDecDigits, sizeof(SQLSMALLINT), &ind);
			poco_odbc_check_stmt (rc, hstmt);

			assertTrue (sizeof(SQL_TIMESTAMP_STRUCT) <= dateTimeColSize);
		}
		else if (SQL_NO_DATA == rc)
			std::cerr << '[' << name() << ']' << " Warning: no SQL_TYPE_TIMESTAMP data type info returned by driver." << std::endl;

		rc = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		poco_odbc_check_stmt (rc, hstmt);

			// Statement begin
			rc = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
			poco_odbc_check_stmt (rc, hstmt);

			std::string sql = "DROP TABLE Test";
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

			sql = format("INSERT INTO Test VALUES (?,?,%s,?,?,?)", blobPlaceholder);
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

			if (dateTimeColSize == 0 || dateTimeDecDigits == -1)
			{
				SQLSMALLINT dataType = 0;
				SQLULEN parameterSize = 0;
				SQLSMALLINT decimalDigits = -1;
				SQLSMALLINT nullable = 0;
				rc = SQLDescribeParam(hstmt, 6, &dataType, &parameterSize, &decimalDigits, &nullable);
				if (SQL_SUCCEEDED(rc))
				{
					if (parameterSize != 0 && dateTimeColSize == 0)
						dateTimeColSize = parameterSize;
					if (decimalDigits != -1 && dateTimeDecDigits == -1)
						dateTimeDecDigits = decimalDigits;
				}
				else
				{
					std::cerr << '[' << name() << ']' << " Warning: could not get SQL_TYPE_TIMESTAMP "
						"parameter description." << std::endl;
				}
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

			sql = "SELECT * FROM Test";
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

			sql = "DROP TABLE Test";
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
	const std::string& select,
	const std::string& procCreateString)
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

			std::string sql = "DROP TABLE Test";
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

			// create stored proc, if provided
			if (!procCreateString.empty())
			{
				sql = procCreateString;
				pStr = (SQLCHAR*) sql.c_str();
				rc = SQLPrepare(hstmt, pStr, (SQLINTEGER) sql.length());
				poco_odbc_check_stmt (rc, hstmt);

				rc = SQLExecute(hstmt);
				poco_odbc_check_stmt (rc, hstmt);
			}

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
			sql = "select count(*) from Test";
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

			sql = "DROP TABLE Test";
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


void SQLExecutor::bareboneODBCStoredFuncTest(const std::string& dbConnString,
	const std::string& procCreateString,
	const std::string& procExecuteString,
	SQLExecutor::DataBinding bindMode,
	SQLExecutor::DataExtraction extractMode)
{
	SQLRETURN rc;
	SQLHENV henv = SQL_NULL_HENV;
	SQLHDBC hdbc = SQL_NULL_HDBC;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;

	// Environment begin
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	poco_odbc_check_env(rc, henv);
	rc = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	poco_odbc_check_env(rc, henv);

	// Connection begin
	rc = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	poco_odbc_check_dbc(rc, hdbc);

	SQLCHAR connectOutput[1024] = { 0 };
	SQLSMALLINT result;
	rc = SQLDriverConnect(hdbc
		, NULL
		, (SQLCHAR*)dbConnString.c_str()
		, (SQLSMALLINT)SQL_NTS
		, connectOutput
		, sizeof(connectOutput)
		, &result
		, SQL_DRIVER_NOPROMPT);
	poco_odbc_check_dbc(rc, hdbc);

	// retrieve datetime type information for this DBMS
	rc = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	poco_odbc_check_stmt(rc, hstmt);

	rc = SQLGetTypeInfo(hstmt, SQL_TYPE_TIMESTAMP);
	poco_odbc_check_stmt(rc, hstmt);

	rc = SQLFetch(hstmt);
	assertTrue(SQL_SUCCEEDED(rc) || SQL_NO_DATA == rc);

	SQLULEN dateTimeColSize = 0;
	SQLSMALLINT dateTimeDecDigits = -1;
	if (SQL_SUCCEEDED(rc))
	{
		SQLLEN ind = 0;
		rc = SQLGetData(hstmt, 3, SQL_C_SLONG, &dateTimeColSize, sizeof(SQLINTEGER), &ind);
		poco_odbc_check_stmt(rc, hstmt);
		rc = SQLGetData(hstmt, 15, SQL_C_SSHORT, &dateTimeDecDigits, sizeof(SQLSMALLINT), &ind);
		poco_odbc_check_stmt(rc, hstmt);

		assertTrue(sizeof(SQL_TIMESTAMP_STRUCT) <= dateTimeColSize);
	}
	else if (SQL_NO_DATA == rc)
		std::cerr << '[' << name() << ']' << " Warning: no SQL_TYPE_TIMESTAMP data type info returned by driver." << std::endl;

	rc = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	poco_odbc_check_stmt(rc, hstmt);

		// Statement begin
		rc = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
		poco_odbc_check_stmt(rc, hstmt);

		std::string sql = "DROP PROCEDURE TestStoredProcedure";
		SQLCHAR* pStr = (SQLCHAR*)sql.c_str();
		SQLExecDirect(hstmt, pStr, (SQLINTEGER)sql.length());
		//no return code check - ignore drop errors

		// create stored prcedure and go
		sql = procCreateString;
		pStr = (SQLCHAR*)sql.c_str();
		rc = SQLPrepare(hstmt, pStr, (SQLINTEGER)sql.length());
		poco_odbc_check_stmt(rc, hstmt);

		rc = SQLExecute(hstmt);
		poco_odbc_check_stmt(rc, hstmt);

		char inParam[10] = "1234";
		char outParam[10] = "";
		char retVal[10] = "";

		sql = procExecuteString;
		pStr = (SQLCHAR*)sql.c_str();
		rc = SQLPrepare(hstmt, pStr, (SQLINTEGER)sql.length());
		poco_odbc_check_stmt(rc, hstmt);

		SQLLEN li[3] = { SQL_NTS, SQL_NTS, SQL_NTS };
		SQLINTEGER size = (SQLINTEGER)strlen(inParam);

		if (SQLExecutor::PB_AT_EXEC == bindMode)
			li[0] = SQL_LEN_DATA_AT_EXEC(size);

		rc = SQLBindParameter(hstmt,
			(SQLUSMALLINT)1,
			SQL_PARAM_OUTPUT,
			SQL_C_CHAR,
			SQL_VARCHAR,
			(SQLUINTEGER)sizeof(retVal),
			0,
			(SQLPOINTER)retVal,
			sizeof(retVal),
			&li[0]);
		poco_odbc_check_stmt(rc, hstmt);

		if (SQLExecutor::PB_AT_EXEC == bindMode)
			li[1] = SQL_LEN_DATA_AT_EXEC(size);

		rc = SQLBindParameter(hstmt,
			(SQLUSMALLINT)2,
			SQL_PARAM_INPUT,
			SQL_C_CHAR,
			SQL_VARCHAR,
			(SQLUINTEGER)sizeof(inParam),
			0,
			(SQLPOINTER)inParam,
			sizeof(inParam),
			&li[1]);
		poco_odbc_check_stmt(rc, hstmt);

		if (SQLExecutor::PB_AT_EXEC == bindMode)
			li[2] = SQL_LEN_DATA_AT_EXEC(size);

		rc = SQLBindParameter(hstmt,
			(SQLUSMALLINT)3,
			SQL_PARAM_OUTPUT,
			SQL_C_CHAR,
			SQL_VARCHAR,
			(SQLUINTEGER)sizeof(outParam),
			0,
			(SQLPOINTER)outParam,
			sizeof(outParam),
			&li[2]);
		poco_odbc_check_stmt(rc, hstmt);

		rc = SQLExecute(hstmt);
		if (rc && SQL_NEED_DATA != rc)
		{
			std::cout << "rc=" << rc << ", SQL_NEED_DATA=" << SQL_NEED_DATA << '\n';
			poco_odbc_check_stmt(rc, hstmt);
		}
		assertTrue(SQL_NEED_DATA == rc || SQL_SUCCEEDED(rc));

		if (SQL_NEED_DATA == rc)
		{
			SQLPOINTER pParam = 0;
			while (SQL_NEED_DATA == (rc = SQLParamData(hstmt, &pParam)))
			{
				if ((pParam != (SQLPOINTER)retVal) &&
					(pParam != (SQLPOINTER)inParam) &&
					(pParam != (SQLPOINTER)outParam))
				{
					fail("Parameter mismatch.");
				}

				assertTrue(0 != (SQLINTEGER)size);
				rc = SQLPutData(hstmt, pParam, (SQLINTEGER)size);
				poco_odbc_check_stmt(rc, hstmt);
			}
		}
		poco_odbc_check_stmt(rc, hstmt);
		assertTrue(std::string(outParam) == std::string(inParam));
		assertTrue(std::string(retVal) == std::string(inParam));

		sql = "DROP PROCEDURE TestStoredProcedure";
		pStr = (SQLCHAR*)sql.c_str();
		rc = SQLExecDirect(hstmt, pStr, (SQLINTEGER)sql.length());
		poco_odbc_check_stmt(rc, hstmt);

		rc = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		poco_odbc_check_stmt(rc, hstmt);

	// Connection end
	rc = SQLDisconnect(hdbc);
	poco_odbc_check_dbc(rc, hdbc);
	rc = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	poco_odbc_check_dbc(rc, hdbc);

	// Environment end
	rc = SQLFreeHandle(SQL_HANDLE_ENV, henv);
	poco_odbc_check_env(rc, henv);
}


void SQLExecutor::execute(const std::string& sql)
{
	try { session() << sql, now;  }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (sql); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (sql); }
}


void SQLExecutor::connection(const std::string& connectString)
{
	std::cout << connectString << std::endl;
	Poco::Data::ODBC::Connection c;
	try { _dataExecutor.connection(c, connectString); }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (connectString, __LINE__, __FILE__); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (connectString, __LINE__, __FILE__); }
	catch(Poco::Exception& se){ std::cout << se.displayText() << std::endl; fail (connectString, __LINE__, __FILE__); }
}


void SQLExecutor::session(const std::string& connectString, int timeout)
{
	Poco::Data::Session s(Poco::Data::ODBC::Connector::KEY, connectString, timeout);
	assertTrue (s.isConnected());
	s.close();
	assertTrue (!s.isConnected());
	s.open();
	assertTrue (s.isConnected());
	s.reconnect();
	assertTrue (s.isConnected());
	s.close();
	assertTrue (!s.isConnected());
	s.reconnect();
	assertTrue (s.isConnected());

	Poco::Any any = s.getProperty("handle");
	assertTrue (typeid(SQLHDBC) == any.type());
	SQLHDBC hdbc = Poco::AnyCast<SQLHDBC>(any);
	assertTrue (SQL_NULL_HDBC != hdbc);
	SQLRETURN rc = SQLDisconnect(hdbc);
	assertTrue (!Utility::isError(rc));
	assertTrue (!s.isConnected());
	s.open();
	assertTrue (s.isConnected());

	hdbc = Poco::AnyCast<SQLHDBC>(any);
	rc = SQLDisconnect(hdbc);
	assertTrue (!Utility::isError(rc));
	assertTrue (!s.isConnected());
	s.reconnect();
	assertTrue (s.isConnected());
	s.close();
	assertTrue (!s.isConnected());
	s.reconnect();
	assertTrue (s.isConnected());
	s.reconnect();
	assertTrue (s.isConnected());
}


void SQLExecutor::notNulls(const std::vector<std::string>& sqlStates)
{
	try
	{
		session() << "INSERT INTO NullTest (i,r,v) VALUES (?,?,?)", use(null), use(null), use(null), now;
		fail ("must fail");
	} catch (StatementException& se)
	{
		//double check if we're failing for the right reason
		//default sqlState value is "23502"; some drivers report "HY???" codes
		if (se.diagnostics().fields().size())
		{
			std::string st = se.diagnostics().sqlState(0);
			for (const auto& sqlState : sqlStates)
				if (sqlState == st) return;

			std::string strSQLStates;
			for (const auto& sqlState : sqlStates)
				strSQLStates.append(sqlState).append(1, ' ');
			std::cerr << '[' << name() << ']' << " Warning: expected SQL state [" << strSQLStates <<
				"], received [" << st << "] instead." << std::endl;
		}
	}
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

	tmp << "INSERT INTO Anys VALUES (?, ?, ?)", use(i), use(f), use(s), now;

	int count = 0;
	tmp << "SELECT COUNT(*) FROM Anys", into(count), now;
	assertTrue (1 == count);

	i = 0;
	f = 0.0;
	s = std::string("");
	tmp << "SELECT * FROM Anys", into(i), into(f), into(s), now;
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
