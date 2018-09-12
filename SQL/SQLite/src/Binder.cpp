//
// Binder.cpp
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Binder
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/SQLite/Binder.h"
#include "Poco/SQL/SQLite/Utility.h"
#include "Poco/SQL/Date.h"
#include "Poco/SQL/Time.h"
#include "Poco/Exception.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include <cstdlib>


using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;


namespace Poco {
namespace SQL {
namespace SQLite {


Binder::Binder(sqlite3_stmt* pStmt):
	_pStmt(pStmt)
{
}


Binder::~Binder()
{
}


void Binder::bind(std::size_t pos, const Poco::Int32 &val, Direction /*dir*/, const WhenNullCb& /*nullCb*/)
{
	int rc = sqlite3_bind_int(_pStmt, (int) pos, val);
	checkReturn(rc);
}


void Binder::bind(std::size_t pos, const Poco::Int64 &val, Direction /*dir*/, const WhenNullCb& /*nullCb*/)
{
	int rc = sqlite3_bind_int64(_pStmt, (int) pos, val);
	checkReturn(rc);
}


#ifndef POCO_LONG_IS_64_BIT
void Binder::bind(std::size_t pos, const long &val, Direction /*dir*/, const WhenNullCb& /*nullCb*/)
{
	long tmp = static_cast<long>(val);
	int rc = sqlite3_bind_int(_pStmt, (int) pos, tmp);
	checkReturn(rc);
}

void Binder::bind(std::size_t pos, const unsigned long &val, Direction /*dir*/, const WhenNullCb& /*nullCb*/)
{
	long tmp = static_cast<long>(val);
	int rc = sqlite3_bind_int(_pStmt, (int) pos, tmp);
	checkReturn(rc);
}
#endif


void Binder::bind(std::size_t pos, const double &val, Direction /*dir*/, const WhenNullCb& /*nullCb*/)
{
	int rc = sqlite3_bind_double(_pStmt, (int) pos, val);
	checkReturn(rc);
}


void Binder::bind(std::size_t pos, const std::string& val, Direction /*dir*/, const WhenNullCb& /*nullCb*/)
{
	int rc = sqlite3_bind_text(_pStmt, (int) pos, val.c_str(), (int) val.size()*sizeof(char), SQLITE_TRANSIENT);
	checkReturn(rc);
}


void Binder::bind(std::size_t pos, const Date& val, Direction dir, const WhenNullCb& nullCb)
{
	DateTime dt(val.year(), val.month(), val.day());
	std::string str(DateTimeFormatter::format(dt, Utility::SQLITE_DATE_FORMAT));
	bind(pos, str, dir, nullCb);
}


void Binder::bind(std::size_t pos, const Time& val, Direction dir, const WhenNullCb& nullCb)
{
	DateTime dt;
	dt.assign(dt.year(), dt.month(), dt.day(), val.hour(), val.minute(), val.second());
	std::string str(DateTimeFormatter::format(dt, Utility::SQLITE_TIME_FORMAT));
	bind(pos, str, dir, nullCb);
}


void Binder::bind(std::size_t pos, const DateTime& val, Direction dir, const WhenNullCb& nullCb)
{
	std::string dt(DateTimeFormatter::format(val, DateTimeFormat::ISO8601_FORMAT));
	bind(pos, dt, dir, nullCb);
}


void Binder::bind(std::size_t pos, const NullData&, Direction, const std::type_info& /*bindType*/)
{
	sqlite3_bind_null(_pStmt, static_cast<int>(pos));
}


void Binder::checkReturn(int rc)
{
	if (rc != SQLITE_OK)
		Utility::throwException(sqlite3_db_handle(_pStmt), rc);
}


} } } // namespace Poco::SQL::SQLite
