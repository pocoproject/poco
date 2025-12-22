//
// Error.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Error
//
// Definition of Error.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_ODBC_Error_INCLUDED
#define Data_ODBC_Error_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Format.h"
#include <vector>
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqlext.h>


namespace Poco {
namespace Data {
namespace ODBC {


template <typename H, SQLSMALLINT handleType>
class Error
	/// Class encapsulating ODBC diagnostic record collection. Collection is generated
	/// during construction. Class provides access and string generation for the collection
	/// as well as individual diagnostic records.
{
public:
	explicit Error(const H& handle) : _pDiag(new Diagnostics<H, handleType>(handle))
		/// Creates the Error.
	{
	}

	Error(const Error& other) : Error(other.diagnostics().handle())
		/// Creates the Error from another one.
	{
	}

	~Error()
		/// Destroys the Error.
	{
	}

	Error& operator = (const Error& other)
		/// Assigns another Error to this one.
	{
		_pDiag.reset(new Diagnostics<H, handleType>(other.diagnostics().handle()));
		return *this;
	}

	const Diagnostics<H, handleType>& diagnostics() const
		/// Returns the associated diagnostics.
	{
		return *_pDiag;
	}

	int count() const
		/// Returns the count of diagnostic records.
	{
		return (int) _pDiag->count();
	}

	std::string& toString(int index, std::string& str) const
		/// Generates the string for the diagnostic record.
	{
		if ((index < 0) || (index > (count() - 1)))
			return str;

		std::string s;
		Poco::format(s,
			"===========================\n"
			"ODBC Diagnostic record #%d:\n"
			"===========================\n"
			"SQLSTATE = %s\nNative Error Code = %ld\n%s\n\n",
			index + 1,
			_pDiag->sqlState(index),
			_pDiag->nativeError(index),
			_pDiag->message(index));

		str.append(s);

		return str;
	}

	std::string toString() const
		/// Generates the string for the diagnostic record collection.
	{
		std::string str;

		Poco::format(str,
			"Connection:%s\nServer:%s\n",
			_pDiag->connectionName(),
			_pDiag->serverName());

		std::string s;
		for (int i = 0; i < count(); ++i)
		{
			s.clear();
			str.append(toString(i, s));
		}

		return str;
	}

private:
	Error();

	std::unique_ptr<Diagnostics<H, handleType>> _pDiag;
};


// explicit instantiation definition
#ifndef POCO_DOC

#if defined(POCO_OS_FAMILY_WINDOWS)
extern template class Error<SQLHENV, SQL_HANDLE_ENV>;
extern template class Error<SQLHDBC, SQL_HANDLE_DBC>;
extern template class Error<SQLHSTMT, SQL_HANDLE_STMT>;
extern template class Error<SQLHDESC, SQL_HANDLE_DESC>;
#else
extern template class ODBC_API Error<SQLHENV, SQL_HANDLE_ENV>;
extern template class ODBC_API Error<SQLHDBC, SQL_HANDLE_DBC>;
extern template class ODBC_API Error<SQLHSTMT, SQL_HANDLE_STMT>;
extern template class ODBC_API Error<SQLHDESC, SQL_HANDLE_DESC>;
#endif

#endif

using EnvironmentError = Error<SQLHENV, SQL_HANDLE_ENV>;
using ConnectionError = Error<SQLHDBC, SQL_HANDLE_DBC>;
using StatementError = Error<SQLHSTMT, SQL_HANDLE_STMT>;
using DescriptorError = Error<SQLHSTMT, SQL_HANDLE_DESC>;


} } } // namespace Poco::Data::ODBC


#endif
