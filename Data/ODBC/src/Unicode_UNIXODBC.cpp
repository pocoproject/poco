//
// Unicode.cpp
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Unicode
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/Unicode_UNIXODBC.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/TextConverter.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/UTF16Encoding.h"
#include "Poco/Buffer.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <cstring>
#include <limits>
#include <iostream>


using Poco::Buffer;
using Poco::UTF8Encoding;
using Poco::UTF16Encoding;
using Poco::TextConverter;
using Poco::InvalidArgumentException;
using Poco::NotImplementedException;


namespace Poco::Data::ODBC {


void makeUTF16(SQLCHAR* pSQLChar, SQLINTEGER length, std::string& target)
{
	int len = length;
	if (SQL_NTS == len)
		len = (int) std::strlen((const char *) pSQLChar);

	UTF8Encoding utf8Encoding;
	UTF16Encoding utf16Encoding;
	TextConverter converter(utf8Encoding, utf16Encoding);

	if (0 != converter.convert(pSQLChar, len, target))
		throw DataFormatException("Error converting UTF-8 to UTF-16");
}


namespace {


SQLINTEGER charCount(const std::string& utf16)
	/// Returns the number of UTF-16 code units in a string made by makeUTF16().
{
	return static_cast<SQLINTEGER>(utf16.size() / sizeof(SQLWCHAR));
}


} // namespace


void makeUTF8(Poco::Buffer<SQLWCHAR>& buffer, SQLINTEGER length, SQLPOINTER pTarget, SQLINTEGER targetLength)
{
	// Some ODBC drivers (notably IBM Informix) may report a returned
	// string length larger than the allocated wide-char buffer, or
	// return non-positive lengths. Handle these defensively instead
	// of throwing, to avoid breaking metadata retrieval for drivers
	// with non-conformant length reporting.
	//
	// Note: this function handles metadata strings (column names,
	// diagnostics, driver info), not row data, which the driver terminates.
	// A driver may also report more than it wrote (the Oracle driver reports
	// the length of a diagnostic message in bytes), so the value ends at its
	// terminator and the rest of the buffer, which may be uninitialised, is
	// not converted.
	if (length <= 0)
	{
		std::memset(pTarget, 0, targetLength);
		return;
	}
	if (length > static_cast<SQLINTEGER>(buffer.sizeBytes()))
		length = static_cast<SQLINTEGER>(buffer.sizeBytes());
	// Ensure length is aligned to SQLWCHAR boundary for safe conversion
	length -= length % sizeof(SQLWCHAR);
	const SQLWCHAR* pBegin = buffer.begin();
	const SQLWCHAR* pEnd = std::find(pBegin, pBegin + length / sizeof(SQLWCHAR), SQLWCHAR(0));
	length = static_cast<SQLINTEGER>((pEnd - pBegin) * sizeof(SQLWCHAR));

	UTF8Encoding utf8Encoding;
	UTF16Encoding utf16Encoding;
	TextConverter converter(utf16Encoding, utf8Encoding);

	std::string result;
	if (0 != converter.convert(buffer.begin(), length, result))
		throw DataFormatException("Error converting UTF-16 to UTF-8");

	// the value is truncated so that the terminator fits into the target
	std::memset(pTarget, 0, targetLength);
	if (targetLength > 0)
		std::memcpy(pTarget, result.data(), std::min(result.size(), static_cast<std::size_t>(targetLength - 1)));
}


SQLRETURN SQLColAttribute(SQLHSTMT hstmt,
	SQLUSMALLINT   iCol,
	SQLUSMALLINT   iField,
	SQLPOINTER	   pCharAttr,
	SQLSMALLINT	   cbCharAttrMax,
	SQLSMALLINT*   pcbCharAttr,
	NumAttrPtrType pNumAttr)
{
	SQLSMALLINT cbCharAttr = 0;
	if (!pcbCharAttr) pcbCharAttr = &cbCharAttr;

	if (isString(pCharAttr, cbCharAttrMax))
	{
		Buffer<SQLWCHAR> buffer(stringLength(pCharAttr, cbCharAttrMax));

		SQLRETURN rc = SQLColAttributeW(hstmt,
			iCol,
			iField,
			buffer.begin(),
			(SQLSMALLINT) buffer.sizeBytes(),
			pcbCharAttr,
			pNumAttr);

		if (!Utility::isError(rc))
			makeUTF8(buffer, *pcbCharAttr, pCharAttr, cbCharAttrMax);

		return rc;
	}

	return SQLColAttributeW(hstmt,
		iCol,
		iField,
		pCharAttr,
		cbCharAttrMax,
		pcbCharAttr,
		pNumAttr);
}


SQLRETURN SQLColAttributes(SQLHSTMT hstmt,
	SQLUSMALLINT icol,
	SQLUSMALLINT fDescType,
	SQLPOINTER   rgbDesc,
	SQLSMALLINT  cbDescMax,
	SQLSMALLINT* pcbDesc,
	SQLLEN*      pfDesc)
{
	SQLSMALLINT cbDesc = 0;
	if (!pcbDesc) pcbDesc = &cbDesc;
	SQLLEN fDesc = 0;
	if (!pfDesc) pfDesc = &fDesc;

	return SQLColAttribute(hstmt,
		icol,
		fDescType,
		rgbDesc,
		cbDescMax,
		pcbDesc,
		pfDesc);
}


SQLRETURN SQLConnect(SQLHDBC hdbc,
	SQLCHAR*    szDSN,
	SQLSMALLINT cbDSN,
	SQLCHAR*    szUID,
	SQLSMALLINT cbUID,
	SQLCHAR*    szAuthStr,
	SQLSMALLINT cbAuthStr)
{
	std::string sqlDSN;
	makeUTF16(szDSN, cbDSN, sqlDSN);

	std::string sqlUID;
	makeUTF16(szUID, cbUID, sqlUID);

	std::string sqlPWD;
	makeUTF16(szAuthStr, cbAuthStr, sqlPWD);

	return SQLConnectW(hdbc,
		reinterpret_cast<SQLWCHAR*>(sqlDSN.data()), static_cast<SQLSMALLINT>(charCount(sqlDSN)),
		reinterpret_cast<SQLWCHAR*>(sqlUID.data()), static_cast<SQLSMALLINT>(charCount(sqlUID)),
		reinterpret_cast<SQLWCHAR*>(sqlPWD.data()), static_cast<SQLSMALLINT>(charCount(sqlPWD)));
}


SQLRETURN SQLDescribeCol(SQLHSTMT hstmt,
	SQLUSMALLINT icol,
	SQLCHAR*     szColName,
	SQLSMALLINT  cbColNameMax,
	SQLSMALLINT* pcbColName,
	SQLSMALLINT* pfSqlType,
	SQLULEN*     pcbColDef,
	SQLSMALLINT* pibScale,
	SQLSMALLINT* pfNullable)
{
	SQLSMALLINT cbColName = 0;
	if (!pcbColName) pcbColName = &cbColName;
	SQLSMALLINT fSqlType = 0;
	if (!pfSqlType) pfSqlType = &fSqlType;
	SQLULEN cbColDef = 0;
	if (!pcbColDef) pcbColDef = &cbColDef;
	SQLSMALLINT ibScale = 0;
	if (!pibScale) pibScale = &ibScale;
	SQLSMALLINT fNullable = 0;
	if (!pfNullable) pfNullable = &fNullable;

	Buffer<SQLWCHAR> buffer(cbColNameMax);
	SQLRETURN rc = SQLDescribeColW(hstmt,
		icol,
		(SQLWCHAR*) buffer.begin(),
		(SQLSMALLINT) buffer.size(),
		pcbColName,
		pfSqlType,
		pcbColDef,
		pibScale,
		pfNullable);

	if (!Utility::isError(rc))
		makeUTF8(buffer, *pcbColName * sizeof(SQLWCHAR), szColName, cbColNameMax);

	return rc;
}


SQLRETURN SQLError(SQLHENV henv,
	SQLHDBC      hdbc,
	SQLHSTMT     hstmt,
	SQLCHAR*     szSqlState,
	SQLINTEGER*  pfNativeError,
	SQLCHAR*     szErrorMsg,
	SQLSMALLINT  cbErrorMsgMax,
	SQLSMALLINT* pcbErrorMsg)
{
	throw NotImplementedException("SQLError is obsolete. "
		"Use SQLGetDiagRec instead.");
}


SQLRETURN SQLExecDirect(SQLHSTMT hstmt,
	SQLCHAR*   szSqlStr,
	SQLINTEGER cbSqlStr)
{
	std::string sqlStr;
	makeUTF16(szSqlStr, cbSqlStr, sqlStr);

	return SQLExecDirectW(hstmt, reinterpret_cast<SQLWCHAR*>(sqlStr.data()), charCount(sqlStr));
}


SQLRETURN SQLGetConnectAttr(SQLHDBC hdbc,
	SQLINTEGER  fAttribute,
	SQLPOINTER  rgbValue,
	SQLINTEGER  cbValueMax,
	SQLINTEGER* pcbValue)
{
	SQLINTEGER cbValue = 0;
	if (!pcbValue) pcbValue = &cbValue;

	if (isString(rgbValue, cbValueMax))
	{
		Buffer<SQLWCHAR> buffer(stringLength(rgbValue, cbValueMax));

		SQLRETURN rc = SQLGetConnectAttrW(hdbc,
				fAttribute,
				buffer.begin(),
				(SQLINTEGER) buffer.sizeBytes(),
				pcbValue);

		if (!Utility::isError(rc))
			makeUTF8(buffer, *pcbValue, rgbValue, cbValueMax);
		return rc;
	}


	return SQLGetConnectAttrW(hdbc,
		fAttribute,
		rgbValue,
		cbValueMax,
		pcbValue);
}


SQLRETURN SQLGetCursorName(SQLHSTMT hstmt,
	SQLCHAR*     szCursor,
	SQLSMALLINT  cbCursorMax,
	SQLSMALLINT* pcbCursor)
{
	throw NotImplementedException("Not implemented");
}


SQLRETURN SQLSetDescField(SQLHDESC hdesc,
	SQLSMALLINT iRecord,
	SQLSMALLINT iField,
	SQLPOINTER  rgbValue,
	SQLINTEGER  cbValueMax)
{
	if (isString(rgbValue, cbValueMax))
	{
		std::string str;
		makeUTF16((SQLCHAR*) rgbValue, cbValueMax, str);

		return SQLSetDescFieldW(hdesc,
			iRecord,
			iField,
			str.data(),
			static_cast<SQLINTEGER>(str.size()));
	}

	return SQLSetDescFieldW(hdesc,
		iRecord,
		iField,
		rgbValue,
		cbValueMax);
}


SQLRETURN SQLGetDescField(SQLHDESC hdesc,
	SQLSMALLINT iRecord,
	SQLSMALLINT iField,
	SQLPOINTER  rgbValue,
	SQLINTEGER	cbValueMax,
	SQLINTEGER* pcbValue)
{
	SQLINTEGER cbValue = 0;
	if (!pcbValue) pcbValue = &cbValue;

	if (isString(rgbValue, cbValueMax))
	{
		Buffer<SQLWCHAR> buffer(stringLength(rgbValue, cbValueMax));

		SQLRETURN rc = SQLGetDescFieldW(hdesc,
			iRecord,
			iField,
			buffer.begin(),
			(SQLINTEGER) buffer.sizeBytes(),
			pcbValue);

		if (!Utility::isError(rc))
			makeUTF8(buffer, *pcbValue, rgbValue, cbValueMax);

		return rc;
	}

	return SQLGetDescFieldW(hdesc,
		iRecord,
		iField,
		rgbValue,
		cbValueMax,
		pcbValue);
}


SQLRETURN SQLGetDescRec(SQLHDESC hdesc,
	SQLSMALLINT  iRecord,
	SQLCHAR*     szName,
	SQLSMALLINT  cbNameMax,
	SQLSMALLINT* pcbName,
	SQLSMALLINT* pfType,
	SQLSMALLINT* pfSubType,
	SQLLEN*      pLength,
	SQLSMALLINT* pPrecision,
	SQLSMALLINT* pScale,
	SQLSMALLINT* pNullable)
{
	throw NotImplementedException();
}


SQLRETURN SQLGetDiagField(SQLSMALLINT fHandleType,
	SQLHANDLE    handle,
	SQLSMALLINT  iRecord,
	SQLSMALLINT  fDiagField,
	SQLPOINTER   rgbDiagInfo,
	SQLSMALLINT  cbDiagInfoMax,
	SQLSMALLINT* pcbDiagInfo)
{
	SQLSMALLINT cbDiagInfo = 0;
	if (!pcbDiagInfo) pcbDiagInfo = &cbDiagInfo;

	if (isString(rgbDiagInfo, cbDiagInfoMax))
	{
		// For a driver without SQLGetDiagFieldW(), unixODBC reads the message
		// into a buffer of the given length in bytes and converts every
		// character of it, as SQLGetInfo() describes.
		const SQLINTEGER length = std::min<SQLINTEGER>(stringLength(rgbDiagInfo, cbDiagInfoMax),
			std::numeric_limits<SQLSMALLINT>::max() / sizeof(SQLWCHAR));
		Buffer<SQLWCHAR> buffer(2 * length + 1);
		std::fill(buffer.begin(), buffer.end(), 0);

		SQLRETURN rc = SQLGetDiagFieldW(fHandleType,
			handle,
			iRecord,
			fDiagField,
			buffer.begin(),
			static_cast<SQLSMALLINT>(length * sizeof(SQLWCHAR)),
			pcbDiagInfo);

		if (!Utility::isError(rc))
			makeUTF8(buffer, *pcbDiagInfo, rgbDiagInfo, cbDiagInfoMax);

		return rc;
	}

	return SQLGetDiagFieldW(fHandleType,
		handle,
		iRecord,
		fDiagField,
		rgbDiagInfo,
		cbDiagInfoMax,
		pcbDiagInfo);
}


SQLRETURN SQLGetDiagRec(SQLSMALLINT fHandleType,
	SQLHANDLE    handle,
	SQLSMALLINT  iRecord,
	SQLCHAR*     szSqlState,
	SQLINTEGER*  pfNativeError,
	SQLCHAR*     szErrorMsg,
	SQLSMALLINT  cbErrorMsgMax,
	SQLSMALLINT* pcbErrorMsg)
{
	SQLINTEGER fNativeError = 0;
	if (!pfNativeError) pfNativeError = &fNativeError;
	SQLSMALLINT cbErrorMsg = 0;
	if (!pcbErrorMsg) pcbErrorMsg = &cbErrorMsg;

	const SQLINTEGER stateLen = SQL_SQLSTATE_SIZE + 1;
	Buffer<SQLWCHAR> bufState(stateLen);
	Buffer<SQLWCHAR> bufErr(cbErrorMsgMax);

	SQLRETURN rc = SQLGetDiagRecW(fHandleType,
		handle,
		iRecord,
		bufState.begin(),
		pfNativeError,
		bufErr.begin(),
		(SQLSMALLINT) bufErr.size(),
		pcbErrorMsg);

	if (!Utility::isError(rc))
	{
		makeUTF8(bufState, stateLen * sizeof(SQLWCHAR), szSqlState, stateLen);
		makeUTF8(bufErr, *pcbErrorMsg * sizeof(SQLWCHAR), szErrorMsg, cbErrorMsgMax);
	}

	return rc;
}


SQLRETURN SQLPrepare(SQLHSTMT hstmt,
	SQLCHAR*   szSqlStr,
	SQLINTEGER cbSqlStr)
{
	std::string sqlStr;
	makeUTF16(szSqlStr, cbSqlStr, sqlStr);

	return SQLPrepareW(hstmt, reinterpret_cast<SQLWCHAR*>(sqlStr.data()), charCount(sqlStr));
}


SQLRETURN SQLSetConnectAttr(SQLHDBC hdbc,
	SQLINTEGER fAttribute,
	SQLPOINTER rgbValue,
	SQLINTEGER cbValue)
{
	if (isString(rgbValue, cbValue))
	{
		std::string str;
		makeUTF16((SQLCHAR*) rgbValue, cbValue, str);

		return SQLSetConnectAttrW(hdbc,
			fAttribute,
			str.data(),
			static_cast<SQLINTEGER>(str.size()));
	}

	return SQLSetConnectAttrW(hdbc,	fAttribute,	rgbValue, cbValue);
}


SQLRETURN SQLSetCursorName(SQLHSTMT hstmt,
	SQLCHAR*    szCursor,
	SQLSMALLINT cbCursor)
{
	throw NotImplementedException("Not implemented");
}


SQLRETURN SQLSetStmtAttr(SQLHSTMT hstmt,
	SQLINTEGER fAttribute,
	SQLPOINTER rgbValue,
	SQLINTEGER cbValueMax)
{
	if (isString(rgbValue, cbValueMax))
	{
		std::string str;
		makeUTF16((SQLCHAR*) rgbValue, cbValueMax, str);

		return SQLSetStmtAttrW(hstmt,
			fAttribute,
			(SQLWCHAR*) str.c_str(),
			(SQLINTEGER) str.size());
	}

	return SQLSetStmtAttrW(hstmt, fAttribute, rgbValue,	cbValueMax);
}


SQLRETURN SQLGetStmtAttr(SQLHSTMT hstmt,
	SQLINTEGER  fAttribute,
	SQLPOINTER  rgbValue,
	SQLINTEGER  cbValueMax,
	SQLINTEGER* pcbValue)
{
	SQLINTEGER cbValue = 0;
	if (!pcbValue) pcbValue = &cbValue;

	if (isString(rgbValue, cbValueMax))
	{
		Buffer<SQLWCHAR> buffer(stringLength(rgbValue, cbValueMax));

		return SQLGetStmtAttrW(hstmt,
			fAttribute,
			(SQLPOINTER) buffer.begin(),
			(SQLINTEGER) buffer.sizeBytes(),
			pcbValue);
	}

	return SQLGetStmtAttrW(hstmt, fAttribute, rgbValue, cbValueMax, pcbValue);
}


SQLRETURN SQLColumns(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szTableName,
	SQLSMALLINT cbTableName,
	SQLCHAR*    szColumnName,
	SQLSMALLINT cbColumnName)
{
	throw NotImplementedException();
}


SQLRETURN SQLGetConnectOption(SQLHDBC hdbc,
	SQLUSMALLINT fOption,
	SQLPOINTER   pvParam)
{
	throw NotImplementedException();
}


SQLRETURN SQLGetInfo(SQLHDBC hdbc,
	SQLUSMALLINT fInfoType,
	SQLPOINTER   rgbInfoValue,
	SQLSMALLINT  cbInfoValueMax,
	SQLSMALLINT* pcbInfoValue)
{
	SQLSMALLINT cbInfoValue = 0;
	if (!pcbInfoValue) pcbInfoValue = &cbInfoValue;

	if (cbInfoValueMax > 0)
	{
		// For a driver without SQLGetInfoW(), unixODBC reads the value into
		// a buffer of the given length in bytes and converts every character
		// of it into the given buffer, which then needs twice the length and
		// a terminator. The driver is told about the first half only, in bytes
		// as an SQLSMALLINT; a longer value is truncated like any value that
		// does not fit.
		const SQLSMALLINT chars = std::min<SQLSMALLINT>(cbInfoValueMax,
			static_cast<SQLSMALLINT>(std::numeric_limits<SQLSMALLINT>::max() / sizeof(SQLWCHAR)));
		Buffer<SQLWCHAR> buffer(2 * chars + 1);
		std::fill(buffer.begin(), buffer.end(), 0);

		SQLRETURN rc = SQLGetInfoW(hdbc,
			fInfoType,
			(SQLPOINTER) buffer.begin(),
			static_cast<SQLSMALLINT>(chars * sizeof(SQLWCHAR)),
			pcbInfoValue);

		if (!Utility::isError(rc))
			makeUTF8(buffer, *pcbInfoValue, rgbInfoValue, cbInfoValueMax);

		return rc;
	}

	return SQLGetInfoW(hdbc, fInfoType, rgbInfoValue, cbInfoValueMax, pcbInfoValue);
}


SQLRETURN SQLGetTypeInfo(SQLHSTMT StatementHandle, SQLSMALLINT DataType)
{
	return SQLGetTypeInfoW(StatementHandle, DataType);
}


SQLRETURN SQLSetConnectOption(SQLHDBC hdbc,
	SQLUSMALLINT fOption,
	SQLULEN      vParam)
{
	throw NotImplementedException();
}


SQLRETURN SQLSpecialColumns(SQLHSTMT hstmt,
	SQLUSMALLINT fColType,
	SQLCHAR*     szCatalogName,
	SQLSMALLINT  cbCatalogName,
	SQLCHAR*     szSchemaName,
	SQLSMALLINT  cbSchemaName,
	SQLCHAR*     szTableName,
	SQLSMALLINT  cbTableName,
	SQLUSMALLINT fScope,
	SQLUSMALLINT fNullable)
{
	throw NotImplementedException();
}


SQLRETURN SQLStatistics(SQLHSTMT hstmt,
	SQLCHAR*     szCatalogName,
	SQLSMALLINT  cbCatalogName,
	SQLCHAR*     szSchemaName,
	SQLSMALLINT  cbSchemaName,
	SQLCHAR*     szTableName,
	SQLSMALLINT  cbTableName,
	SQLUSMALLINT fUnique,
	SQLUSMALLINT fAccuracy)
{
	throw NotImplementedException();
}


SQLRETURN SQLTables(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szTableName,
	SQLSMALLINT cbTableName,
	SQLCHAR*    szTableType,
	SQLSMALLINT cbTableType)
{
	throw NotImplementedException();
}


SQLRETURN SQLDataSources(SQLHENV henv,
	SQLUSMALLINT fDirection,
	SQLCHAR*     szDSN,
	SQLSMALLINT  cbDSNMax,
	SQLSMALLINT* pcbDSN,
	SQLCHAR*     szDesc,
	SQLSMALLINT  cbDescMax,
	SQLSMALLINT* pcbDesc)
{
	SQLSMALLINT cbDSN = 0, cbDesc = 0;
	if (!pcbDSN) pcbDSN = &cbDSN;
	if (!pcbDesc) pcbDesc = &cbDesc;

	Buffer<SQLWCHAR> bufDSN(cbDSNMax);
	Buffer<SQLWCHAR> bufDesc(cbDescMax);

	SQLRETURN rc = SQLDataSourcesW(henv,
		fDirection,
		bufDSN.begin(),
		(SQLSMALLINT) bufDSN.size(),
		pcbDSN,
		bufDesc.begin(),
		(SQLSMALLINT) bufDesc.size(),
		pcbDesc);

	if (!Utility::isError(rc))
	{
		makeUTF8(bufDSN, *pcbDSN * sizeof(SQLWCHAR), szDSN, cbDSNMax);
		makeUTF8(bufDesc, *pcbDesc * sizeof(SQLWCHAR), szDesc, cbDescMax);
	}

	return rc;
}


SQLRETURN SQLDriverConnect(SQLHDBC hdbc,
	SQLHWND      hwnd,
	SQLCHAR*     szConnStrIn,
	SQLSMALLINT  cbConnStrIn,
	SQLCHAR*     szConnStrOut,
	SQLSMALLINT  cbConnStrOutMax,
	SQLSMALLINT* pcbConnStrOut,
	SQLUSMALLINT fDriverCompletion)
{
	SQLSMALLINT cbConnStrOut = 0;
	if (!pcbConnStrOut) pcbConnStrOut = &cbConnStrOut;

	SQLSMALLINT len = cbConnStrIn;
	if (SQL_NTS == len)
		len = static_cast<SQLSMALLINT>(std::strlen(reinterpret_cast<const char*>(szConnStrIn)));

	std::string connStrIn;
	makeUTF16(szConnStrIn, len, connStrIn);

	Buffer<SQLWCHAR> out(cbConnStrOutMax);
	SQLRETURN rc = SQLDriverConnectW(hdbc,
		hwnd,
		reinterpret_cast<SQLWCHAR*>(connStrIn.data()),
		static_cast<SQLSMALLINT>(charCount(connStrIn)),
		out.begin(),
		cbConnStrOutMax,
		pcbConnStrOut,
		fDriverCompletion);

	if (!Utility::isError(rc) && szConnStrOut)
		makeUTF8(out, *pcbConnStrOut * sizeof(SQLWCHAR), szConnStrOut, cbConnStrOutMax);

	return rc;
}


SQLRETURN SQLBrowseConnect(SQLHDBC hdbc,
	SQLCHAR*     szConnStrIn,
	SQLSMALLINT  cbConnStrIn,
	SQLCHAR*     szConnStrOut,
	SQLSMALLINT  cbConnStrOutMax,
	SQLSMALLINT* pcbConnStrOut)
{
	SQLSMALLINT cbConnStrOut = 0;
	if (!pcbConnStrOut) pcbConnStrOut = &cbConnStrOut;

	std::string str;
	makeUTF16(szConnStrIn, cbConnStrIn, str);

	Buffer<SQLWCHAR> bufConnStrOut(cbConnStrOutMax);

	SQLRETURN rc = SQLBrowseConnectW(hdbc,
		reinterpret_cast<SQLWCHAR*>(str.data()),
		static_cast<SQLSMALLINT>(charCount(str)),
		bufConnStrOut.begin(),
		(SQLSMALLINT) bufConnStrOut.size(),
		pcbConnStrOut);

	if (!Utility::isError(rc))
		makeUTF8(bufConnStrOut, *pcbConnStrOut * sizeof(SQLWCHAR), szConnStrOut, cbConnStrOutMax);

	return rc;
}


SQLRETURN SQLColumnPrivileges(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szTableName,
	SQLSMALLINT cbTableName,
	SQLCHAR*    szColumnName,
	SQLSMALLINT cbColumnName)
{
	throw NotImplementedException();
}


SQLRETURN SQLForeignKeys(SQLHSTMT hstmt,
	SQLCHAR*    szPkCatalogName,
	SQLSMALLINT cbPkCatalogName,
	SQLCHAR*    szPkSchemaName,
	SQLSMALLINT cbPkSchemaName,
	SQLCHAR*    szPkTableName,
	SQLSMALLINT cbPkTableName,
	SQLCHAR*    szFkCatalogName,
	SQLSMALLINT cbFkCatalogName,
	SQLCHAR*    szFkSchemaName,
	SQLSMALLINT cbFkSchemaName,
	SQLCHAR*    szFkTableName,
	SQLSMALLINT cbFkTableName)
{
	throw NotImplementedException();
}


SQLRETURN SQLNativeSql(SQLHDBC hdbc,
	SQLCHAR*    szSqlStrIn,
	SQLINTEGER  cbSqlStrIn,
	SQLCHAR*    szSqlStr,
	SQLINTEGER  cbSqlStrMax,
	SQLINTEGER* pcbSqlStr)
{
	SQLINTEGER cbSqlStr = 0;
	if (!pcbSqlStr) pcbSqlStr = &cbSqlStr;

	std::string str;
	makeUTF16(szSqlStrIn, cbSqlStrIn, str);
	const SQLINTEGER inLength = charCount(str);
	// Some drivers (SQLite ODBC 0.99991) write a terminator into the input,
	// at the position of the output buffer length. A driver with wide-character
	// functions gets the input below, which is therefore as long as the output
	// buffer. A driver without them gets a copy that unixODBC converts into
	// 4 * inLength + 1 bytes, so the output length passed on is not larger.
	constexpr SQLINTEGER maxLength = (std::numeric_limits<SQLINTEGER>::max)();
	const SQLINTEGER convertedLength = (inLength < (maxLength - 1) / 4) ? 4 * inLength + 1 : maxLength;
	const SQLINTEGER outLength = (std::max<SQLINTEGER>)(0, (std::min)(cbSqlStrMax, convertedLength));
	if (outLength > inLength)
		str.resize((static_cast<std::size_t>(outLength) + 1) * sizeof(SQLWCHAR), '\0');

	Buffer<SQLWCHAR> bufSQLOut(static_cast<std::size_t>(outLength));

	SQLRETURN rc = SQLNativeSqlW(hdbc,
		reinterpret_cast<SQLWCHAR*>(str.data()),
		inLength,
		bufSQLOut.begin(),
		(SQLINTEGER) bufSQLOut.size(),
		pcbSqlStr);

	if (!Utility::isError(rc))
		makeUTF8(bufSQLOut, *pcbSqlStr * sizeof(SQLWCHAR), szSqlStr, cbSqlStrMax);

	return rc;
}


SQLRETURN SQLPrimaryKeys(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szTableName,
	SQLSMALLINT cbTableName)
{
	throw NotImplementedException();
}


SQLRETURN SQLProcedureColumns(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szProcName,
	SQLSMALLINT cbProcName,
	SQLCHAR*    szColumnName,
	SQLSMALLINT cbColumnName)
{
	throw NotImplementedException();
}


SQLRETURN SQLProcedures(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szProcName,
	SQLSMALLINT cbProcName)
{
	throw NotImplementedException();
}


SQLRETURN SQLTablePrivileges(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szTableName,
	SQLSMALLINT cbTableName)
{
	throw NotImplementedException();
}


SQLRETURN SQLDrivers(SQLHENV henv,
	SQLUSMALLINT fDirection,
	SQLCHAR*     szDriverDesc,
	SQLSMALLINT  cbDriverDescMax,
	SQLSMALLINT* pcbDriverDesc,
	SQLCHAR*     szDriverAttributes,
	SQLSMALLINT  cbDrvrAttrMax,
	SQLSMALLINT* pcbDrvrAttr)
{
	SQLSMALLINT cbDriverDesc = 0, cbDrvrAttr = 0;
	if (!pcbDriverDesc) pcbDriverDesc = &cbDriverDesc;
	if (!pcbDrvrAttr) pcbDrvrAttr = &cbDrvrAttr;

	Buffer<SQLWCHAR> bufDriverDesc(cbDriverDescMax);
	Buffer<SQLWCHAR> bufDriverAttr(cbDrvrAttrMax);

	SQLRETURN rc = SQLDriversW(henv,
		fDirection,
		bufDriverDesc.begin(),
		(SQLSMALLINT) bufDriverDesc.size(),
		pcbDriverDesc,
		bufDriverAttr.begin(),
		(SQLSMALLINT) bufDriverAttr.size(),
		pcbDrvrAttr);

	if (!Utility::isError(rc))
	{
		makeUTF8(bufDriverDesc, *pcbDriverDesc * sizeof(SQLWCHAR), szDriverDesc, cbDriverDescMax);
		makeUTF8(bufDriverAttr, *pcbDrvrAttr * sizeof(SQLWCHAR), szDriverAttributes, cbDrvrAttrMax);
	}

	return rc;
}


} // namespace Poco::Data::ODBC
