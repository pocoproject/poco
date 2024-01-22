//
// Binder.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Binder
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/ODBC/Binder.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/DateTime.h"
#include "Poco/Exception.h"
#include <sql.h>


namespace Poco {
namespace Data {
namespace ODBC {


Binder::Binder(const StatementHandle& rStmt,
	std::size_t maxFieldSize,
	Binder::ParameterBinding dataBinding,
	const TypeInfo* pDataTypes,
	Poco::TextEncoding::Ptr pFromEncoding,
	Poco::TextEncoding::Ptr pDBEncoding):
	Poco::Data::AbstractBinder(pFromEncoding, pDBEncoding),
	_rStmt(rStmt),
	_paramBinding(dataBinding),
	_pTypeInfo(pDataTypes),
	_paramSetSize(0),
	_maxFieldSize(maxFieldSize)
{
}


Binder::~Binder()
{
	freeMemory();
}


void Binder::freeMemory()
{
	LengthPtrVec::iterator itLen = _lengthIndicator.begin();
	LengthPtrVec::iterator itLenEnd = _lengthIndicator.end();
	for(; itLen != itLenEnd; ++itLen) delete *itLen;

	LengthVecVec::iterator itVecLen = _vecLengthIndicator.begin();
	LengthVecVec::iterator itVecLenEnd = _vecLengthIndicator.end();
	for (; itVecLen != itVecLenEnd; ++itVecLen) delete *itVecLen;

	TimeMap::iterator itT = _times.begin();
	TimeMap::iterator itTEnd = _times.end();
	for(; itT != itTEnd; ++itT) delete itT->first;

	DateMap::iterator itD = _dates.begin();
	DateMap::iterator itDEnd = _dates.end();
	for(; itD != itDEnd; ++itD) delete itD->first;

	TimestampMap::iterator itTS = _timestamps.begin();
	TimestampMap::iterator itTSEnd = _timestamps.end();
	for(; itTS != itTSEnd; ++itTS) delete itTS->first;

	StringMap::iterator itStr = _strings.begin();
	StringMap::iterator itStrEnd = _strings.end();
	for(; itStr != itStrEnd; ++itStr) std::free(itStr->first);

	CharPtrVec::iterator itChr = _charPtrs.begin();
	CharPtrVec::iterator endChr = _charPtrs.end();
	for (; itChr != endChr; ++itChr) std::free(*itChr);

	UTF16CharPtrVec::iterator itUTF16Chr = _utf16CharPtrs.begin();
	UTF16CharPtrVec::iterator endUTF16Chr = _utf16CharPtrs.end();
	for (; itUTF16Chr != endUTF16Chr; ++itUTF16Chr) std::free(*itUTF16Chr);

	UUIDMap::iterator itUUID = _uuids.begin();
	UUIDMap::iterator itUUIDEnd = _uuids.end();
	for(; itUUID != itUUIDEnd; ++itUUID) delete [] itUUID->first;

	BoolPtrVec::iterator itBool = _boolPtrs.begin();
	BoolPtrVec::iterator endBool = _boolPtrs.end();
	for (; itBool != endBool; ++itBool) delete [] *itBool;

	DateVecVec::iterator itDateVec = _dateVecVec.begin();
	DateVecVec::iterator itDateVecEnd = _dateVecVec.end();
	for (; itDateVec != itDateVecEnd; ++itDateVec) delete *itDateVec;

	TimeVecVec::iterator itTimeVec = _timeVecVec.begin();
	TimeVecVec::iterator itTimeVecEnd = _timeVecVec.end();
	for (; itTimeVec != itTimeVecEnd; ++itTimeVec) delete *itTimeVec;

	DateTimeVecVec::iterator itDateTimeVec = _dateTimeVecVec.begin();
	DateTimeVecVec::iterator itDateTimeVecEnd = _dateTimeVecVec.end();
	for (; itDateTimeVec != itDateTimeVecEnd; ++itDateTimeVec) delete *itDateTimeVec;

	if (transcodeRequired() && _inParams.size())
	{
		ParamMap::iterator itInParams = _inParams.begin();
		ParamMap::iterator itInParamsEnd = _inParams.end();
		for (; itInParams != itInParamsEnd; ++itInParams) free(itInParams->first);
	}
}


void Binder::bind(std::size_t pos, const std::string& val, Direction dir)
{
	char* pTCVal = 0;
	SQLINTEGER size = 0;
	if (transcodeRequired())
	{
		std::string tcVal;
		transcode(val, tcVal);
		size = (SQLINTEGER)tcVal.size();
		pTCVal = reinterpret_cast<char*>(std::calloc((size_t)size+1, 1));
		std::memcpy(pTCVal, tcVal.data(), size);
	}
	else size = (SQLINTEGER)val.size();
	SQLPOINTER pVal = 0;
	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_C_CHAR, colSize, decDigits, val.size());

