//
// MySQLException.h
//
// Library: Data/MySQL
// Package: MySQL
// Module:  MySQLException
//
// Definition of the MySQLException class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_MySQL_MySQLException_INCLUDED
#define SQL_MySQL_MySQLException_INCLUDED

#include "Poco/SQL/MySQL/MySQL.h"
#include "Poco/SQL/SQLException.h"
#include <typeinfo>
#include <string>


typedef struct st_mysql MYSQL;
typedef struct st_mysql_stmt MYSQL_STMT;


namespace Poco {
namespace SQL {
namespace MySQL {

// End-user include this file and use in code ConnectionException/StatementException
// So it need not know

class MySQL_API MySQLException: public Poco::SQL::SQLException
	/// Base class for all MySQL exceptions
{
public:

	MySQLException(const std::string& msg, int errCode = 0);
		/// Creates MySQLException.

	MySQLException(const MySQLException& exc);
		/// Creates MySQLException.

	~MySQLException() throw();
		/// Destroys MySQLexception.

	MySQLException& operator=(const MySQLException& exc);
		/// Assignment operator.

	const char* name() const throw();
		/// Returns exception name.

	const char* className() const throw();
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
};


class ConnectionException : public MySQLException
	/// ConnectionException
{
public:

	ConnectionException(const std::string& msg);
		/// Creates ConnectionException from string.

	ConnectionException(const std::string& text, MYSQL* h);
		/// Creates ConnectionException from string and handle.

private:

	static std::string compose(const std::string& text, MYSQL* h);

};


class TransactionException : public ConnectionException
	/// TransactionException
{
public:

	TransactionException(const std::string& msg);
		/// Creates TransactionException from string.

	TransactionException(const std::string& text, MYSQL* h);
		/// Creates TransactionException from string and handle.
};


class StatementException : public MySQLException
	/// StatementException
{
public:

	StatementException(const std::string& msg);
		/// Creates StatementException from string.

	StatementException(const std::string& text, MYSQL_STMT* h, const std::string& stmt = "");
		/// Creates StatementException from string and handle.

private:

	static std::string compose(const std::string& text, MYSQL_STMT* h, const std::string& stmt);
};


//
// inlines
//

inline MySQLException& MySQLException::operator=(const MySQLException& exc)
{
	Poco::SQL::SQLException::operator=(exc);
	return *this;
}

inline const char* MySQLException::name() const throw()
{
	return "MySQL";
}

inline const char* MySQLException::className() const throw()
{
	return typeid(*this).name();
}

inline Poco::Exception* MySQLException::clone() const
{
	return new MySQLException(*this);
}

inline void MySQLException::rethrow() const
{
	throw *this;
}


} } } // namespace Poco::SQL::MySQL

#endif //Data_MySQL_MySQLException_INCLUDED
