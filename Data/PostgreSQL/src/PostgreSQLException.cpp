//
// PostgreSQLException.cpp
//
// $Id: //poco/1.5/Data/PostgreSQL/src/PostgreSQLException.cpp#1 $
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


#include "Poco/Data/PostgreSQL/PostgreSQLException.h"

namespace Poco {
namespace Data {
namespace PostgreSQL {


PostgreSQLException::PostgreSQLException(const std::string& aMessage)
: Poco::Data::DataException(std::string("[PostgreSQL]: ") + aMessage)
{
}


PostgreSQLException::PostgreSQLException(const PostgreSQLException& anException)
: Poco::Data::DataException(anException)
{
}


PostgreSQLException::~PostgreSQLException() throw()
{
}


/////
//
// ConnectionException
//
/////


ConnectionException::ConnectionException(const std::string& aMessage)
: PostgreSQLException(aMessage)
{
}


/////
//
// TransactionException
//
/////

TransactionException::TransactionException(const std::string& aMessage)
: ConnectionException(aMessage)
{
}


/////
//
// StatementException
//
/////


StatementException::StatementException(const std::string& aMessage)
: PostgreSQLException(aMessage)
{
}


} } } // namespace Poco::Data::PostgreSQL