	if (isOutBound(dir))
	{
		getColumnOrParameterSize(pos, size);
		char* pChar = (char*) std::calloc(size, sizeof(char));
		pVal = (SQLPOINTER) pChar;
		_outParams.insert(ParamMap::value_type(pVal, size));
		_strings.insert(StringMap::value_type(pChar, const_cast<std::string*>(&val)));
	}
	else if (isInBound(dir))
	{
		if (!pTCVal)
		{
			pVal = (SQLPOINTER)val.c_str();
			_inParams.insert(ParamMap::value_type(pVal, size));
		}
		else
		{
			pVal = (SQLPOINTER)pTCVal;
			_inParams.insert(ParamMap::value_type(pVal, size));
		}
	}
	else
		throw InvalidArgumentException("ODBC::Binder::bind(string):Parameter must be [in] OR [out] bound.");

	SQLLEN* pLenIn = new SQLLEN(SQL_NTS);

	if (PB_AT_EXEC == _paramBinding)
		*pLenIn = SQL_LEN_DATA_AT_EXEC(size);

	_lengthIndicator.push_back(pLenIn);

	if (Utility::isError(SQLBindParameter(_rStmt,
		(SQLUSMALLINT) pos + 1,
		toODBCDirection(dir),
		SQL_C_CHAR,
		Utility::sqlDataType(SQL_C_CHAR),
		(SQLUINTEGER) colSize,
		0,
		pVal,
		(SQLINTEGER) size,
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "ODBC::Binder::bind(string):SQLBindParameter(std::string)");
	}
}


void Binder::bind(std::size_t pos, const UTF16String& val, Direction dir)
{
	typedef UTF16String::value_type CharT;

	SQLPOINTER pVal = 0;
	SQLINTEGER size = (SQLINTEGER)(val.size() * sizeof(CharT));
	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_C_WCHAR, colSize, decDigits);

	if (isOutBound(dir))
	{
		getColumnOrParameterSize(pos, size);
		CharT* pChar = (CharT*)std::calloc(size, 1);
		pVal = (SQLPOINTER)pChar;
		_outParams.insert(ParamMap::value_type(pVal, size));
		_utf16Strings.insert(UTF16StringMap::value_type(pChar, const_cast<UTF16String*>(&val)));
	}
	else if (isInBound(dir))
	{
		pVal = (SQLPOINTER)val.c_str();
		_inParams.insert(ParamMap::value_type(pVal, size));
	}
	else
		throw InvalidArgumentException("ODBC::Binder::bind():Parameter must be [in] OR [out] bound.");

	SQLLEN* pLenIn = new SQLLEN(SQL_NTS);

	if (PB_AT_EXEC == _paramBinding)
	{
		*pLenIn = SQL_LEN_DATA_AT_EXEC(size);
	}

	_lengthIndicator.push_back(pLenIn);

	if (Utility::isError(SQLBindParameter(_rStmt,
		(SQLUSMALLINT)pos + 1,
		toODBCDirection(dir),
		SQL_C_WCHAR,
		Utility::sqlDataType(SQL_C_WCHAR),
		(SQLUINTEGER)colSize,
		0,
		pVal,
		(SQLINTEGER)size,
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "ODBC::Binder::bind(UTF16String):SQLBindParameter(std::string)");
	}
}


void Binder::bind(std::size_t pos, const Date& val, Direction dir)
{
	SQLINTEGER size = (SQLINTEGER) sizeof(SQL_DATE_STRUCT);
	SQLLEN* pLenIn = new SQLLEN;
	*pLenIn  = size;

	_lengthIndicator.push_back(pLenIn);

	SQL_DATE_STRUCT* pDS = new SQL_DATE_STRUCT;
	Utility::dateSync(*pDS, val);

	_dates.insert(DateMap::value_type(pDS, const_cast<Date*>(&val)));

	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_TYPE_DATE, colSize, decDigits);

