//
// Binder.cpp
//
// $Id: //poco/Main/Data/ODBC/src/Binder.cpp#4 $
//
// Library: ODBC
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
#include "Poco/Data/LOB.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/DateTime.h"
#include "Poco/Exception.h"
#include <sql.h>


namespace Poco {
namespace Data {
namespace ODBC {

static void getProp(const TypeInfo& dataTypes, SQLSMALLINT sqlType, size_t& val)
{
	const std::string NM("COLUMN_SIZE");
	Poco::DynamicAny r;
	if (dataTypes.tryGetInfo(sqlType, NM, r))
	{
		long sz = r.convert<long>();
		// Postgres driver returns SQL_NO_TOTAL(-4) in some cases
		if (sz >= 0)
			val = static_cast<size_t>(sz);
	}
}

Binder::Binder(const StatementHandle& rStmt,
	std::size_t maxFieldSize,
	Binder::ParameterBinding dataBinding,
	TypeInfo* pDataTypes,
	ODBCMetaColumn::NumericConversion numericConversion,
	bool insertOnly) :
	_rStmt(rStmt),
	_paramBinding(dataBinding),
	_pTypeInfo(pDataTypes),
	_paramSetSize(0),
	_maxFieldSize(maxFieldSize),
	_maxCharColLength(1024),
	_maxWCharColLength(1024),
	_maxVarBinColSize(1024),
	_numericConversion(numericConversion),
	_insertOnly(insertOnly)
{
	getProp(*_pTypeInfo, SQL_WVARCHAR, _maxWCharColLength);
	getProp(*_pTypeInfo, SQL_VARCHAR, _maxCharColLength);
	getProp(*_pTypeInfo, SQL_VARBINARY, _maxVarBinColSize);
}


Binder::~Binder()
{
	freeMemory();
}


void Binder::freeMemory()
{
	if (_lengthIndicator.size() > 0)
	{
		LengthPtrVec::iterator itLen = _lengthIndicator.begin();
		LengthPtrVec::iterator itLenEnd = _lengthIndicator.end();
		for (; itLen != itLenEnd; ++itLen) delete *itLen;
	}

	if (_vecLengthIndicator.size() > 0)
	{
		LengthVecVec::iterator itVecLen = _vecLengthIndicator.begin();
		LengthVecVec::iterator itVecLenEnd = _vecLengthIndicator.end();
		for (; itVecLen != itVecLenEnd; ++itVecLen) delete *itVecLen;
	}

	if (_times.size() > 0)
	{
		TimeMap::iterator itT = _times.begin();
		TimeMap::iterator itTEnd = _times.end();
		for (; itT != itTEnd; ++itT) delete itT->first;
	}

	if (_dates.size() > 0)
	{
		DateMap::iterator itD = _dates.begin();
		DateMap::iterator itDEnd = _dates.end();
		for (; itD != itDEnd; ++itD) delete itD->first;
	}

	if (_timestamps.size() > 0)
	{
		TimestampMap::iterator itTS = _timestamps.begin();
		TimestampMap::iterator itTSEnd = _timestamps.end();
		for (; itTS != itTSEnd; ++itTS) delete itTS->first;
	}

	if (_strings.size() > 0)
	{
		StringMap::iterator itStr = _strings.begin();
		StringMap::iterator itStrEnd = _strings.end();
		for (; itStr != itStrEnd; ++itStr) std::free(itStr->first);
	}

	if (_charPtrs.size() > 0)
	{
		CharPtrVec::iterator itChr = _charPtrs.begin();
		CharPtrVec::iterator endChr = _charPtrs.end();
		for (; itChr != endChr; ++itChr) std::free(*itChr);
	}

	if (_utf16CharPtrs.size() > 0)
	{
		UTF16CharPtrVec::iterator itUTF16Chr = _utf16CharPtrs.begin();
		UTF16CharPtrVec::iterator endUTF16Chr = _utf16CharPtrs.end();
		for (; itUTF16Chr != endUTF16Chr; ++itUTF16Chr) std::free(*itUTF16Chr);
	}

	if (_boolPtrs.size() > 0)
	{
		BoolPtrVec::iterator itBool = _boolPtrs.begin();
		BoolPtrVec::iterator endBool = _boolPtrs.end();
		for (; itBool != endBool; ++itBool) delete[] * itBool;
	}

	if (_dateVecVec.size() > 0)
	{
		DateVecVec::iterator itDateVec = _dateVecVec.begin();
		DateVecVec::iterator itDateVecEnd = _dateVecVec.end();
		for (; itDateVec != itDateVecEnd; ++itDateVec) delete *itDateVec;
	}

	if (_timeVecVec.size() > 0)
	{
		TimeVecVec::iterator itTimeVec = _timeVecVec.begin();
		TimeVecVec::iterator itTimeVecEnd = _timeVecVec.end();
		for (; itTimeVec != itTimeVecEnd; ++itTimeVec) delete *itTimeVec;
	}

	if (_dateTimeVecVec.size() > 0)
	{
		DateTimeVecVec::iterator itDateTimeVec = _dateTimeVecVec.begin();
		DateTimeVecVec::iterator itDateTimeVecEnd = _dateTimeVecVec.end();
		for (; itDateTimeVec != itDateTimeVecEnd; ++itDateTimeVec) delete *itDateTimeVec;
	}

	if (_containers.size() > 0)
	{
		AnyPtrVecVec::iterator itAnyVec = _containers.begin();
		AnyPtrVecVec::iterator itAnyVecEnd = _containers.end();
		for (; itAnyVec != itAnyVecEnd; ++itAnyVec)
		{
			AnyPtrVec::iterator b = itAnyVec->begin();
			AnyPtrVec::iterator e = itAnyVec->end();
			for (; b != e; ++b) delete *b;
		}
	}
}


void Binder::bind(std::size_t pos, const std::string& val, Direction dir, const WhenNullCb& nullCb)
{
	SQLPOINTER pVal = 0;
	SQLINTEGER size = (SQLINTEGER) val.size();

	SQLSMALLINT sqType = SQL_LONGVARCHAR;
	if (isOutBound(dir))
	{
		getColumnOrParameterSize(pos, size);
		char* pChar = (char*) std::calloc(size, sizeof(char));
		pVal = (SQLPOINTER) pChar;
		_outParams.insert(ParamMap::value_type(pVal, size));
		_strings.insert(StringMap::value_type(pChar, const_cast<std::string*>(&val)));
		if (size < _maxCharColLength) sqType = SQL_VARCHAR;
	}
	else if (isInBound(dir))
	{
		pVal = (SQLPOINTER) val.c_str();
		_inParams.insert(ParamMap::value_type(pVal, size));
		if (size < _maxCharColLength) sqType = SQL_VARCHAR;
	}
	else
		throw InvalidArgumentException("Parameter must be [in] OR [out] bound.");

	SQLLEN* pLenIn = new SQLLEN;
	if (isOutBound(dir) && nullCb.defined())
		_nullCbMap.insert(NullCbMap::value_type( pLenIn, nullCb) );
	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_C_CHAR, colSize, decDigits);
	*pLenIn = SQL_NTS;

