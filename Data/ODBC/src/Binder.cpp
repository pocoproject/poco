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
#include "Poco/Data/ODBC/ODBCMetaColumn.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Parameter.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/DateTime.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <limits>
#include <sql.h>


namespace Poco::Data::ODBC {


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

	UTF16StringMap::iterator itUTF16Str = _utf16Strings.begin();
	UTF16StringMap::iterator itUTF16StrEnd = _utf16Strings.end();
	for (; itUTF16Str != itUTF16StrEnd; ++itUTF16Str) std::free(itUTF16Str->first);

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

	// Free only the transcoded copies the binder allocated. The _inParams keys
	// can be memory the caller owns (a LOB's content, a UTF16String's buffer),
	// so they must not be freed here.
	CharPtrVec::iterator itTC = _transcodedStrings.begin();
	CharPtrVec::iterator endTC = _transcodedStrings.end();
	for (; itTC != endTC; ++itTC) std::free(*itTC);
}


void Binder::bind(std::size_t pos, const std::string& val, Direction dir)
{
	rejectScalarWithParamArray();
	char* pTCVal = nullptr;
	SQLINTEGER size = 0;
	if (transcodeRequired())
	{
		std::string tcVal;
		transcode(val, tcVal);
		// the length is passed to the driver as an SQLINTEGER
		if (tcVal.size() > static_cast<std::size_t>(std::numeric_limits<SQLINTEGER>::max()))
			throw LengthExceededException("ODBC::Binder::bind(string): the transcoded value is too long.");
		size = static_cast<SQLINTEGER>(tcVal.size());
		pTCVal = static_cast<char*>(Utility::allocArray(1, tcVal.size() + 1, sizeof(char)));
		std::memcpy(pTCVal, tcVal.data(), tcVal.size());
		// the binder owns this copy and frees it in freeMemory(), on every exit
		// path (also when a later step throws, and for outbound parameters where
		// it is otherwise unused)
		_transcodedStrings.push_back(pTCVal);
	}
	else
	{
		if (val.size() > static_cast<std::size_t>(std::numeric_limits<SQLINTEGER>::max()))
			throw LengthExceededException("ODBC::Binder::bind(string): the value is too long.");
		size = static_cast<SQLINTEGER>(val.size());
	}
	SQLPOINTER pVal = nullptr;
	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_C_CHAR, colSize, decDigits, val.size());

	SQLLEN* pLenIn = new SQLLEN(SQL_NTS);
	_lengthIndicator.push_back(pLenIn);

	if (isOutBound(dir))
	{
		// The buffer receives the value and the terminator the driver appends,
		// and holds the value io() sends. It is as long as the size the driver
		// reports, or DEFAULT_PARAM_SIZE, limited to the maximum field size, and
		// no shorter than the value: io() sends all of it, and an output string
		// sized in advance keeps its length.
		const std::size_t valueLength = pTCVal ? static_cast<std::size_t>(size) : val.size();
		getColumnOrParameterSize(pos, size);
		std::size_t bufferLength = (size > 0) ? static_cast<std::size_t>(size) : std::size_t(0);
		if (bufferLength < valueLength) bufferLength = valueLength;
		++bufferLength;
		if (bufferLength > static_cast<std::size_t>(std::numeric_limits<SQLINTEGER>::max()))
			throw LengthExceededException("ODBC::Binder::bind(string): the output parameter buffer is too long.");
		char* pChar = static_cast<char*>(Utility::allocArray(1, bufferLength, sizeof(char)));
		_strings.insert(StringMap::value_type(pChar,
			OutString<std::string>{const_cast<std::string*>(&val), static_cast<SQLLEN>(bufferLength), pLenIn}));
		if (isInBound(dir))
			std::memcpy(pChar, pTCVal ? pTCVal : val.data(), valueLength);
		pVal = static_cast<SQLPOINTER>(pChar);
		size = static_cast<SQLINTEGER>(bufferLength);
		// SQLPutData() sends the value of io(), not the whole buffer
		_outParams.insert(ParamMap::value_type(pVal, isInBound(dir) ? static_cast<SQLLEN>(valueLength) : static_cast<SQLLEN>(size)));
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

	if (PB_AT_EXEC == _paramBinding)
		*pLenIn = SQL_LEN_DATA_AT_EXEC(static_cast<SQLLEN>(parameterSize(pVal)));

	int rc = SQLBindParameter(_rStmt,
		(SQLUSMALLINT)pos + 1,
		toODBCDirection(dir),
		SQL_C_CHAR,
		TypeInfo::sqlDataType<SQL_C_CHAR>(),
		getStringColSize(colSize),
		0,
		pVal,
		(SQLINTEGER)size,
		_lengthIndicator.back());

	if (Utility::isError(rc))
	{
		throw StatementException(_rStmt, "ODBC::Binder::bind(string):SQLBindParameter(std::string)");
	}
}