	if (Utility::isError(SQLBindParameter(_rStmt,
		(SQLUSMALLINT) pos + 1,
		toODBCDirection(dir),
		SQL_C_TYPE_DATE,
		Utility::sqlDataType(SQL_C_TYPE_DATE),
		colSize,
		decDigits,
		(SQLPOINTER) pDS,
		0,
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "ODBC::Binder::bind(Date):SQLBindParameter(Date)");
	}
}


void Binder::bind(std::size_t pos, const Time& val, Direction dir)
{
	SQLINTEGER size = (SQLINTEGER) sizeof(SQL_TIME_STRUCT);
	SQLLEN* pLenIn = new SQLLEN;
	*pLenIn  = size;

	_lengthIndicator.push_back(pLenIn);

	SQL_TIME_STRUCT* pTS = new SQL_TIME_STRUCT;
	Utility::timeSync(*pTS, val);

	_times.insert(TimeMap::value_type(pTS, const_cast<Time*>(&val)));

	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_TYPE_TIME, colSize, decDigits);

	if (Utility::isError(SQLBindParameter(_rStmt,
		(SQLUSMALLINT) pos + 1,
		toODBCDirection(dir),
		SQL_C_TYPE_TIME,
		Utility::sqlDataType(SQL_C_TYPE_TIME),
		colSize,
		decDigits,
		(SQLPOINTER) pTS,
		0,
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "ODBC::Binder::bind(Time):SQLBindParameter(Time)");
	}
}


void Binder::bind(std::size_t pos, const Poco::DateTime& val, Direction dir)
{
	SQLINTEGER size = (SQLINTEGER) sizeof(SQL_TIMESTAMP_STRUCT);
	SQLLEN* pLenIn = new SQLLEN;
	*pLenIn  = size;

	_lengthIndicator.push_back(pLenIn);

	SQL_TIMESTAMP_STRUCT* pTS = new SQL_TIMESTAMP_STRUCT;
	Utility::dateTimeSync(*pTS, val);

	_timestamps.insert(TimestampMap::value_type(pTS, const_cast<DateTime*>(&val)));

	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_TYPE_TIMESTAMP, colSize, decDigits);

