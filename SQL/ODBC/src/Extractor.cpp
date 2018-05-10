//
// Extractor.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Extractor
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/ODBC/Extractor.h"
#include "Poco/SQL/ODBC/ODBCMetaColumn.h"
#include "Poco/SQL/ODBC/Utility.h"
#include "Poco/SQL/ODBC/ODBCException.h"
#include "Poco/SQL/LOB.h"
#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include <typeinfo>

namespace Poco {
namespace SQL {
namespace ODBC {


const std::string Extractor::FLD_SIZE_EXCEEDED_FMT = "Specified data size (%z bytes) "
	"exceeds maximum value (%z).\n"
	"Use Session.setProperty(\"maxFieldSize\", value) "
	"to increase the maximum allowed data size\n";


Extractor::Extractor(const StatementHandle& rStmt,
	Preparator::Ptr pPreparator):
	_rStmt(rStmt),
	_pPreparator(pPreparator),
	_dataExtraction(pPreparator->getDataExtraction())
{
}


Extractor::~Extractor()
{
}


template<>
bool Extractor::extractBoundImpl<std::string>(std::size_t pos, std::string& val)
{
	if (isNull(pos)) return false;

	std::size_t dataSize = _pPreparator->actualDataSize(pos);
	char* sp = AnyCast<char*>(_pPreparator->at(pos));
	std::size_t len = std::strlen(sp);
	if (len < dataSize) dataSize = len;
	checkDataSize(dataSize);
	val.assign(sp, dataSize);

	return true;
}


template<>
bool Extractor::extractBoundImpl<UTF16String>(std::size_t pos, UTF16String& val)
{
	typedef UTF16String::value_type CharT;
	if (isNull(pos)) return false;
	std::size_t dataSize = _pPreparator->actualDataSize(pos);
	CharT* sp = 0;
	UTF16String us;
	const std::type_info& ti = _pPreparator->at(pos).type();
	if (ti == typeid(CharT*))
	{
		sp = AnyCast<CharT*>(_pPreparator->at(pos));
	}
	else if (ti == typeid(char*))
	{
		std::string s(AnyCast<char*>(_pPreparator->at(pos)));
		Poco::UnicodeConverter::convert(s, us);
		sp = const_cast<CharT*>(us.c_str());
	}
	else
	{
		throw Poco::SQL::ExtractException("Unsupported string type: " + std::string(ti.name()));
	}
	std::size_t len = Poco::UnicodeConverter::UTFStrlen(sp);
	if (len < dataSize) dataSize = len;
	checkDataSize(dataSize);
	val.assign(sp, dataSize);

	return true;
}


template<>
bool Extractor::extractBoundImpl<Poco::SQL::Date>(std::size_t pos, Poco::SQL::Date& val)
{
	if (isNull(pos)) return false;
	SQL_DATE_STRUCT& ds = *AnyCast<SQL_DATE_STRUCT>(&(_pPreparator->at(pos)));
	Utility::dateSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::vector<Poco::SQL::Date> >(std::size_t pos,
	std::vector<Poco::SQL::Date>& val)
{
	std::vector<SQL_DATE_STRUCT>& ds = RefAnyCast<std::vector<SQL_DATE_STRUCT> >(_pPreparator->at(pos));
	Utility::dateSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::deque<Poco::SQL::Date> >(std::size_t pos,
	std::deque<Poco::SQL::Date>& val)
{
	std::vector<SQL_DATE_STRUCT>& ds = RefAnyCast<std::vector<SQL_DATE_STRUCT> >(_pPreparator->at(pos));
	Utility::dateSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::list<Poco::SQL::Date> >(std::size_t pos,
	std::list<Poco::SQL::Date>& val)
{
	std::vector<SQL_DATE_STRUCT>& ds = RefAnyCast<std::vector<SQL_DATE_STRUCT> >(_pPreparator->at(pos));
	Utility::dateSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImpl<Poco::SQL::Time>(std::size_t pos, Poco::SQL::Time& val)
{
	if (isNull(pos)) return false;

	std::size_t dataSize = _pPreparator->actualDataSize(pos);
	checkDataSize(dataSize);
	SQL_TIME_STRUCT& ts = *AnyCast<SQL_TIME_STRUCT>(&_pPreparator->at(pos));
	Utility::timeSync(val, ts);

	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::vector<Poco::SQL::Time> >(std::size_t pos,
	std::vector<Poco::SQL::Time>& val)
{
	std::vector<SQL_TIME_STRUCT>& ds = RefAnyCast<std::vector<SQL_TIME_STRUCT> >(_pPreparator->at(pos));
	Utility::timeSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::deque<Poco::SQL::Time> >(std::size_t pos,
	std::deque<Poco::SQL::Time>& val)
{
	std::vector<SQL_TIME_STRUCT>& ds = RefAnyCast<std::vector<SQL_TIME_STRUCT> >(_pPreparator->at(pos));
	Utility::timeSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::list<Poco::SQL::Time> >(std::size_t pos,
	std::list<Poco::SQL::Time>& val)
{
	std::vector<SQL_TIME_STRUCT>& ds = RefAnyCast<std::vector<SQL_TIME_STRUCT> >(_pPreparator->at(pos));
	Utility::timeSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImpl<Poco::DateTime>(std::size_t pos, Poco::DateTime& val)
{
	if (isNull(pos)) return false;

	std::size_t dataSize = _pPreparator->actualDataSize(pos);
	checkDataSize(dataSize);
	SQL_TIMESTAMP_STRUCT& tss = *AnyCast<SQL_TIMESTAMP_STRUCT>(&_pPreparator->at(pos));
	Utility::dateTimeSync(val, tss);

	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::vector<Poco::DateTime> >(std::size_t pos,
	std::vector<Poco::DateTime>& val)
{
	std::vector<SQL_TIMESTAMP_STRUCT>& ds = RefAnyCast<std::vector<SQL_TIMESTAMP_STRUCT> >(_pPreparator->at(pos));
	Utility::dateTimeSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::deque<Poco::DateTime> >(std::size_t pos,
	std::deque<Poco::DateTime>& val)
{
	std::vector<SQL_TIMESTAMP_STRUCT>& ds = RefAnyCast<std::vector<SQL_TIMESTAMP_STRUCT> >(_pPreparator->at(pos));
	Utility::dateTimeSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::list<Poco::DateTime> >(std::size_t pos,
	std::list<Poco::DateTime>& val)
{
	std::vector<SQL_TIMESTAMP_STRUCT>& ds = RefAnyCast<std::vector<SQL_TIMESTAMP_STRUCT> >(_pPreparator->at(pos));
	Utility::dateTimeSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::vector<bool> >(std::size_t pos,
	std::vector<bool>& val)
{
	std::size_t length = _pPreparator->getLength();
	bool** p = AnyCast<bool*>(&_pPreparator->at(pos));
	val.assign(*p, *p + length);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::deque<bool> >(std::size_t pos,
	std::deque<bool>& val)
{
	std::size_t length = _pPreparator->getLength();
	bool** p = AnyCast<bool*>(&_pPreparator->at(pos));
	val.assign(*p, *p + length);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::list<bool> >(std::size_t pos,
	std::list<bool>& val)
{
	std::size_t length = _pPreparator->getLength();
	bool** p = AnyCast<bool*>(&_pPreparator->at(pos));
	val.assign(*p, *p + length);
	return true;
}


template<typename T>
bool Extractor::extractManualStringImpl(std::size_t pos, T& val, SQLSMALLINT cType)
{
	typedef typename T::value_type CharType;
	const std::size_t bytesPerChar = sizeof(CharType);
	const std::size_t charsPerChunk = CHUNK_SIZE;
	const std::size_t bytesPerChunk = charsPerChunk * bytesPerChar;
	SQLLEN totalBytes;
	SQLRETURN rc;
	CharType tmpBuf[charsPerChunk + 1];  // space for NUL terminator
	
	val.clear();
	resizeLengths(pos);

	rc = SQLGetData(_rStmt,
		(SQLUSMALLINT)pos + 1,
		cType,                         // C data type
		tmpBuf,                        // returned value
		bytesPerChunk + bytesPerChar,  // buffer length in bytes, including NUL terminator
		&totalBytes);                  // size in bytes of this field, not including NULL terminator

	if (SQL_NO_DATA != rc && Utility::isError(rc))
		throw StatementException(_rStmt, "SQLGetData()");

	if (SQL_NO_TOTAL == totalBytes)  // unknown length, throw
		throw UnknownDataLengthException("Could not determine returned data length.");

	if (isNullLengthIndicator(totalBytes))
	{
		_lengths[pos] = totalBytes;
		return false;
	}

	if (SQL_NO_DATA == rc || !totalBytes)
		return true;

	_lengths[pos] = totalBytes;
	const std::size_t totalChars = totalBytes / bytesPerChar;

	if (totalBytes <= bytesPerChunk)
	{
		// There is no additional data; we've already got it all
		val.append(tmpBuf, totalChars);
	}
	else
	{
		// Reserve space in result string, append what we already have, resize and insert the remaining
		// data in-place.
		const std::size_t bytesRemaining = totalBytes - bytesPerChunk;
		SQLLEN fetchedBytes = 0;
		val.reserve(totalChars);
		val.append(tmpBuf, charsPerChunk);
		val.resize(totalChars);
		rc = SQLGetData(_rStmt,
			(SQLUSMALLINT)pos + 1,
			cType,                          // C data type
			&val[charsPerChunk],            // buffer to write to
			bytesRemaining + bytesPerChar,  // buffer length in bytes, including NUL terminator
			&fetchedBytes);                 // number of bytes remaining, not including NULL terminator

		if (SQL_NO_DATA != rc && Utility::isError(rc))
			throw StatementException(_rStmt, "SQLGetData()");

		if (SQL_NO_TOTAL == fetchedBytes)  // unknown length, throw
			throw UnknownDataLengthException("Could not determine returned data length.");

		if (bytesRemaining != fetchedBytes)  // unexpected number of bytes
			throw UnknownDataLengthException("Unexpected number of bytes returned from second call to SQLGetData().");
	}
	return true;
}

template<>
bool Extractor::extractManualImpl<std::string>(std::size_t pos, std::string& val, SQLSMALLINT cType)
{
	return extractManualStringImpl(pos, val, cType);
}


template<>
bool Extractor::extractManualImpl<UTF16String>(std::size_t pos, UTF16String& val, SQLSMALLINT cType)
{
	return extractManualStringImpl(pos, val, cType);
}


template<>
bool Extractor::extractManualImpl<Poco::SQL::CLOB>(std::size_t pos,
	Poco::SQL::CLOB& val,
	SQLSMALLINT cType)
{
	return extractManualLOBImpl(pos, val, cType);
}


template<>
bool Extractor::extractManualImpl<Poco::SQL::BLOB>(std::size_t pos,
	Poco::SQL::BLOB& val,
	SQLSMALLINT cType)
{
	return extractManualLOBImpl(pos, val, cType);
}


template<typename T>
bool Extractor::extractManualLOBImpl(std::size_t pos,
	Poco::SQL::LOB<T>& val,
	SQLSMALLINT cType)
{
	const int bufSize = CHUNK_SIZE;
	std::size_t fetchedSize = bufSize;
	std::size_t totalSize = 0;

	SQLLEN len;
	
	Poco::Buffer<T> apChar(bufSize);
	T* pChar = apChar.begin();
	SQLRETURN rc = 0;
	
	val.clear();
	resizeLengths(pos);

	do
	{
		// clear out the latest data in the buffer
		if (fetchedSize > 0)
			std::memset(pChar, 0, fetchedSize);
		len = 0;
		rc = SQLGetData(_rStmt,
			(SQLUSMALLINT) pos + 1,
			cType, //C data type
			pChar, //returned value
			bufSize, //buffer length
			&len); //length indicator
		
		_lengths[pos] += len;

		if (SQL_NO_DATA != rc && Utility::isError(rc))
			throw StatementException(_rStmt, "SQLGetData()");

		if (SQL_NO_TOTAL == len)//unknown length, throw
			throw UnknownDataLengthException("Could not determine returned data length.");

		if (isNullLengthIndicator(len))
			return false;

		if (SQL_NO_DATA == rc || !len)
			break;

		fetchedSize = len > bufSize ? bufSize : len;
		totalSize += fetchedSize;
		val.appendRaw(pChar, fetchedSize);
	}while (true);

	return true;
}


template<>
bool Extractor::extractManualImpl<Poco::SQL::Date>(std::size_t pos,
	Poco::SQL::Date& val,
	SQLSMALLINT cType)
{
	SQL_DATE_STRUCT ds;
	resizeLengths(pos);

	SQLRETURN rc = SQLGetData(_rStmt,
		(SQLUSMALLINT) pos + 1,
		cType, //C data type
		&ds, //returned value
		sizeof(ds), //buffer length
		&_lengths[pos]); //length indicator
	
	if (Utility::isError(rc))
		throw StatementException(_rStmt, "SQLGetData()");

	if (isNullLengthIndicator(_lengths[pos]))
		return false;
	else
		Utility::dateSync(val, ds);

	return true;
}


template<>
bool Extractor::extractManualImpl<Poco::SQL::Time>(std::size_t pos,
	Poco::SQL::Time& val,
	SQLSMALLINT cType)
{
	SQL_TIME_STRUCT ts;
	resizeLengths(pos);

	SQLRETURN rc = SQLGetData(_rStmt,
		(SQLUSMALLINT) pos + 1,
		cType, //C data type
		&ts, //returned value
		sizeof(ts), //buffer length
		&_lengths[pos]); //length indicator
	
	if (Utility::isError(rc))
		throw StatementException(_rStmt, "SQLGetData()");

	if (isNullLengthIndicator(_lengths[pos]))
		return false;
	else
		Utility::timeSync(val, ts);

	return true;
}


template<>
bool Extractor::extractManualImpl<Poco::DateTime>(std::size_t pos,
	Poco::DateTime& val,
	SQLSMALLINT cType)
{
	SQL_TIMESTAMP_STRUCT ts;
	resizeLengths(pos);

	SQLRETURN rc = SQLGetData(_rStmt,
		(SQLUSMALLINT) pos + 1,
		cType, //C data type
		&ts, //returned value
		sizeof(ts), //buffer length
		&_lengths[pos]); //length indicator
	
	if (Utility::isError(rc))
		throw StatementException(_rStmt, "SQLGetData()");

	if (isNullLengthIndicator(_lengths[pos]))
		return false;
	else
		Utility::dateTimeSync(val, ts);

	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Int32& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_SLONG);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int32>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int32>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int32>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::Int64& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_SBIGINT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int64>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int64>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int64>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


#ifndef POCO_LONG_IS_64_BIT
bool Extractor::extract(std::size_t pos, long& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_SLONG);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, unsigned long& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_SLONG);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<long>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<long>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<long>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}
#endif


bool Extractor::extract(std::size_t pos, double& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_DOUBLE);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<double>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<double>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<double>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::string& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction || _pPreparator->isPotentiallyHuge(pos))
		return extractManualImpl(pos, val, SQL_C_CHAR);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<std::string>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<std::string>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<std::string>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, UTF16String& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction || _pPreparator->isPotentiallyHuge(pos))
		return extractManualImpl(pos, val, SQL_C_WCHAR);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<UTF16String>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<UTF16String>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<UTF16String>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::SQL::BLOB& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction || _pPreparator->isPotentiallyHuge(pos))
		return extractManualImpl(pos, val, SQL_C_BINARY);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::SQL::CLOB& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction || _pPreparator->isPotentiallyHuge(pos))
		return extractManualImpl(pos, val, SQL_C_BINARY);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::SQL::BLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::SQL::BLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::SQL::BLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::SQL::CLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::SQL::CLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::SQL::CLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::SQL::Date& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_TYPE_DATE);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::SQL::Date>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::SQL::Date>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::SQL::Date>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::SQL::Time& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_TYPE_TIME);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::SQL::Time>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::SQL::Time>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::SQL::Time>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::DateTime& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_TYPE_TIMESTAMP);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::DateTime>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::DateTime>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::DateTime>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::Int8& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_STINYINT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int8>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int8>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int8>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::UInt8& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_UTINYINT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt8>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt8>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt8>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::Int16& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_SSHORT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Int16>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Int16>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Int16>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::UInt16& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_USHORT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt16>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt16>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt16>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::UInt32& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_ULONG);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt32>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt32>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt32>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::UInt64& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_SBIGINT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::UInt64>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::UInt64>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::UInt64>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, bool& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_BIT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<bool>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<bool>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<bool>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, float& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_FLOAT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<float>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<float>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<float>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, char& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_STINYINT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<char>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<char>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<char>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::Any& val)
{
	return extractImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Any>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImpl(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Any>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImpl(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Any>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImpl(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::DynamicAny& val)
{
	return extractImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::DynamicAny>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImpl(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::DynamicAny>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImpl(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::DynamicAny>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImpl(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::isNull(std::size_t col, std::size_t row)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
	{
		try
		{
			return isNullLengthIndicator(_lengths.at(col));
		}
		catch (std::out_of_range& ex)
		{
			throw RangeException(ex.what());
		}
	}
	else return SQL_NULL_DATA == _pPreparator->actualDataSize(col, row);
}


void Extractor::checkDataSize(std::size_t size)
{
	std::size_t maxSize = _pPreparator->getMaxFieldSize();
	if (size > maxSize)
		throw SQLException(format(FLD_SIZE_EXCEEDED_FMT, size, maxSize));
}


} } } // namespace Poco::SQL::ODBC
