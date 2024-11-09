//
// Extractor.cpp
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Extractor
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/SQLite/Extractor.h"
#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/DataException.h"
#include "Poco/DateTimeParser.h"
#include "Poco/Exception.h"
#include "Poco/Debugger.h"
#if defined(POCO_UNBUNDLED)
#include <sqlite3.h>
#else
#include "sqlite3.h"
#endif
#include <cstdlib>


using Poco::DateTimeParser;


namespace Poco {
namespace Data {
namespace SQLite {


Extractor::Extractor(sqlite3_stmt* pStmt):
	_pStmt(pStmt)
{
}


Extractor::~Extractor()
{
}


bool Extractor::extract(std::size_t pos, Poco::Int32& val)
{
	return extract(sqlite3_column_int, pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Int64& val)
{
	return extract(sqlite3_column_int64, pos, val);
}


bool Extractor::extract(std::size_t pos, double& val)
{
	return extract(sqlite3_column_double, pos, val);
}


bool Extractor::extract(std::size_t pos, std::string& val)
{
	if (isNull(pos)) return false;
	const char *pBuf = reinterpret_cast<const char*>(sqlite3_column_text(_pStmt, (int) pos));
	if (!pBuf)
		val.clear();
	else
		val.assign(pBuf);
	return true;
}


bool Extractor::extract(std::size_t pos, UTF16String& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, Poco::Int8& val)
{
	return extract(sqlite3_column_int, pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt8& val)
{
	return extract(sqlite3_column_int, pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Int16& val)
{
	return extract(sqlite3_column_int, pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt16& val)
{
	return extract(sqlite3_column_int, pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt32& val)
{
	return extract(sqlite3_column_int, pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt64& val)
{
	return extract(sqlite3_column_int64, pos, val);
}


bool Extractor::extract(std::size_t pos, bool& val)
{
	return extract(sqlite3_column_int, pos, val);
}


bool Extractor::extract(std::size_t pos, float& val)
{
	if (isNull(pos)) return false;
	val = static_cast<float>(sqlite3_column_double(_pStmt, (int) pos));
	return true;
}


bool Extractor::extract(std::size_t pos, char& val)
{
	return extract(sqlite3_column_int, pos, val);
}


bool Extractor::extract(std::size_t pos, Date& val)
{
	if (isNull(pos)) return false;
	std::string str;
	extract(pos, str);
	int tzd;
	DateTime dt = DateTimeParser::parse(Utility::SQLITE_DATE_FORMAT, str, tzd);
	val = dt;
	return true;
}


bool Extractor::extract(std::size_t pos, Time& val)
{
	if (isNull(pos)) return false;
	std::string str;
	extract(pos, str);
	int tzd;
	DateTime dt = DateTimeParser::parse(Utility::SQLITE_TIME_FORMAT, str, tzd);
	val = dt;
	return true;
}


bool Extractor::extract(std::size_t pos, DateTime& val)
{
	if (isNull(pos)) return false;
	std::string dt;
	extract(pos, dt);
	int tzd;
	DateTimeParser::parse(dt, val, tzd);
	return true;
}


bool Extractor::extract(std::size_t pos, UUID& val)
{
	if (isNull(pos)) return false;
	std::string str;
	extract(pos, str);
	val.parse(str);
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Any& val)
{
	return extractImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Dynamic::Var& val)
{
	return extractImpl(pos, val);
}


// Nullable
bool Extractor::extract(std::size_t pos, Nullable<Poco::Int32>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<Poco::Int64>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int64(_pStmt, (int) pos);
	return true;
}


#ifndef POCO_INT64_IS_LONG

bool Extractor::extract(std::size_t pos, long& val)
{
	if (isNull(pos)) return false;
	val = sqlite3_column_int64(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, unsigned long& val)
{
	if (isNull(pos)) return false;
	val = sqlite3_column_int64(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<long>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<long>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<long>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<unsigned long>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<unsigned long>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<unsigned long>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, Nullable<long>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<unsigned long>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int(_pStmt, (int) pos);
	return true;
}

#endif // POCO_INT64_IS_LONG


bool Extractor::extract(std::size_t pos, Nullable<double>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_double(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<std::string>& val)
{
	if (isNull(pos)) val.clear();
	else
	{
		const char *pBuf = reinterpret_cast<const char*>(sqlite3_column_text(_pStmt, (int) pos));
		if (!pBuf) val = ""s;
		else val = pBuf;
	}
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<UTF16String>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, Nullable<Poco::Int8>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<Poco::UInt8>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<Poco::Int16>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<Poco::UInt16>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<Poco::UInt32>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<Poco::UInt64>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int64(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<bool>& val)
{
	if (isNull(pos)) val.clear();
	else val = (0 != sqlite3_column_int(_pStmt, (int) pos));
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<float>& val)
{
	if (isNull(pos)) val.clear();
	else val = static_cast<float>(sqlite3_column_double(_pStmt, (int) pos));
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<char>& val)
{
	if (isNull(pos)) val.clear();
	else val = sqlite3_column_int(_pStmt, (int) pos);
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<Date>& val)
{
	if (isNull(pos)) val.clear();
	else
	{
		std::string str;
		extract(pos, str);
		int tzd;
		val = DateTimeParser::parse(Utility::SQLITE_DATE_FORMAT, str, tzd);
	}
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<Time>& val)
{
	if (isNull(pos)) val.clear();
	else
	{
		std::string str;
		extract(pos, str);
		int tzd;
		val = DateTimeParser::parse(Utility::SQLITE_TIME_FORMAT, str, tzd);
	}
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<DateTime>& val)
{
	if (isNull(pos)) val.clear();
	else
	{
		std::string dt;
		extract(pos, dt);
		int tzd;
		if (val.isNull()) val = DateTime();
		DateTimeParser::parse(dt, val.value(), tzd);
	}
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<UUID>& val)
{
	if (isNull(pos)) val.clear();
	else
	{
		std::string str;
		extract(pos, str);
		if (val.isNull()) val = UUID();
		val.value().parse(str);
	}
	return true;
}


bool Extractor::extract(std::size_t pos, Nullable<Poco::Any>& val)
{
	if (isNull(pos))
	{
		val.clear();
		return true;
	}
	if (val.isNull()) val = Poco::Any();
	return extractImpl(pos, val.value());
}


bool Extractor::extract(std::size_t pos, Nullable<Dynamic::Var>& val)
{
	if (isNull(pos))
	{
		val.clear();
		return true;
	}
	if (val.isNull()) val = Dynamic::Var();
	return extractImpl(pos, val.value());
}


inline bool Extractor::extract(std::size_t pos, Nullable<Poco::Data::BLOB>& val)
{
	if (isNull(pos))
	{
		val.clear();
		return true;
	}
	if (val.isNull()) val = Poco::Data::BLOB();
	return extractLOB<Poco::Data::BLOB::ValueType>(pos, val.value());
}


inline bool Extractor::extract(std::size_t pos, Nullable<Poco::Data::CLOB>& val)
{
	if (isNull(pos))
	{
		val.clear();
		return true;
	}
	if (val.isNull()) val = Poco::Data::CLOB();
	return extractLOB<Poco::Data::CLOB::ValueType>(pos, val.value());
}


bool Extractor::isNull(std::size_t pos, std::size_t)
{
	if (pos >= _nulls.size())
		_nulls.resize(pos + 1);

	if (!_nulls[pos].first)
	{
		_nulls[pos].first = true;
		_nulls[pos].second = (SQLITE_NULL == sqlite3_column_type(_pStmt, static_cast<int>(pos)));
	}

	return _nulls[pos].second;
}


// Containers
bool Extractor::extract(std::size_t pos, std::vector<Poco::Int8>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int8>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int8>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt8>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt8>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt8>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int16>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int16>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int16>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt16>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt16>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt16>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int32>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int32>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int32>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt32>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt32>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt32>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int64>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int64>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int64>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt64>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt64>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt64>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<bool>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<bool>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<bool>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<char>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<char>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<char>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<std::string>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<std::string>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<std::string>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<UTF16String>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<UTF16String>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<UTF16String>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<float>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<float>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<float>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<double>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<double>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<double>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::Date>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::Date>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::Date>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::Time>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::Time>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::Time>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::BLOB>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::BLOB>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::BLOB>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::CLOB>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::CLOB>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::CLOB>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::DateTime>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::DateTime>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::DateTime>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UUID>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UUID>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UUID>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Any>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Any>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Any>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Dynamic::Var>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Dynamic::Var>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Dynamic::Var>& val)
{
	throw Poco::NotImplementedException(poco_src_loc);
}


} } } // namespace Poco::Data::SQLite
