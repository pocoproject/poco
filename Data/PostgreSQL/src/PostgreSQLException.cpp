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


#include "Poco/Data/PostgreSQL/PostgreSQLException.h"
#include <cstring> 

namespace Poco {
namespace Data {
namespace PostgreSQL {


PostgreSQLException::PostgreSQLException(const std::string& aMessage):
	Poco::Data::DataException(std::string("[PostgreSQL]: ") + aMessage)
{
}

PostgreSQLException::PostgreSQLException(const std::string& aMessage,const char* pAnSqlState):
	Poco::Data::DataException(std::string("[PostgreSQL]: ") + aMessage)
{
        // handle anSqlState
        if (pAnSqlState == nullptr) _sqlState[0] = '\0';
	else
	{
		strncpy(_sqlState,pAnSqlState,5);
                _sqlState[5] = '\0';
	}
	
}


PostgreSQLException::PostgreSQLException(const PostgreSQLException& anException):
	Poco::Data::DataException(anException)
{
}


PostgreSQLException::~PostgreSQLException() noexcept
{
}


//
// ConnectionException
//


ConnectionException::ConnectionException(const std::string& aMessage):
	PostgreSQLException(aMessage)
{
}


//
// TransactionException
//

TransactionException::TransactionException(const std::string& aMessage):
	ConnectionException(aMessage)
{
}


//
// StatementException
//


StatementException::StatementException(const std::string& aMessage):
	PostgreSQLException(aMessage)
{
}

StatementException::StatementException(const std::string& aMessage,const char* pAnSqlState):
	PostgreSQLException(aMessage,pAnSqlState)
{
}




} } } // namespace Poco::Data::PostgreSQL
