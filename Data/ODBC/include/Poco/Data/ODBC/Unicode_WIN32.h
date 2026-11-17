//
// Unicode.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Unicode
//
// Definition of Unicode_WIN32.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_ODBC_Unicode_WIN32_INCLUDED
#define Data_ODBC_Unicode_WIN32_INCLUDED

#include "Poco/Buffer.h"
#include "Poco/UnicodeConverter.h"
#include <sql.h>
#include <sqltypes.h>
#include <algorithm>
#include <cstring>
#include <string>

namespace Poco::Data::ODBC {


inline void makeUTF16(SQLCHAR* pSQLChar, SQLINTEGER length, std::wstring& target)
	/// Utility function for conversion from UTF-8 to UTF-16
{
	int len = length;
	if (SQL_NTS == len)
		len = (int) std::strlen((const char *) pSQLChar);

	UnicodeConverter::toUTF16((const char *) pSQLChar, len, target);
}


inline void makeUTF8(Poco::Buffer<wchar_t>& buffer, SQLINTEGER length, SQLPOINTER pTarget, SQLINTEGER targetLength)
	/// Utility function for conversion from UTF-16 to UTF-8. Length is in bytes.
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
	// Ensure length is aligned to wchar_t boundary for safe conversion
	length -= length % sizeof(wchar_t);

	length /= sizeof(wchar_t);
	length = static_cast<SQLINTEGER>(std::find(buffer.begin(), buffer.begin() + length, L'\0') - buffer.begin());
	std::string result;
	UnicodeConverter::toUTF8(buffer.begin(), length, result);

	// the value is truncated so that the terminator fits into the target
	std::memset(pTarget, 0, targetLength);
	if (targetLength > 0)
		std::memcpy(pTarget, result.data(), (std::min)(result.size(), static_cast<std::size_t>(targetLength - 1)));
}


} // namespace Poco::Data::ODBC


#endif // Data_ODBC_Unicode_WIN32_INCLUDED
