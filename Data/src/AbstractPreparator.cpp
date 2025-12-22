//
// AbstractPreparator.cpp
//
// Library: Data
// Package: DataCore
// Module:  AbstractPreparator
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/AbstractPreparator.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/DateTime.h"
#include "Poco/Types.h"
#include "Poco/Any.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Debugger.h"


namespace Poco {
namespace Data {


AbstractPreparator::AbstractPreparator(Poco::UInt32 length):
	_length(length),
	_bulk(false)
{
}


AbstractPreparator::~AbstractPreparator()
{
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Int8>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Int8>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Int8>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::UInt8>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::UInt8>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::UInt8>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Int16>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Int16>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Int16>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::UInt16>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::UInt16>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::UInt16>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Int32>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Int32>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Int32>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::UInt32>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::UInt32>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::UInt32>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Int64>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Int64>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Int64>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::UInt64>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::UInt64>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::UInt64>& val)
{
	throw NotImplementedException(poco_src_loc);
}


#ifndef POCO_INT64_IS_LONG
void AbstractPreparator::prepare(std::size_t pos, const std::vector<long>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<long>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<long>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<unsigned long>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<unsigned long>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<unsigned long>& val)
{
	throw NotImplementedException(poco_src_loc);
}
#endif


void AbstractPreparator::prepare(std::size_t pos, const std::vector<bool>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<bool>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<bool>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<float>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<float>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<float>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<double>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<double>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<double>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<char>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<char>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<char>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<std::string>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<std::string>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<std::string>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const UTF16String& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<UTF16String>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<UTF16String>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<UTF16String>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<BLOB>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<BLOB>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<BLOB>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<CLOB>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<CLOB>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<CLOB>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<DateTime>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<DateTime>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<DateTime>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Date>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Date>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Date>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Time>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Time>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Time>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<UUID>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<UUID>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<UUID>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Any>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Any>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Any>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Dynamic::Var>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Dynamic::Var>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Dynamic::Var>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Poco::Int8>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Poco::Int8>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Poco::Int8>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Poco::Int8>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Poco::UInt8>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Poco::UInt8>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Poco::UInt8>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Poco::UInt8>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Poco::Int16>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Poco::Int16>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Poco::Int16>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Poco::Int16>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Poco::UInt16>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Poco::UInt16>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Poco::UInt16>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Poco::UInt16>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Poco::Int32>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Poco::Int32>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Poco::Int32>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Poco::Int32>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Poco::UInt32>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Poco::UInt32>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Poco::UInt32>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Poco::UInt32>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Poco::Int64>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Poco::Int64>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Poco::Int64>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Poco::Int64>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Poco::UInt64>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Poco::UInt64>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Poco::UInt64>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Poco::UInt64>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


#ifndef POCO_INT64_IS_LONG
void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<long>&)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<unsigned long>&)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<long>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<long>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<long>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<unsigned long>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<unsigned long>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<unsigned long>>& val)
{
	throw NotImplementedException(poco_src_loc);
}

#endif

void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<bool>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<bool>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<bool>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<bool>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<float>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<float>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<float>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<float>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<double>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<double>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<double>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<double>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<char>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<char>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<char>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<char>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<std::string>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<std::string>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<std::string>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<std::string>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<UTF16String>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<UTF16String>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<UTF16String>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<UTF16String>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<BLOB>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<CLOB>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<BLOB>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<BLOB>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<BLOB>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<CLOB>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<CLOB>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<CLOB>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<DateTime>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<DateTime>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<DateTime>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<DateTime>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Date>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Date>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Date>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Date>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Time>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Time>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Time>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Time>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<UUID>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<UUID>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<UUID>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<UUID>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Any>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Any>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Any>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Any>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const Poco::Nullable<Poco::Dynamic::Var>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::vector<Poco::Nullable<Poco::Dynamic::Var>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::deque<Poco::Nullable<Poco::Dynamic::Var>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


void AbstractPreparator::prepare(std::size_t pos, const std::list<Poco::Nullable<Poco::Dynamic::Var>>& val)
{
	throw NotImplementedException(poco_src_loc);
}


} } // namespace Poco::Data