	if (PB_AT_EXEC == _paramBinding)
		*pLenIn = SQL_LEN_DATA_AT_EXEC(size);

	_lengthIndicator.push_back(pLenIn);

	if (Utility::isError(SQLBindParameter(_rStmt, 
		(SQLUSMALLINT) pos + 1, 
		toODBCDirection(dir), 
		SQL_C_CHAR, 
		sqType,
		(SQLUINTEGER) colSize,
		0,
		pVal, 
		(SQLINTEGER) size, 
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "SQLBindParameter(std::string)");
	}
}


void Binder::bind(std::size_t pos, const UTF16String& val, Direction dir, const WhenNullCb& nullCb)
{
	typedef UTF16String::value_type CharT;

	SQLPOINTER pVal = 0;
	SQLINTEGER size = (SQLINTEGER)(val.size() * sizeof(CharT));
	SQLSMALLINT sqType = (val.size() < _maxWCharColLength) ? SQL_WVARCHAR : SQL_WLONGVARCHAR;
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
		throw InvalidArgumentException("Parameter must be [in] OR [out] bound.");

	SQLLEN* pLenIn = new SQLLEN;
	if (isOutBound(dir) && nullCb.defined())
		_nullCbMap.insert(NullCbMap::value_type(pLenIn, nullCb));

	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_C_WCHAR, colSize, decDigits);
	*pLenIn = SQL_NTS;

