//
// Utility.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Utility
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Handle.h"
#include "Poco/Data/ODBC/ConnectionHandle.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTime.h"
#include <algorithm>
#include <cmath>
#include <vector>


namespace Poco::Data::ODBC {


const TypeInfo Utility::_dataTypes;


Utility::DriverMap& Utility::drivers(Utility::DriverMap& driverMap)
{
	static const EnvironmentHandle henv;
	const int length = sizeof(SQLCHAR) * 512;

	SQLCHAR desc[length];
	std::memset(desc, 0, length);
	SQLSMALLINT len1 = length;
	SQLCHAR attr[length];
	std::memset(attr, 0, length);
	SQLSMALLINT len2 = length;
	RETCODE rc = 0;

	if (!Utility::isError(rc = SQLDrivers(henv,
		SQL_FETCH_FIRST,
		desc,
		length,
		&len1,
		attr,
		len2,
		&len2)))
	{
		do
		{
			driverMap.insert(DSNMap::value_type(std::string((char *) desc),
				std::string((char *) attr)));
			std::memset(desc, 0, length);
			std::memset(attr, 0, length);
			len2 = length;
		}while (!Utility::isError(rc = SQLDrivers(henv,
			SQL_FETCH_NEXT,
			desc,
			length,
			&len1,
			attr,
			len2,
			&len2)));
	}

	if (SQL_NO_DATA != rc)
		throw EnvironmentException(henv);

	return driverMap;
}


Utility::DSNMap& Utility::dataSources(Utility::DSNMap& dsnMap)
{
	static const EnvironmentHandle henv;
	const int length = sizeof(SQLCHAR) * 512;
	const int dsnLength = sizeof(SQLCHAR) * (SQL_MAX_DSN_LENGTH + 1);

	SQLCHAR dsn[dsnLength];
	std::memset(dsn, 0, dsnLength);
	SQLSMALLINT len1 = sizeof(SQLCHAR) * SQL_MAX_DSN_LENGTH;
	SQLCHAR desc[length];
	std::memset(desc, 0, length);
	SQLSMALLINT len2 = length;
	RETCODE rc = 0;

	while (!Utility::isError(rc = Poco::Data::ODBC::SQLDataSources(henv,
		SQL_FETCH_NEXT,
		dsn,
		SQL_MAX_DSN_LENGTH,
		&len1,
		desc,
		len2,
		&len2)))
	{
		dsnMap.insert(DSNMap::value_type(std::string((char *) dsn), std::string((char *) desc)));
		std::memset(dsn, 0, dsnLength);
		std::memset(desc, 0, length);
		len2 = length;
	}

	if (SQL_NO_DATA != rc)
		throw EnvironmentException(henv);

	return dsnMap;
}


void Utility::dateTimeSync(Poco::DateTime& dt, const SQL_TIMESTAMP_STRUCT& ts)
{
	double msec = ts.fraction/1000000;
	double usec = 1000 * (msec - std::floor(msec));

	dt.assign(ts.year,
		ts.month,
		ts.day,
		ts.hour,
		ts.minute,
		ts.second,
		(int) std::floor(msec),
		(int) std::floor(usec));
}


void Utility::dateSync(SQL_DATE_STRUCT& ds, const Date& d)
{
	ds.year = d.year();
	ds.month = d.month();
	ds.day = d.day();
}


void Utility::timeSync(SQL_TIME_STRUCT& ts, const Time& t)
{
	ts.hour = t.hour();
	ts.minute = t.minute();
	ts.second = t.second();
}


void Utility::dateTimeSync(SQL_TIMESTAMP_STRUCT& ts, const Poco::DateTime& dt)
{
	ts.year = dt.year();
	ts.month = dt.month();
	ts.day = dt.day();
	ts.hour = dt.hour();
	ts.minute = dt.minute();
	ts.second = dt.second();
	// Fraction support is limited to milliseconds due to MS SQL Server limitation
	// see http://support.microsoft.com/kb/263872
	ts.fraction = (dt.millisecond() * 1000000);
}


std::string Utility::sqlGetInfo(const ConnectionHandle& db, SQLUSMALLINT type)
{
	return sqlGetInfo(db, type, 1024);
}


std::string Utility::sqlGetInfo(const ConnectionHandle& db, SQLUSMALLINT type, SQLSMALLINT bufferSize)
{
	if (bufferSize <= 0)
		throw InvalidArgumentException("ODBC::Utility::sqlGetInfo(): buffer size must be positive.");

	std::string ret = "unknown"s;
	// One zero byte more than the driver is told about: the driver manager can
	// fill the buffer without a terminator when it truncates a converted value
	// and then look for the terminator itself, which must stay in the buffer.
	std::vector<SQLCHAR> info(static_cast<std::size_t>(bufferSize) + 1, 0);
	SQLSMALLINT retSize = 0;
	poco_check_ptr (db.handle());
	SQLRETURN rc = Poco::Data::ODBC::SQLGetInfo(const_cast<SQLHDBC>(db.handle()), type, info.data(), bufferSize, &retSize);
	if (!isError(rc))
	{
		// The reported length can differ from the length of the value in the
		// buffer: for a truncated value it is the length of the whole value,
		// and in Unicode builds it is the length in bytes of the UTF-16 value
		// that was converted to UTF-8 into the buffer. The value ends at its
		// terminator, or at the end of the buffer if it fills the buffer.
		const char* pInfo = reinterpret_cast<const char*>(info.data());
		ret.assign(pInfo, std::find(pInfo, pInfo + bufferSize, '\0'));
	}
	return ret;
}


std::string Utility::dbmsName(const ConnectionHandle& db)
{
	return sqlGetInfo(db, SQL_DBMS_NAME);
}


std::string Utility::dbmsVersion(const ConnectionHandle& db)
{
	return sqlGetInfo(db, SQL_DBMS_VER);
}


std::string Utility::driverName(const ConnectionHandle& db)
{
	return sqlGetInfo(db, SQL_DRIVER_NAME);
}


std::string Utility::driverVersion(const ConnectionHandle& db)
{
	return sqlGetInfo(db, SQL_DRIVER_VER);
}


std::string Utility::driverODBCVersion(const ConnectionHandle& db)
{
	return sqlGetInfo(db, SQL_DRIVER_ODBC_VER);
}


} // namespace Poco::Data::ODBC
