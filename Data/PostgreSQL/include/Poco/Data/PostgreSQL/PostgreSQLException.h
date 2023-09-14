//
// PostgreSQLException.h
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  PostgreSQLException
//
// Definition of the PostgreSQLException class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_PostgreSQL_PostgreSQLException_INCLUDED
#define SQL_PostgreSQL_PostgreSQLException_INCLUDED


#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/DataException.h"
#include <typeinfo>
#include <string>


namespace Poco {
namespace Data {
namespace PostgreSQL {


class PostgreSQL_API PostgreSQLException: public Poco::Data::DataException
	/// Base class for all PostgreSQL exceptions
{
public:
	explicit PostgreSQLException(const std::string& aMessage);
		/// Creates PostgreSQLException.


	explicit PostgreSQLException(const std::string& aMessage,const char * pAnSqlState);
		/// Creates PostgreSQLException.


	PostgreSQLException(const PostgreSQLException& exc);
		/// Creates PostgreSQLException.

	~PostgreSQLException() noexcept;
		/// Destroys PostgreSQLexception.

	PostgreSQLException& operator = (const PostgreSQLException& exc);
		/// Assignment operator.

	const char* name() const noexcept;
		/// Returns exception name.

	const char* className() const noexcept;
		/// Returns the name of the exception class.

	Poco::Exception* clone() const;
		/// Creates an exact copy of the exception.
		///
		/// The copy can later be thrown again by
		/// invoking rethrow() on it.

	void rethrow() const;
		/// (Re)Throws the exception.
		///
		/// This is useful for temporarily storing a
		/// copy of an exception (see clone()), then
		/// throwing it again.

	const char* sqlState() const noexcept;
		/// Returns the SqlState


private:
	char _sqlState[6];
};


class ConnectionException: public PostgreSQLException
	/// ConnectionException
{
public:
	ConnectionException(const std::string& aMessage);
		/// Creates ConnectionException from string.
};


class TransactionException: public ConnectionException
	/// TrabsactionException
{
public:
	TransactionException(const std::string& aMessage);
		/// Creates TransactionException from string.
};


class StatementException: public PostgreSQLException
	/// StatementException
{
public:
	StatementException(const std::string& aMessage);
		/// Creates StatementException from string.

	StatementException(const std::string& aMessage,const char* pAnSqlState);
		/// Creates StatementException from string with support for sqlState.

};


//
// inlines
//


inline PostgreSQLException& PostgreSQLException::operator = (const PostgreSQLException& exc)
{
	Poco::Data::DataException::operator = (exc);
	return *this;
}


inline const char* PostgreSQLException::name() const noexcept
{
	return "PostgreSQL";
}


inline const char* PostgreSQLException::className() const noexcept
{
	return typeid(*this).name();
}


inline Poco::Exception* PostgreSQLException::clone() const
{
	return new PostgreSQLException(*this);
}


inline void PostgreSQLException::rethrow() const
{
	throw *this;
}


inline const char* PostgreSQLException::sqlState() const noexcept
{
	return _sqlState;
}



} } } // namespace Poco::Data::PostgreSQL


#endif //SQL_PostgreSQL_PostgreSQLException_INCLUDED
