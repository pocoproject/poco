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


#include "Poco/SQL/ODBC/Utility.h"
#include "Poco/SQL/ODBC/Handle.h"
#include "Poco/SQL/ODBC/ODBCException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTime.h"
#include <cmath>


namespace Poco {
namespace SQL {
namespace ODBC {


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

	if (!Utility::isError(rc = Poco::SQL::ODBC::SQLDrivers(henv,
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
		}while (!Utility::isError(rc = Poco::SQL::ODBC::SQLDrivers(henv,
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

	while (!Utility::isError(rc = Poco::SQL::ODBC::SQLDataSources(henv,
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
	double msec = ts.fraction/1000000.0;
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
	ds.year = static_cast<SQLSMALLINT>(d.year());
	ds.month = static_cast<SQLUSMALLINT>(d.month());
	ds.day = static_cast<SQLUSMALLINT>(d.day());
}


void Utility::timeSync(SQL_TIME_STRUCT& ts, const Time& t)
{
	ts.hour = static_cast<SQLUSMALLINT>(t.hour());
	ts.minute = static_cast<SQLUSMALLINT>(t.minute());
	ts.second = static_cast<SQLUSMALLINT>(t.second());
}


void Utility::dateTimeSync(SQL_TIMESTAMP_STRUCT& ts, const Poco::DateTime& dt)
{
	ts.year = static_cast<SQLSMALLINT>(dt.year());
	ts.month = static_cast<SQLUSMALLINT>(dt.month());
	ts.day = static_cast<SQLUSMALLINT>(dt.day());
	ts.hour = static_cast<SQLUSMALLINT>(dt.hour());
	ts.minute = static_cast<SQLUSMALLINT>(dt.minute());
	ts.second = static_cast<SQLUSMALLINT>(dt.second());
	// Fraction support is limited to milliseconds due to MS SQL Server limitation
	// see http://support.microsoft.com/kb/263872
	ts.fraction = (dt.millisecond() * 1000000);// + (dt.microsecond() * 1000);
}


} } } // namespace Poco::SQL::ODBC