	if (Utility::isError(SQLBindParameter(_rStmt,
		(SQLUSMALLINT) pos + 1,
		toODBCDirection(dir),
		SQL_C_TYPE_TIMESTAMP,
		Utility::sqlDataType(SQL_C_TYPE_TIMESTAMP),
		colSize,
		decDigits,
		(SQLPOINTER) pTS,
		0,
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "ODBC::Binder::bind(DateTime):SQLBindParameter(DateTime)");
	}
}


void Binder::bind(std::size_t pos, const UUID& val, Direction dir)
{
	SQLINTEGER size = (SQLINTEGER) 16;
	SQLLEN* pLenIn = new SQLLEN;
	*pLenIn = size;

	_lengthIndicator.push_back(pLenIn);

	char* pUUID = new char[16];
	val.copyTo(pUUID);

	_uuids.insert(UUIDMap::value_type(pUUID, const_cast<UUID*>(&val)));

	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;

	if (Utility::isError(SQLBindParameter(_rStmt,
		(SQLUSMALLINT) pos + 1,
		toODBCDirection(dir),
		SQL_C_BINARY,
		SQL_GUID,
		colSize,
		decDigits,
		(SQLPOINTER) pUUID,
		0,
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "SQLBindParameter(UUID)");
	}
}


void Binder::bind(std::size_t pos, const NullData& val, Direction dir)
{
	if (isOutBound(dir) || !isInBound(dir))
		throw NotImplementedException("NULL parameter type can only be inbound.");

	_inParams.insert(ParamMap::value_type(SQLPOINTER(0), SQLINTEGER(0)));

	SQLLEN* pLenIn = new SQLLEN;
	*pLenIn  = SQL_NULL_DATA;

	_lengthIndicator.push_back(pLenIn);

	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_C_STINYINT, colSize, decDigits);

	if (Utility::isError(SQLBindParameter(_rStmt,
		(SQLUSMALLINT) pos + 1,
		SQL_PARAM_INPUT,
		SQL_C_STINYINT,
		Utility::sqlDataType(SQL_C_STINYINT),
		colSize,
		decDigits,
		0,
		0,
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "ODBC::Binder::bind(NullData):SQLBindParameter()");
	}
}


std::size_t Binder::parameterSize(SQLPOINTER pAddr) const
{
	ParamMap::const_iterator it = _inParams.find(pAddr);
	if (it != _inParams.end()) return it->second;

	it = _outParams.find(pAddr);
	if (it != _outParams.end()) return it->second;

	throw NotFoundException("Requested data size not found.");
}


void Binder::bind(std::size_t pos, const char* const &pVal, Direction dir)
{
	throw NotImplementedException("char* binding not implemented, Use std::string instead.");
}


SQLSMALLINT Binder::toODBCDirection(Direction dir) const
{
	bool in = isInBound(dir);
	bool out = isOutBound(dir);
	SQLSMALLINT ioType = SQL_PARAM_TYPE_UNKNOWN;
	if (in && out) ioType = SQL_PARAM_INPUT_OUTPUT;
	else if(in)    ioType = SQL_PARAM_INPUT;
	else if(out)   ioType = SQL_PARAM_OUTPUT;
	else throw Poco::IllegalStateException("Binder not bound (must be [in] OR [out]).");

	return ioType;
}


void Binder::synchronize()
{
	if (_dates.size())
	{
		DateMap::iterator it = _dates.begin();
		DateMap::iterator end = _dates.end();
		for(; it != end; ++it)
			Utility::dateSync(*it->second, *it->first);
	}

	if (_times.size())
	{
		TimeMap::iterator it = _times.begin();
		TimeMap::iterator end = _times.end();
		for(; it != end; ++it)
			Utility::timeSync(*it->second, *it->first);
	}

	if (_timestamps.size())
	{
		TimestampMap::iterator it = _timestamps.begin();
		TimestampMap::iterator end = _timestamps.end();
		for(; it != end; ++it)
			Utility::dateTimeSync(*it->second, *it->first);
	}

	if (!transcodeRequired())
	{
		if (_strings.size())
		{
			StringMap::iterator it = _strings.begin();
			StringMap::iterator end = _strings.end();
			for (; it != end; ++it)
				it->second->assign(it->first, std::strlen(it->first));
		}
	}
	else
	{
		if (_strings.size())
		{
			StringMap::iterator it = _strings.begin();
			StringMap::iterator end = _strings.end();
			for (; it != end; ++it)
			{
				std::string str(it->first, std::strlen(it->first));
				reverseTranscode(str, *it->second);
			}
		}
	}

	if (_uuids.size())
	{
		UUIDMap::iterator it = _uuids.begin();
		UUIDMap::iterator end = _uuids.end();
		for(; it != end; ++it)
			it->second->copyFrom(it->first);
	}
}


void Binder::reset()
{
	freeMemory();
	LengthPtrVec().swap(_lengthIndicator);
	_inParams.clear();
	_outParams.clear();
	_dates.clear();
	_times.clear();
	_timestamps.clear();
	_strings.clear();
	_uuids.clear();
	_dateVecVec.clear();
	_timeVecVec.clear();
	_dateTimeVecVec.clear();
	_charPtrs.clear();
	_boolPtrs.clear();
	_containers.clear();
	_paramSetSize = 0;
}


void Binder::getColSizeAndPrecision(std::size_t pos,
	SQLSMALLINT cDataType,
	SQLINTEGER& colSize,
	SQLSMALLINT& decDigits,
	std::size_t actualSize)
{
	SQLSMALLINT sqlDataType = Utility::sqlDataType(cDataType);
	colSize = 0;
	decDigits = 0;

	// Not all drivers are equally willing to cooperate in this matter.
	// Hence the funky flow control.
	if (_pTypeInfo)
	{
		DynamicAny tmp;
		bool foundSize(false);
		bool foundPrec(false);
		foundSize = _pTypeInfo->tryGetInfo(cDataType, "COLUMN_SIZE", tmp);
		if (foundSize) colSize = tmp;
		else foundSize = _pTypeInfo->tryGetInfo(sqlDataType, "COLUMN_SIZE", tmp);
		if (foundSize) colSize = tmp;

		if (actualSize > colSize)
		{
			throw LengthExceededException(Poco::format("Error binding column %z size=%z, max size=%ld)",
					pos, actualSize, static_cast<long>(colSize)));
		}
		foundPrec = _pTypeInfo->tryGetInfo(cDataType, "MAXIMUM_SCALE", tmp);
		if (foundPrec) decDigits = tmp;
		else foundPrec = _pTypeInfo->tryGetInfo(sqlDataType, "MAXIMUM_SCALE", tmp);
		if (foundPrec) decDigits = tmp;

		if (foundSize && foundPrec)
			return;
	}

	try
	{
		Parameter p(_rStmt, pos);
		colSize = (SQLINTEGER) p.columnSize();
		decDigits = (SQLSMALLINT) p.decimalDigits();
		return;
	}
	catch (StatementException&)
	{
	}

	try
	{
		ODBCMetaColumn c(_rStmt, pos);
		colSize = (SQLINTEGER) c.length();
		decDigits = (SQLSMALLINT) c.precision();
		return;
	}
	catch (StatementException&)
	{
	}

	// last check, just in case
	if ((0 != colSize) && (actualSize > colSize))
	{
		throw LengthExceededException(Poco::format("Error binding column %z size=%z, max size=%ld)",
				pos, actualSize, static_cast<long>(colSize)));
	}

	return;
}


std::size_t Binder::getParamSizeDirect(std::size_t pos, SQLINTEGER& size)
{
//On Linux, PostgreSQL driver segfaults on SQLGetDescField, so this is disabled for now
#ifdef POCO_OS_FAMILY_WINDOWS
	size = DEFAULT_PARAM_SIZE;
	SQLHDESC hIPD = 0;
	if (!Utility::isError(SQLGetStmtAttr(_rStmt, SQL_ATTR_IMP_PARAM_DESC, &hIPD, SQL_IS_POINTER, 0)))
	{
		SQLULEN sz = 0;
		if (!Utility::isError(SQLGetDescField(hIPD, (SQLSMALLINT)pos + 1, SQL_DESC_LENGTH, &sz, SQL_IS_UINTEGER, 0)) &&
			sz > 0)
		{
			size = (SQLINTEGER)sz;
		}
	}
#endif
	return static_cast<std::size_t>(size);
}


void Binder::getColumnOrParameterSize(std::size_t pos, SQLINTEGER& size)
{
	std::size_t colSize = 0;
	std::size_t paramSize = 0;

	try
	{
		ODBCMetaColumn col(_rStmt, pos);
		colSize = col.length();
	}
	catch (StatementException&) { }

	try
	{
		Parameter p(_rStmt, pos);
		paramSize = p.columnSize();
	}
	catch (StatementException&) {}

	if (colSize == 0 && paramSize == 0)
		paramSize = getParamSizeDirect(pos, size);

	if (colSize > 0 && paramSize > 0)
		size = colSize < paramSize ? static_cast<SQLINTEGER>(colSize) : static_cast<SQLINTEGER>(paramSize);
	else if (colSize > 0)
		size = static_cast<SQLINTEGER>(colSize);
	else if (paramSize > 0)
		size = static_cast<SQLINTEGER>(paramSize);

	if (size > _maxFieldSize) size = static_cast<SQLINTEGER>(_maxFieldSize);
}


void Binder::setParamSetSize(std::size_t length)
{
	if (0 == _paramSetSize)
	{
		if (Utility::isError(Poco::Data::ODBC::SQLSetStmtAttr(_rStmt, SQL_ATTR_PARAM_BIND_TYPE, SQL_PARAM_BIND_BY_COLUMN, SQL_IS_UINTEGER)) ||
			Utility::isError(Poco::Data::ODBC::SQLSetStmtAttr(_rStmt, SQL_ATTR_PARAMSET_SIZE, (SQLPOINTER) length, SQL_IS_UINTEGER)))
				throw StatementException(_rStmt, "ODBC::Binder::setParamSetSize():SQLSetStmtAttr()");

		_paramSetSize = static_cast<SQLINTEGER>(length);
	}
}


} } } // namespace Poco::Data::ODBC