void Binder::bind(std::size_t pos, const UTF16String& val, Direction dir)
{
	rejectScalarWithParamArray();
	typedef UTF16String::value_type CharT;

	SQLPOINTER pVal = nullptr;
	// the length is passed to the driver as an SQLINTEGER
	if (val.size() > static_cast<std::size_t>(std::numeric_limits<SQLINTEGER>::max()) / sizeof(CharT))
		throw LengthExceededException("ODBC::Binder::bind(UTF16String): the value is too long.");
	SQLINTEGER size = static_cast<SQLINTEGER>(val.size() * sizeof(CharT));
	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_C_WCHAR, colSize, decDigits);

	SQLLEN* pLenIn = new SQLLEN(SQL_NTS);
	_lengthIndicator.push_back(pLenIn);

	if (isOutBound(dir))
	{
		// as for std::string, but in characters, as the size the driver reports
		SQLINTEGER chars = 0;
		getColumnOrParameterSize(pos, chars);
		std::size_t bufferChars = (chars > 0) ? static_cast<std::size_t>(chars) : std::size_t(0);
		if (bufferChars < val.size()) bufferChars = val.size();
		++bufferChars;
		if (bufferChars > static_cast<std::size_t>(std::numeric_limits<SQLINTEGER>::max()) / sizeof(CharT))
			throw LengthExceededException("ODBC::Binder::bind(UTF16String): the output parameter buffer is too long.");
		CharT* pChar = static_cast<CharT*>(Utility::allocArray(1, bufferChars, sizeof(CharT)));
		_utf16Strings.insert(UTF16StringMap::value_type(pChar,
			OutString<UTF16String>{const_cast<UTF16String*>(&val), static_cast<SQLLEN>(bufferChars * sizeof(CharT)), pLenIn}));
		if (isInBound(dir))
			std::memcpy(pChar, val.data(), val.size() * sizeof(CharT));
		pVal = (SQLPOINTER)pChar;
		const SQLLEN valueLength = static_cast<SQLLEN>(val.size() * sizeof(CharT));
		size = static_cast<SQLINTEGER>(bufferChars * sizeof(CharT));
		// SQLPutData() sends the value of io(), not the whole buffer
		_outParams.insert(ParamMap::value_type(pVal, isInBound(dir) ? valueLength : static_cast<SQLLEN>(size)));
	}
	else if (isInBound(dir))
	{
		pVal = (SQLPOINTER)val.c_str();
		_inParams.insert(ParamMap::value_type(pVal, size));
	}
	else
		throw InvalidArgumentException("ODBC::Binder::bind():Parameter must be [in] OR [out] bound.");

	if (PB_AT_EXEC == _paramBinding)
	{
		*pLenIn = SQL_LEN_DATA_AT_EXEC(static_cast<SQLLEN>(parameterSize(pVal)));
	}

	if (Utility::isError(SQLBindParameter(_rStmt,
		(SQLUSMALLINT)pos + 1,
		toODBCDirection(dir),
		SQL_C_WCHAR,
		TypeInfo::sqlDataType<SQL_C_WCHAR>(),
		getStringColSize(colSize),
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
	rejectScalarWithParamArray();
	SQLINTEGER size = Utility::sizeOf(val);
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
		TypeInfo::sqlDataType<SQL_C_TYPE_DATE>(),
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
	rejectScalarWithParamArray();
	SQLINTEGER size = Utility::sizeOf(val);
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
		TypeInfo::sqlDataType<SQL_C_TYPE_TIME>(),
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
	rejectScalarWithParamArray();
	SQLINTEGER size = Utility::sizeOf(val);
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
		TypeInfo::sqlDataType<SQL_C_TYPE_TIMESTAMP>(),
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
	rejectScalarWithParamArray();
	SQLINTEGER size = Utility::sizeOf(val);
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
	rejectScalarWithParamArray();
	if (isOutBound(dir) || !isInBound(dir))
		throw NotImplementedException("NULL parameter type can only be inbound.");

	_inParams.insert(ParamMap::value_type(SQLPOINTER(nullptr), SQLINTEGER(0)));

	SQLLEN* pLenIn = new SQLLEN;
	*pLenIn  = SQL_NULL_DATA;

	_lengthIndicator.push_back(pLenIn);

	SQLINTEGER colSize = 0;
	SQLSMALLINT decDigits = 0;
	getColSizeAndPrecision(pos, SQL_C_CHAR, colSize, decDigits);

	if (Utility::isError(SQLBindParameter(_rStmt,
		(SQLUSMALLINT) pos + 1,
		SQL_PARAM_INPUT,
		SQL_C_CHAR,
		TypeInfo::sqlDataType<SQL_C_CHAR>(),
		colSize,
		decDigits,
		nullptr,
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


namespace {


// Returns the number of characters of the value the driver returned in the
// buffer of a string output parameter. NULL is empty. A length the driver
// returned (in bytes) that fits into the buffer is the length of the value,
// which can contain NUL characters. Otherwise the value ends at the
// terminator, and no further than the buffer: the driver did not set the
// length (SQL_NTS, or the length of data at execution), or it truncated the
// value (SQL_NO_TOTAL, or a longer length), which sets truncated.
template <typename C>
std::size_t outputLength(const C* pBuffer, SQLLEN bufferLength, SQLLEN length, bool& truncated)
{
	if (SQL_NULL_DATA == length) return 0;

	// the buffer holds this many characters and the terminator
	const std::size_t limit = static_cast<std::size_t>(bufferLength) / sizeof(C) - 1;
	if (length >= 0 && static_cast<std::size_t>(length) / sizeof(C) <= limit)
		return static_cast<std::size_t>(length) / sizeof(C);

	if (length >= 0 || SQL_NO_TOTAL == length)
		truncated = true;

	return static_cast<std::size_t>(std::find(pBuffer, pBuffer + limit, C(0)) - pBuffer);
}


} // namespace


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

	// a value that did not fit is reported once every output parameter is transferred
	bool truncated = false;

	StringMap::iterator itStr = _strings.begin();
	StringMap::iterator itStrEnd = _strings.end();
	for (; itStr != itStrEnd; ++itStr)
	{
		const OutString<std::string>& out = itStr->second;
		const std::size_t length = outputLength(itStr->first, out.bufferLength, *out.pLength, truncated);
		if (!transcodeRequired())
			out.pValue->assign(itStr->first, length);
		else
			reverseTranscode(std::string(itStr->first, length), *out.pValue);
	}

	UTF16StringMap::iterator itUTF16Str = _utf16Strings.begin();
	UTF16StringMap::iterator itUTF16StrEnd = _utf16Strings.end();
	for (; itUTF16Str != itUTF16StrEnd; ++itUTF16Str)
	{
		const OutString<UTF16String>& out = itUTF16Str->second;
		out.pValue->assign(itUTF16Str->first, outputLength(itUTF16Str->first, out.bufferLength, *out.pLength, truncated));
	}

	if (_uuids.size())
	{
		UUIDMap::iterator it = _uuids.begin();
		UUIDMap::iterator end = _uuids.end();
		for(; it != end; ++it)
			it->second->copyFrom(it->first);
	}

	if (truncated)
	{
		throw LengthExceededException("ODBC::Binder::synchronize(): the value of a string output parameter "
			"was truncated to fit into its buffer. Resize the output string to the length of the value "
			"before the execution, or, if the driver reports a parameter size larger than the session "
			"property maxFieldSize, raise maxFieldSize.");
	}
}


void Binder::reset()
{
	if (!_detached && _rStmt.isValid())
	{
		// The driver must not keep the addresses of the buffers freed below: a
		// parameter that is not bound again would be read from (or, for an
		// output parameter, written to) freed memory. Unbound markers make the
		// next execution fail instead. Nothing to do if this fails.
		static_cast<void>(SQLFreeStmt(_rStmt, SQL_RESET_PARAMS));
	}
	freeMemory();
	LengthPtrVec().swap(_lengthIndicator);
	// freeMemory() deleted these: the next bind must allocate them again
	LengthVecVec().swap(_vecLengthIndicator);
	_inParams.clear();
	_outParams.clear();
	_dates.clear();
	_times.clear();
	_timestamps.clear();
	_strings.clear();
	_utf16Strings.clear();
	_uuids.clear();
	_dateVecVec.clear();
	_timeVecVec.clear();
	_dateTimeVecVec.clear();
	_charPtrs.clear();
	_utf16CharPtrs.clear();
	_boolPtrs.clear();
	_containers.clear();
	_transcodedStrings.clear();
	if (!_detached && _paramSetSize > 1 && _rStmt.isValid())
	{
		// the next execution may bind scalars only; nothing to do if this fails
		static_cast<void>(Poco::Data::ODBC::SQLSetStmtAttr(_rStmt, SQL_ATTR_PARAMSET_SIZE, reinterpret_cast<SQLPOINTER>(static_cast<SQLULEN>(1)), SQL_IS_UINTEGER));
	}
	_paramSetSize = 0;
}


void Binder::detach()
{
	// run while the handle is still valid, then stop touching it
	reset();
	_detached = true;
}


SQLUINTEGER Binder::getStringColSize(SQLUINTEGER columnSize)
{
#if defined(POCO_DATA_ODBC_HAVE_SQL_SERVER_EXT) && POCO_DATA_SQL_SERVER_BIG_STRINGS
	if (Utility::dbmsName(_rStmt.connection()) == Utility::MS_SQL_SERVER_DBMS_NAME)
		return SQL_SS_LENGTH_UNLIMITED;
#endif
	return columnSize;
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
		Dynamic::Var tmp;
		bool foundSize(false);
		bool foundPrec(false);

		// SQLServer driver reports COLUMN_SIZE 8000 for VARCHAR(MAX),
		// so the size check must be skipped when big strings are enabled
#ifdef POCO_DATA_ODBC_HAVE_SQL_SERVER_EXT
		bool isVarchar(false);
		switch (sqlDataType)
		{
		case SQL_VARCHAR:
		case SQL_WVARCHAR:
		case SQL_WLONGVARCHAR:
			isVarchar = true;
			break;
		default: break;
		}
#endif // POCO_DATA_ODBC_HAVE_SQL_SERVER_EXT

		foundSize = _pTypeInfo->tryGetInfo(cDataType, "COLUMN_SIZE", tmp);
		if (foundSize) colSize = tmp;
		else foundSize = _pTypeInfo->tryGetInfo(sqlDataType, "COLUMN_SIZE", tmp);
		if (foundSize) colSize = tmp;

		if (actualSize > static_cast<std::size_t>(colSize)
#ifdef POCO_DATA_ODBC_HAVE_SQL_SERVER_EXT
			&& !isVarchar
#endif
			)
		{
			throw LengthExceededException(Poco::format("ODBC::Binder::getColSizeAndPrecision();%d: Error binding column %z size=%z, max size=%ld)",
				__LINE__, pos, actualSize, static_cast<long>(colSize)));
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
		colSize = (SQLINTEGER)p.columnSize();
		decDigits = (SQLSMALLINT)p.decimalDigits();
		return;
	}
	catch (StatementException&)
	{
	}

	try
	{
		ODBCMetaColumn c(_rStmt, pos);
		colSize = (SQLINTEGER)c.length();
		decDigits = (SQLSMALLINT)c.precision();
		return;
	}
	catch (StatementException&)
	{
	}

	// last check, just in case
	if ((0 != colSize) && (actualSize > static_cast<std::size_t>(colSize)))
	{
		throw LengthExceededException(Poco::format("ODBC::Binder::getColSizeAndPrecision();%d: Error binding column %z size=%z, max size=%ld)",
			__LINE__, pos, actualSize, static_cast<long>(colSize)));
	}

	return;
}


std::size_t Binder::getParamSizeDirect(std::size_t pos)
{
	// on every platform, so that no parameter gets an empty buffer
	std::size_t size = DEFAULT_PARAM_SIZE;
//On Linux, PostgreSQL driver segfaults on SQLGetDescField, so this is disabled for now
#ifdef POCO_OS_FAMILY_WINDOWS
	SQLHDESC hIPD = 0;
	if (!Utility::isError(SQLGetStmtAttr(_rStmt, SQL_ATTR_IMP_PARAM_DESC, &hIPD, SQL_IS_POINTER, 0)))
	{
		SQLULEN sz = 0;
		if (!Utility::isError(SQLGetDescField(hIPD, (SQLSMALLINT)pos + 1, SQL_DESC_LENGTH, &sz, SQL_IS_UINTEGER, 0)) &&
			sz > 0)
		{
			size = static_cast<std::size_t>(sz);
		}
	}
#endif
	return size;
}


void Binder::getColumnOrParameterSize(std::size_t pos, SQLINTEGER& size)
{
	// The size the driver describes for the parameter. The result column at the
	// same position is in general a different value, so it neither shortens nor
	// replaces that size. It gives the size only if the driver describes none,
	// as for the return value of {? = call ...} with drivers that return it as
	// a result column.
	std::size_t paramSize = 0;

	try
	{
		Parameter p(_rStmt, pos);
		paramSize = p.columnSize();
	}
	catch (StatementException&){}

	if (0 == paramSize)
	{
		try
		{
			ODBCMetaColumn col(_rStmt, pos);
			paramSize = col.length();
		}
		catch (StatementException&){}
	}

	if (0 == paramSize)
		paramSize = getParamSizeDirect(pos);

	// limited before it is narrowed, so that a larger size can not wrap around
	size = static_cast<SQLINTEGER>(std::min({paramSize, _maxFieldSize,
		static_cast<std::size_t>(std::numeric_limits<SQLINTEGER>::max())}));
}


void Binder::setParamSetSize(std::size_t length)
{
	if (0 == length)
		throw InvalidArgumentException("ODBC::Binder::setParamSetSize(): empty container not allowed.");
	if (length > 1 && !_lengthIndicator.empty())
		throw InvalidArgumentException("ODBC::Binder: a single value and a parameter array can not be bound together.");
	if (0 == _paramSetSize)
	{
		if (Utility::isError(Poco::Data::ODBC::SQLSetStmtAttr(_rStmt, SQL_ATTR_PARAM_BIND_TYPE, reinterpret_cast<SQLPOINTER>(SQL_PARAM_BIND_BY_COLUMN), SQL_IS_UINTEGER)) ||
			Utility::isError(Poco::Data::ODBC::SQLSetStmtAttr(_rStmt, SQL_ATTR_PARAMSET_SIZE, reinterpret_cast<SQLPOINTER>(length), SQL_IS_UINTEGER)))
				throw StatementException(_rStmt, "ODBC::Binder::setParamSetSize():SQLSetStmtAttr()");

		_paramSetSize = static_cast<SQLINTEGER>(length);
	}
	else if (static_cast<std::size_t>(_paramSetSize) != length)
	{
		// the driver reads _paramSetSize values from every parameter array
		throw InvalidArgumentException("ODBC::Binder::setParamSetSize(): parameter arrays of different sizes can not be bound together.");
	}
}


} // namespace Poco::Data::ODBC
