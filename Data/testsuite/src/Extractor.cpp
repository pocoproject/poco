//
// Extractor.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Extractor.h"
#include "Poco/Data/LOB.h"
#include "Poco/Exception.h"


namespace Poco {
namespace Data {
namespace Test {


Extractor::Extractor(Poco::TextEncoding::Ptr pDBEncoding):
	AbstractExtractor(pDBEncoding)
{
}


Extractor::~Extractor()
{
}


// Int8

bool Extractor::extract(std::size_t pos, Poco::Int8& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int8>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int8>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int8>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Int8>& val)
{
	return true;
}


// UInt8

bool Extractor::extract(std::size_t pos, Poco::UInt8& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt8>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt8>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt8>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt8>& val)
{
	return true;
}


// Int16

bool Extractor::extract(std::size_t pos, Poco::Int16& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int16>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int16>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int16>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Int16>& val)
{
	return true;
}


// UInt16

bool Extractor::extract(std::size_t pos, Poco::UInt16& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt16>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt16>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt16>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt16>& val)
{
	return true;
}


// Int32

bool Extractor::extract(std::size_t pos, Poco::Int32& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int32>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int32>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int32>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Int32>& val)
{
	val = 1;
	return true;
}


// UInt32

bool Extractor::extract(std::size_t pos, Poco::UInt32& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt32>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt32>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt32>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt32>& val)
{
	return true;
}


// Int64

bool Extractor::extract(std::size_t pos, Poco::Int64& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int64>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int64>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int64>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Int64>& val)
{
	return true;
}


#ifndef POCO_INT64_IS_LONG
// long

bool Extractor::extract(std::size_t pos, long& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<long>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<long>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<long>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<long>& val)
{
	return true;
}


// unsigned long

bool Extractor::extract(std::size_t pos, unsigned long& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<unsigned long>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<unsigned long>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<unsigned long>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<unsigned long>& val)
{
	return true;
}
#endif


// UInt64

bool Extractor::extract(std::size_t pos, Poco::UInt64& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt64>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt64>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt64>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt64>& val)
{
	return true;
}


// bool

bool Extractor::extract(std::size_t pos, bool& val)
{
	val = false;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<bool>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<bool>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<bool>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<bool>& val)
{
	return true;
}


// float

bool Extractor::extract(std::size_t pos, float& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<float>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<float>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<float>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<float>& val)
{
	return true;
}


// double

bool Extractor::extract(std::size_t pos, double& val)
{
	val = 0;
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<double>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<double>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<double>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<double>& val)
{
	return true;
}


// char

bool Extractor::extract(std::size_t pos, char& val)
{
	val = '\0';
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<char>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<char>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<char>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<char>& val)
{
	return true;
}


// std::string

bool Extractor::extract(std::size_t pos, std::string& val)
{
	if (!transcodeRequired())
		val = _stringValue;
	else
		transcode(_stringValue, val);
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<std::string>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<std::string>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<std::string>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<std::string>& val)
{
	return true;
}


// UTF16String

bool Extractor::extract(std::size_t pos, Poco::UTF16String& val)
{
	std::string str("");
	Poco::UnicodeConverter::convert(str, val);
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UTF16String>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UTF16String>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UTF16String>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::UTF16String>& val)
{
	return true;
}


// BLOB

bool Extractor::extract(std::size_t pos, Poco::Data::BLOB& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::BLOB>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::BLOB>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::BLOB>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Data::BLOB>& val)
{
	return true;
}


// CLOB

bool Extractor::extract(std::size_t pos, Poco::Data::CLOB& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::CLOB>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::CLOB>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::CLOB>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Data::CLOB>& val)
{
	return true;
}


// Date

bool Extractor::extract(std::size_t pos, Poco::Data::Date& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::Date>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::Date>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::Date>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Data::Date>& val)
{
	return true;
}


// Time

bool Extractor::extract(std::size_t pos, Poco::Data::Time& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::Time>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::Time>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::Time>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Data::Time>& val)
{
	return true;
}


// DateTime

bool Extractor::extract(std::size_t pos, Poco::DateTime& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::DateTime>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::DateTime>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::DateTime>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::DateTime>& val)
{
	return true;
}


// UUID

bool Extractor::extract(std::size_t pos, Poco::UUID& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UUID>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UUID>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UUID>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::UUID>& val)
{
	return true;
}


// Any

bool Extractor::extract(std::size_t pos, Poco::Any& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Any>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Any>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Any>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Any>& val)
{
	return true;
}


// Var

bool Extractor::extract(std::size_t pos, Poco::Dynamic::Var& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Dynamic::Var>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Dynamic::Var>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Dynamic::Var>& val)
{
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Dynamic::Var>& val)
{
	return true;
}


} } } // namespace Poco::Data::Test