	if (PB_AT_EXEC == _paramBinding)
	{
		*pLenIn = SQL_LEN_DATA_AT_EXEC(size);
	}

	_lengthIndicator.push_back(pLenIn);

	if (Utility::isError(SQLBindParameter(_rStmt,
		(SQLUSMALLINT)pos + 1,
		toODBCDirection(dir),
		SQL_C_WCHAR,
		sqType,
		(SQLUINTEGER)colSize,
		0,
		pVal,
		(SQLINTEGER)size,
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "SQLBindParameter(std::string)");
	}
}


void Binder::bind(std::size_t pos, const Date& val, Direction dir, const WhenNullCb& nullCb)
{
	SQLLEN* pLenIn = new SQLLEN;
	*pLenIn = SQL_NTS; // microsoft example does that, otherwise no null indicator is returned

	_lengthIndicator.push_back(pLenIn);
	if (isOutBound(dir) && nullCb.defined()) 
		_nullCbMap.insert(NullCbMap::value_type(pLenIn, nullCb));
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
		SQL_TYPE_DATE, 
		colSize,
		decDigits,
		(SQLPOINTER) pDS, 
		0, 
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "SQLBindParameter(Date)");
	}
}


void Binder::bind(std::size_t pos, const Time& val, Direction dir, const WhenNullCb& nullCb)
{
	SQLLEN* pLenIn = new SQLLEN;
	*pLenIn  = SQL_NTS; // microsoft example does that, otherwise no null indicator is returned
	if (isOutBound(dir) && nullCb.defined())
		_nullCbMap.insert(NullCbMap::value_type(pLenIn, nullCb));

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
		SQL_TYPE_TIME, 
		colSize,
		decDigits,
		(SQLPOINTER) pTS, 
		0, 
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "SQLBindParameter(Time)");
	}
}


void Binder::bind(std::size_t pos, const Poco::DateTime& val, Direction dir, const WhenNullCb& nullCb)
{
	SQLLEN* pLenIn = new SQLLEN;
	*pLenIn = SQL_NTS; // microsoft example does that, otherwise no null indicator is returned
	if (isOutBound(dir) && nullCb.defined())
		_nullCbMap.insert(NullCbMap::value_type(pLenIn, nullCb));

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
		SQL_TYPE_TIMESTAMP, 
		colSize,
		decDigits,
		(SQLPOINTER) pTS, 
		0, 
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "SQLBindParameter(DateTime)");
	}
}


