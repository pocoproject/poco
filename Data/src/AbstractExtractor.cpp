//
// AbstractExtractor.cpp
//
// Library: Data
// Package: DataCore
// Module:  AbstractExtractor
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/AbstractExtractor.h"
#include "Poco/Data/Transcoder.h"
#include "Poco/Exception.h"


namespace Poco {
namespace Data {


AbstractExtractor::AbstractExtractor(Poco::TextEncoding::Ptr pDBEncoding,
	Poco::TextEncoding::Ptr pToEncoding):
	_pTranscoder(Transcoder::create(pDBEncoding, pToEncoding))
{
}


AbstractExtractor::~AbstractExtractor()
{
}


void AbstractExtractor::transcode(const std::string& from, std::string& to)
{
	if (_pTranscoder)
		_pTranscoder->transcode(from, to);
}


void AbstractExtractor::reverseTranscode(const std::string& from, std::string& to)
{
	if (_pTranscoder)
		_pTranscoder->reverseTranscode(from, to);
}


bool AbstractExtractor::extract(std::size_t pos, UTF16String& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, std::vector<UTF16String>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, std::deque<UTF16String>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, std::list<UTF16String>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, std::vector<UUID>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, std::deque<UUID>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, std::list<UUID>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Poco::Int8>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt8>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Poco::Int16>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt16>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Poco::Int32>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt32>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Poco::Int64>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt64>& val)
{
	throw NotImplementedException(poco_src_loc);
}


#ifndef POCO_INT64_IS_LONG
bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<long>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<unsigned long>& val)
{
	throw NotImplementedException(poco_src_loc);
}

#endif

bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<bool>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<float>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<double>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<char>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<std::string>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<UTF16String>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<BLOB>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<CLOB>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<DateTime>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Date>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Time>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<UUID>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Any>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool AbstractExtractor::extract(std::size_t pos, Poco::Nullable<Poco::Dynamic::Var>& val)
{
	throw NotImplementedException(poco_src_loc);
}


} } // namespace Poco::Data
