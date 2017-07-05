//
// PostgreSQLException.h
//
// $Id: //poco/1.5/Data/PostgreSQL/include/Poco/Data/PostgreSQL/PostgreSQLException.h#1 $
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


#ifndef Data_PostgreSQL_PostgreSQLException_INCLUDED
#define Data_PostgreSQL_PostgreSQLException_INCLUDED

#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/DataException.h"

#include <typeinfo>
#include <string>

namespace Poco {
namespace Data {
namespace PostgreSQL {

// End-user include this file and use in code ConnectionException/StatementException
// So it need not know 

class PostgreSQL_API PostgreSQLException: public Poco::Data::DataException
	/// Base class for all PostgreSQL exceptions
{
public:

	PostgreSQLException(const std::string& aMessage);
		/// Creates PostgreSQLException.

	PostgreSQLException(const PostgreSQLException& exc);
		/// Creates PostgreSQLException.

	~PostgreSQLException() throw();
		/// Destroys PostgreSQLexception.

	PostgreSQLException& operator=(const PostgreSQLException& exc);
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


class ConnectionException : public PostgreSQLException
	/// ConnectionException
{
public:

	ConnectionException(const std::string& aMessage);
		/// Creates ConnectionException from string.

};


class TransactionException : public ConnectionException
	/// TrabsactionException
{
public:

	TransactionException(const std::string& aMessage);
		/// Creates TransactionException from string.
};


class StatementException : public PostgreSQLException
	/// StatementException
{
public:

	StatementException(const std::string& aMessage);
		/// Creates StatementException from string.
};


//
// inlines
//

inline PostgreSQLException& PostgreSQLException::operator=(const PostgreSQLException& exc)
{
	Poco::Data::DataException::operator=(exc);
	return *this;
}

inline const char* PostgreSQLException::name() const throw()
{
	return "PostgreSQL";
}

inline const char* PostgreSQLException::className() const throw()
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


} } } // namespace Poco::Data::PostgreSQL

#endif //Data_PostgreSQL_PostgreSQLException_INCLUDED