void Binder::bind(std::size_t pos, const NullData& val, Direction dir, const std::type_info& bindType)
{
	if (isOutBound(dir) || !isInBound(dir))
		throw NotImplementedException("NULL parameter type can only be inbound.");

	_inParams.insert(ParamMap::value_type(SQLPOINTER(0), SQLINTEGER(0)));

	SQLLEN* pLenIn = new SQLLEN;
	*pLenIn  = SQL_NULL_DATA;

	_lengthIndicator.push_back(pLenIn);

	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;

	const SQLSMALLINT colType = (bindType == typeid(void) || bindType == typeid(NullData) || bindType == typeid(NullType)) ?
														_pTypeInfo->nullDataType(val) : _pTypeInfo->tryTypeidToCType(bindType, SQL_C_TINYINT);
	getColSizeAndPrecision(pos, colType, colSize, decDigits);

	if (Utility::isError(SQLBindParameter(_rStmt, 
		(SQLUSMALLINT) pos + 1, 
		SQL_PARAM_INPUT, 
		colType,
		Utility::sqlDataType(colType),
		colSize,
		decDigits,
		0, 
		0, 
		_lengthIndicator.back())))
	{
		throw StatementException(_rStmt, "SQLBindParameter()");
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


void Binder::bind(std::size_t pos, const char* const &pVal, Direction dir, const WhenNullCb& nullCb)
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

	if (_strings.size())
	{
		StringMap::iterator it = _strings.begin();
		StringMap::iterator end = _strings.end();
		for(; it != end; ++it)
			it->second->assign(it->first, std::strlen(it->first));
	}

	if (_nullCbMap.size())
	{
		NullCbMap::iterator it = _nullCbMap.begin();
		NullCbMap::iterator end = _nullCbMap.end();
		for (; it != end; ++it)
			if (*it->first == SQL_NULL_DATA) it->second.onNull();
	}
}


void Binder::reset()
{
	freeMemory();

	if (_lengthIndicator.size() > 0) 
		LengthPtrVec().swap(_lengthIndicator);
	if (_inParams.size() > 0)
		_inParams.clear();
	if (_outParams.size() > 0)
		_outParams.clear();
	if (_dates.size() > 0)
		_dates.clear();
	if (_times.size() > 0)
		_times.clear();
	if (_timestamps.size() > 0)
		_timestamps.clear();
	if (_strings.size() > 0)
		_strings.clear();
	if (_dateVecVec.size() > 0)
		_dateVecVec.clear();
	if (_timeVecVec.size() > 0)
		_timeVecVec.clear();
	if (_dateTimeVecVec.size() > 0)
		_dateTimeVecVec.clear();
	if (_charPtrs.size() > 0)
		_charPtrs.clear();
	if (_boolPtrs.size() > 0)
		_boolPtrs.clear();
	if (_containers.size() > 0)
		_containers.clear();
	if (_nullCbMap.size() > 0)
		_nullCbMap.clear();
	_paramSetSize = 0;
	if (!_insertOnly)
		_parameters.clear();
}


void Binder::getColSizeAndPrecision(std::size_t pos, 
	SQLSMALLINT cDataType, 
	SQLINTEGER& colSize, 
	SQLSMALLINT& decDigits)
{
	// Not all drivers are equally willing to cooperate in this matter.
	// Hence the funky flow control.

	if (_pTypeInfo)
	{
		DynamicAny tmp;
		bool found = _pTypeInfo->tryGetInfo(cDataType, "COLUMN_SIZE", tmp);
		if (found) colSize = tmp;
		found = _pTypeInfo->tryGetInfo(cDataType, "MINIMUM_SCALE", tmp);
		if (found)
		{
			decDigits = tmp;
			return;
		}
	}

	if (_parameters.size() <= pos || !_parameters[pos].defined())
	{
		if (_parameters.size() <= pos)
			_parameters.resize(pos + 1);
		_parameters[pos] = ParamDescriptor(0, cDataType, 0);

		try
		{
			{
				Parameter p(_rStmt, pos);
				_parameters[pos] = ParamDescriptor(static_cast<SQLINTEGER>(p.columnSize()), cDataType, static_cast<SQLSMALLINT>(p.decimalDigits()));
			}
		} 
		catch (StatementException&) 
		{
			try
			{
				ODBCMetaColumn c(_rStmt, pos, _numericConversion);
				_parameters[pos] = ParamDescriptor(static_cast<SQLINTEGER>(c.length()), cDataType, static_cast<SQLSMALLINT>(c.precision()));
			}
			catch (StatementException&) {}
		}
	}

	// we may have no success, so use zeros and hope for the best
	// (most drivers do not require these most of the times anyway)
	colSize = _parameters[pos].colSize;
	decDigits = _parameters[pos].decDigits;
}


void Binder::getColumnOrParameterSize(std::size_t pos, SQLINTEGER& size)
{
	std::size_t colSize = 0;
	std::size_t paramSize = 0;

	try
	{
		ODBCMetaColumn col(_rStmt, pos, _numericConversion);
		colSize = col.length();
	}
	catch (StatementException&) { }

	try
	{
		Parameter p(_rStmt, pos);
		paramSize = p.columnSize();
	}
	catch (StatementException&)
	{
		size = DEFAULT_PARAM_SIZE;
//On Linux, PostgreSQL driver segfaults on SQLGetDescField, so this is disabled for now
#ifdef POCO_OS_FAMILY_WINDOWS
		SQLHDESC hIPD = 0;
		if (!Utility::isError(Poco::Data::ODBC::SQLGetStmtAttr(_rStmt, SQL_ATTR_IMP_PARAM_DESC, &hIPD, SQL_IS_POINTER, 0)))
		{
			SQLUINTEGER sz = 0;
			if (!Utility::isError(Poco::Data::ODBC::SQLGetDescField(hIPD, (SQLSMALLINT)pos + 1, SQL_DESC_LENGTH, &sz, SQL_IS_UINTEGER, 0)) &&
				sz > 0)
			{
				size = sz;
			}
		}
#endif
	}

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
				throw StatementException(_rStmt, "SQLSetStmtAttr()");

		_paramSetSize = static_cast<SQLINTEGER>(length);
	}
}


} } } // namespace Poco::Data::ODBC
