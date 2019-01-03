//
// PostgreSQLException.cpp
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  PostgreSQLException
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/PostgreSQL/PostgreSQLException.h"

namespace Poco {
namespace SQL {
namespace PostgreSQL {


PostgreSQLException::PostgreSQLException(const std::string& aMessage)
: Poco::SQL::SQLException(std::string("[PostgreSQL]: ") + aMessage)
{
}


PostgreSQLException::PostgreSQLException(const PostgreSQLException& anException)
: Poco::SQL::SQLException(anException)
{
}


PostgreSQLException::~PostgreSQLException() throw()
{
}


//
// ConnectionException
//


ConnectionException::ConnectionException(const std::string& aMessage)
: PostgreSQLException(aMessage)
{
}


//
// TransactionException
//

TransactionException::TransactionException(const std::string& aMessage)
: ConnectionException(aMessage)
{
}


//
// StatementException
//


StatementException::StatementException(const std::string& aMessage)
: PostgreSQLException(aMessage)
{
}


} } } // namespace Poco::SQL::PostgreSQL
