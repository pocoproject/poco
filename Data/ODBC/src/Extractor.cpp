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


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/Extractor.h"
#include "Poco/Data/ODBC/ODBCMetaColumn.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/LOB.h"
#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include <typeinfo>


namespace Poco {
namespace Data {
namespace ODBC {


const std::string Extractor::FLD_SIZE_EXCEEDED_FMT = "Specified data size (%z bytes) "
	"exceeds maximum value (%z).\n"
	"Use Session.setProperty(\"maxFieldSize\", value) "
	"to increase the maximum allowed data size\n";


Extractor::Extractor(const StatementHandle& rStmt,
	Preparator::Ptr pPreparator,
	TextEncoding::Ptr pDBEncoding,
	Poco::TextEncoding::Ptr pToEncoding): AbstractExtractor(pDBEncoding, pToEncoding),
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
		throw Poco::Data::ExtractException("Unsupported string type: " + std::string(ti.name()));
	}
	std::size_t len = Poco::UnicodeConverter::UTFStrlen(sp);
	if (len < dataSize) dataSize = len;
	checkDataSize(dataSize);
	val.assign(sp, dataSize);

	return true;
}


template<>
bool Extractor::extractBoundImpl<Poco::Data::Date>(std::size_t pos, Poco::Data::Date& val)
{
	if (isNull(pos)) return false;
	SQL_DATE_STRUCT& ds = *AnyCast<SQL_DATE_STRUCT>(&(_pPreparator->at(pos)));
	Utility::dateSync(val, ds);
	return true;
}



template<>
bool Extractor::extractBoundImplContainer<std::vector<Poco::Data::Date> >(std::size_t pos,
	std::vector<Poco::Data::Date>& val)
{
	std::vector<SQL_DATE_STRUCT>& ds = RefAnyCast<std::vector<SQL_DATE_STRUCT> >(_pPreparator->at(pos));
	Utility::dateSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::deque<Poco::Data::Date> >(std::size_t pos,
	std::deque<Poco::Data::Date>& val)
{
	std::vector<SQL_DATE_STRUCT>& ds = RefAnyCast<std::vector<SQL_DATE_STRUCT> >(_pPreparator->at(pos));
	Utility::dateSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::list<Poco::Data::Date> >(std::size_t pos,
	std::list<Poco::Data::Date>& val)
{
	std::vector<SQL_DATE_STRUCT>& ds = RefAnyCast<std::vector<SQL_DATE_STRUCT> >(_pPreparator->at(pos));
	Utility::dateSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImpl<Poco::Data::Time>(std::size_t pos, Poco::Data::Time& val)
{
	if (isNull(pos)) return false;

	std::size_t dataSize = _pPreparator->actualDataSize(pos);
	checkDataSize(dataSize);
	SQL_TIME_STRUCT& ts = *AnyCast<SQL_TIME_STRUCT>(&_pPreparator->at(pos));
	Utility::timeSync(val, ts);

	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::vector<Poco::Data::Time> >(std::size_t pos,
	std::vector<Poco::Data::Time>& val)
{
	std::vector<SQL_TIME_STRUCT>& ds = RefAnyCast<std::vector<SQL_TIME_STRUCT> >(_pPreparator->at(pos));
	Utility::timeSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::deque<Poco::Data::Time> >(std::size_t pos,
	std::deque<Poco::Data::Time>& val)
{
	std::vector<SQL_TIME_STRUCT>& ds = RefAnyCast<std::vector<SQL_TIME_STRUCT> >(_pPreparator->at(pos));
	Utility::timeSync(val, ds);
	return true;
}


template<>
bool Extractor::extractBoundImplContainer<std::list<Poco::Data::Time> >(std::size_t pos,
	std::list<Poco::Data::Time>& val)
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
bool Extractor::extractBoundImpl<Poco::UUID>(std::size_t pos, Poco::UUID& val)
{
	if (isNull(pos)) return false;

	std::size_t dataSize = _pPreparator->actualDataSize(pos);
	checkDataSize(dataSize);
	char* pBuffer = *AnyCast<char*>(&_pPreparator->at(pos));
	val.copyFrom(pBuffer);

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


template<>
bool Extractor::extractManualImpl<std::string>(std::size_t pos, std::string& val, SQLSMALLINT cType)
{
	std::size_t maxSize = _pPreparator->getMaxFieldSize();

	SQLLEN len;
	Poco::Buffer<char> apChar(CHUNK_SIZE);
	char* pChar = apChar.begin();
	SQLRETURN rc = 0;

	val.clear();
	resizeLengths(pos);

	do
	{
		std::memset(pChar, 0, CHUNK_SIZE);
		len = 0;
		rc = SQLGetData(_rStmt,
			(SQLUSMALLINT) pos + 1,
			cType, //C data type
			pChar, //returned value
			CHUNK_SIZE, //buffer length
			&len); //length indicator

		if (SQL_SUCCESS_WITH_INFO == rc)
		{
			StatementDiagnostics d(_rStmt);
			std::size_t fieldCount = d.fields().size();
			for (int i = 0; i < fieldCount; ++i)
			{
				if (d.sqlState(i) == "01004"s)
				{
					if (len == SQL_NO_TOTAL || len > CHUNK_SIZE) // only part of data was returned
						len = CHUNK_SIZE-1; // SQLGetData terminates the returned string
				}
			}
		}

		if (SQL_NO_DATA != rc && Utility::isError(rc))
			throw StatementException(_rStmt, "ODBC::Extractor::extractManualImpl(string):SQLGetData()");

		if (SQL_NO_TOTAL == len)//unknown length, throw
			throw UnknownDataLengthException("ODBC::Extractor::extractManualImpl(string):Could not determine returned data length.");

		if (isNullLengthIndicator(len))
		{
			_lengths[pos] = len;
			return false;
		}

		if (SQL_NO_DATA == rc || !len)
			break;

		_lengths[pos] += len;
		if (_lengths[pos] <= maxSize)
			val.append(pChar, len);
		else
			throw DataException(format(FLD_SIZE_EXCEEDED_FMT, static_cast<std::size_t>(_lengths[pos]), maxSize));

	}while (true);

	return true;
}


template<>
bool Extractor::extractManualImpl<UTF16String>(std::size_t pos, UTF16String& val, SQLSMALLINT cType)
{
	std::size_t maxSize = _pPreparator->getMaxFieldSize();
	SQLLEN len;
	Poco::Buffer<UTF16String::value_type> apChar(CHUNK_SIZE);
	UTF16String::value_type* pChar = apChar.begin();
	SQLRETURN rc = 0;

	val.clear();
	resizeLengths(pos);

	do
	{
		std::memset(pChar, 0, CHUNK_SIZE);
		len = 0;
		rc = SQLGetData(_rStmt,
			(SQLUSMALLINT)pos + 1,
			cType, //C data type
			pChar, //returned value
			CHUNK_SIZE, //buffer length
			&len); //length indicator

		if (SQL_SUCCESS_WITH_INFO == rc)
		{
			StatementDiagnostics d(_rStmt);
			std::size_t fieldCount = d.fields().size();
			for (int i = 0; i < fieldCount; ++i)
			{
				if (d.sqlState(i) == "01004"s)
				{
					if (len == SQL_NO_TOTAL || len > CHUNK_SIZE) // only part of data was returned
						len = CHUNK_SIZE - 2;
				}
			}
		}

		if (SQL_NO_DATA != rc && Utility::isError(rc))
			throw StatementException(_rStmt, "ODBC::Extractor::extractManualImpl(UTF16String):SQLGetData()");

		if (SQL_NO_TOTAL == len)//unknown length, throw
			throw UnknownDataLengthException("ODBC::Extractor::extractManualImpl(UTF16String):Could not determine returned data length.");

		if (isNullLengthIndicator(len))
		{
			_lengths[pos] = len;
			return false;
		}

		if (SQL_NO_DATA == rc || !len)
			break;

		_lengths[pos] += len;
		if (_lengths[pos] <= maxSize)
			val.append(pChar, len / sizeof(UTF16Char));
		else
			throw DataException(format(FLD_SIZE_EXCEEDED_FMT, static_cast<std::size_t>(_lengths[pos]), maxSize));
	} while (true);

	return true;
}


template<>
bool Extractor::extractManualImpl<Poco::Data::CLOB>(std::size_t pos,
	Poco::Data::CLOB& val,
	SQLSMALLINT cType)
{
	std::size_t maxSize = _pPreparator->getMaxFieldSize();
	std::size_t fetchedSize = 0;

	SQLLEN len;
	const int bufSize = CHUNK_SIZE;
	Poco::Buffer<char> apChar(bufSize);
	char* pChar = apChar.begin();
	SQLRETURN rc = 0;

	val.clear();
	resizeLengths(pos);

	do
	{
		std::memset(pChar, 0, bufSize);
		len = 0;
		rc = SQLGetData(_rStmt,
			(SQLUSMALLINT)pos + 1,
			cType, //C data type
			pChar, //returned value
			bufSize, //buffer length
			&len); //length indicator

		if (SQL_NO_DATA != rc && Utility::isError(rc))
			throw StatementException(_rStmt, "ODBC::Extractor::extractManualImpl(CLOB):SQLGetData()");

		if (SQL_NO_TOTAL == len)//unknown length, throw
			throw UnknownDataLengthException("ODBC::Extractor::extractManualImpl(CLOB):Could not determine returned data length.");

		if (isNullLengthIndicator(len))
			return false;

		if (SQL_NO_DATA == rc || !len)
			break;

		fetchedSize = len > CHUNK_SIZE ? CHUNK_SIZE : len;
		_lengths[pos] += fetchedSize;
		if (_lengths[pos] <= maxSize)
			val.appendRaw(pChar, fetchedSize);
		else
			throw DataException(format(FLD_SIZE_EXCEEDED_FMT, fetchedSize, maxSize));

	} while (true);

	return true;
}


template<>
bool Extractor::extractManualImpl<Poco::Data::Date>(std::size_t pos,
	Poco::Data::Date& val,
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
		throw StatementException(_rStmt, "ODBC::Extractor::extractManualImpl(Date):SQLGetData()");

	if (isNullLengthIndicator(_lengths[pos]))
		return false;
	else
		Utility::dateSync(val, ds);

	return true;
}


template<>
bool Extractor::extractManualImpl<Poco::Data::Time>(std::size_t pos,
	Poco::Data::Time& val,
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
		throw StatementException(_rStmt, "ODBC::Extractor::extractManualImpl(Time):SQLGetData()");

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
		throw StatementException(_rStmt, "ODBC::Extractor::extractManualImpl(DateTime):SQLGetData()");

	if (isNullLengthIndicator(_lengths[pos]))
		return false;
	else
		Utility::dateTimeSync(val, ts);

	return true;
}


template<>
bool Extractor::extractManualImpl<Poco::UUID>(std::size_t pos,
	Poco::UUID& val,
	SQLSMALLINT cType)
{
	char buffer[16];
	resizeLengths(pos);

	SQLRETURN rc = SQLGetData(_rStmt,
		(SQLUSMALLINT) pos + 1,
		cType, //C data type
		&buffer, //returned value
		sizeof(buffer), //buffer length
		&_lengths[pos]); //length indicator

	if (Utility::isError(rc))
		throw StatementException(_rStmt, "SQLGetData()");

	if (isNullLengthIndicator(_lengths[pos]))
		return false;
	else
		val.copyFrom(buffer);

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


#ifndef POCO_INT64_IS_LONG
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


bool Extractor::extract(std::size_t pos, std::vector<unsigned long>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<unsigned long>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<unsigned long>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}
#endif // POCO_INT64_IS_LONG


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
	bool ret = false;

	if (!transcodeRequired())
	{
		if (Preparator::DE_MANUAL == _dataExtraction)
			ret = extractManualImpl(pos, val, SQL_C_CHAR);
		else
			ret = extractBoundImpl(pos, val);
	}
	else
	{
		std::string result;
		if (Preparator::DE_MANUAL == _dataExtraction)
			ret = extractManualImpl(pos, result, SQL_C_CHAR);
		else
			ret = extractBoundImpl(pos, result);
		transcode(result, val);

	}

	return ret;
}


bool Extractor::extract(std::size_t pos, std::vector<std::string>& val)
{
	return stringContainerExtract(pos, val);
}


bool Extractor::extract(std::size_t pos, std::deque<std::string>& val)
{
	return stringContainerExtract(pos, val);
}


bool Extractor::extract(std::size_t pos, std::list<std::string>& val)
{
	return stringContainerExtract(pos, val);
}


bool Extractor::extract(std::size_t pos, UTF16String& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
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


bool Extractor::extract(std::size_t pos, Poco::Data::BLOB& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_BINARY);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Data::CLOB& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_BINARY);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::BLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::BLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::BLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::CLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::CLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::CLOB>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::Data::Date& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_TYPE_DATE);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::Date>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::Date>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::Date>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::Data::Time& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_TYPE_TIME);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Data::Time>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Data::Time>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImplContainer(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Data::Time>& val)
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


bool Extractor::extract(std::size_t pos, Poco::UUID& val)
{
	if (Preparator::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_BINARY);
	else
		return extractBoundImpl(pos, val);
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


bool Extractor::extract(std::size_t pos, Poco::Dynamic::Var& val)
{
	return extractImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::vector<Poco::Dynamic::Var>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImpl(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::deque<Poco::Dynamic::Var>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImpl(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, std::list<Poco::Dynamic::Var>& val)
{
	if (Preparator::DE_BOUND == _dataExtraction)
		return extractBoundImpl(pos, val);
	else
		throw InvalidAccessException("Direct container extraction only allowed for bound mode.");
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Int8>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt8>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Int16>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt16>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Int32>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt32>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Int64>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt64>& val)
{
	return extractNullable(pos, val);
}


#ifndef POCO_INT64_IS_LONG
bool Extractor::extract(std::size_t pos, Poco::Nullable<long>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<unsigned long>& val)
{
	return extractNullable(pos, val);
}
#endif

bool Extractor::extract(std::size_t pos, Poco::Nullable<bool>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<float>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<double>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<char>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<std::string>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<UTF16String>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<BLOB>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<CLOB>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<DateTime>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Date>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Time>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<UUID>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Any>& val)
{
	return extractNullable(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Nullable<Poco::Dynamic::Var>& val)
{
	return extractNullable(pos, val);
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
	else
		return SQL_NULL_DATA == _pPreparator->actualDataSize(col, row);
}


void Extractor::checkDataSize(std::size_t size)
{
	std::size_t maxSize = _pPreparator->getMaxFieldSize();
	if (size > maxSize)
		throw DataException(format(FLD_SIZE_EXCEEDED_FMT, size, maxSize));
}


} } } // namespace Poco::Data::ODBC
