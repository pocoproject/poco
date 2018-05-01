//
// SessionImpl.cpp
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  SessionImpl
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/PostgreSQL/SessionImpl.h"
#include "Poco/SQL/PostgreSQL/PostgreSQLException.h"
#include "Poco/SQL/PostgreSQL/PostgreSQLStatementImpl.h"
#include "Poco/SQL/PostgreSQL/PostgreSQLTypes.h"
#include "Poco/SQL/Session.h"
#include "Poco/NumberParser.h"
#include "Poco/String.h"

#include <map>

namespace
{
	std::string copyStripped(std::string::const_iterator aFromStringCItr, std::string::const_iterator aToStringCItr)
	{
		// skip leading spaces
		while ((aFromStringCItr != aToStringCItr) && isspace(*aFromStringCItr)) aFromStringCItr++;
		// skip trailing spaces
		while ((aFromStringCItr != aToStringCItr) && isspace(*(aToStringCItr - 1))) aToStringCItr--;

		return std::string(aFromStringCItr, aToStringCItr);
	}

	std::string createConnectionStringFromOptionsMap(const std::map <std::string, std::string> anOptionsMap)
	{
		std::string connectionString;

		for (std::map<std::string, std::string>::const_iterator citr = anOptionsMap.begin(); citr != anOptionsMap.end(); ++citr)
		{
			connectionString.append(citr->first);
			connectionString.append("=");
			connectionString.append(citr->second);
			connectionString.append(" ");
		}

		return connectionString;
	}
}


namespace Poco {
namespace SQL {
namespace PostgreSQL {


SessionImpl::SessionImpl(const std::string& aConnectionString, std::size_t aLoginTimeout):
	Poco::SQL::AbstractSessionImpl<SessionImpl>(aConnectionString, aLoginTimeout)
{
	setProperty("handle", static_cast<SessionHandle*>(&_sessionHandle));
	setConnectionTimeout(CONNECTION_TIMEOUT_DEFAULT);
	open();
}


SessionImpl::~SessionImpl()
{
	try
	{
		close();
	}
	catch (...)
	{
	}
}


void SessionImpl::setConnectionTimeout(std::size_t aTimeout)
{
	_timeout = aTimeout;
}


void SessionImpl::open(const std::string& aConnectionString)
{
	if (connectionString() != aConnectionString)
	{
		if (isConnected())
		{
			throw ConnectionException("Session already connected");
		}

		if (! aConnectionString.empty())
		{
			setConnectionString(aConnectionString);
		}
	}

	poco_assert_dbg (! connectionString().empty());

	unsigned int timeout = static_cast<unsigned int>(getLoginTimeout());

	// PostgreSQL connections can use environment variables for connection parameters.
	// As such it is not an error if they are not part of the connection string

	std::map <std::string, std::string> optionsMap;

	// Default values
	optionsMap["connect_timeout"] = Poco::NumberFormatter::format(timeout);

	const std::string& connString = connectionString();

	for (std::string::const_iterator start = connString.begin();;)
	{
		std::string::const_iterator finish = std::find(start, connString.end(), ' '); // space is the separator between keyword=value pairs
		std::string::const_iterator middle = std::find(start, finish, '=');

		if (middle == finish)
		{
			throw PostgreSQLException("create session: bad connection string format, cannot find '='");
		}

		optionsMap[ copyStripped(start, middle) ] = copyStripped(middle + 1, finish);

		if ((finish == connString.end()) || (finish + 1 == connString.end())) break;

		start = finish + 1;
	}

	// Real connect
	_sessionHandle.connect(createConnectionStringFromOptionsMap(optionsMap));

	addFeature("autoCommit",
		&SessionImpl::setAutoCommit,
		&SessionImpl::isAutoCommit);

	addFeature("asynchronousCommit",
		&SessionImpl::setAutoCommit,
		&SessionImpl::isAutoCommit);
}


void SessionImpl::close()
{
	if (isConnected())
	{
		_sessionHandle.disconnect();
	}
}


bool SessionImpl::isConnected() const
{
	return _sessionHandle.isConnected();
}


StatementImpl::Ptr SessionImpl::createStatementImpl()
{
	return new PostgreSQLStatementImpl (*this);
}


bool SessionImpl::isTransaction() const
{
	return _sessionHandle.isTransaction();
}


void SessionImpl::begin()
{
	if (isTransaction())
	{
		throw Poco::InvalidAccessException("Already in transaction.");
	}

	_sessionHandle.startTransaction();

}


 void SessionImpl::commit()
{
	// Not an error to issue a COMMIT without a preceding BEGIN
	_sessionHandle.commit();
}


void SessionImpl::rollback()
{
	// Not an error to issue a ROLLBACK without a preceding BEGIN
	_sessionHandle.rollback();
}


void SessionImpl::setAutoCommit(const std::string&, bool aValue)
{
	_sessionHandle.setAutoCommit(aValue);
}


bool SessionImpl::isAutoCommit(const std::string&) const
{
	return _sessionHandle.isAutoCommit();
}


void SessionImpl::setAsynchronousCommit(const std::string&,  bool aValue)
{
	_sessionHandle.setAsynchronousCommit(aValue);
}


bool SessionImpl::isAsynchronousCommit(const std::string&) const
{
	return _sessionHandle.isAsynchronousCommit();
}


void SessionImpl::setTransactionIsolation(Poco::UInt32 aTI)
{
	return _sessionHandle.setTransactionIsolation(aTI);
}


Poco::UInt32 SessionImpl::getTransactionIsolation() const
{
	return _sessionHandle.transactionIsolation();
}


bool SessionImpl::hasTransactionIsolation(Poco::UInt32 aTI) const
{
	return _sessionHandle.hasTransactionIsolation(aTI);
}


}}} // namespace Poco::SQL::PostgreSQL
