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


#include "Poco/Data/PostgreSQL/SessionImpl.h"
#include "Poco/Data/PostgreSQL/PostgreSQLException.h"
#include "Poco/Data/PostgreSQL/PostgreSQLStatementImpl.h"
#include "Poco/Data/PostgreSQL/PostgreSQLTypes.h"
#include "Poco/Data/Session.h"
#include "Poco/NumberParser.h"
#include "Poco/String.h"
#include "libpq-fe.h"
#include <map>
#include <memory>


namespace Poco::Data::PostgreSQL {


SessionImpl::SessionImpl(const std::string& aConnectionString, std::size_t aLoginTimeout):
	Poco::Data::AbstractSessionImpl<SessionImpl>(aConnectionString, aLoginTimeout),
	_connectorName("postgresql")
{
	setFeature("sqlParse", false); // the parse currently cannot handle the PostgreSQL placeholders $1, $2, etc.
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


void SessionImpl::setName()
{
	setDBMSName("PostgreSQL"s);
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

		if (!aConnectionString.empty())
		{
			setConnectionString(aConnectionString);
		}
	}

	poco_assert_dbg (!connectionString().empty());

	unsigned int timeout = static_cast<unsigned int>(getLoginTimeout());

	// PostgreSQL connections can use environment variables for connection parameters.
	// As such it is not an error if they are not part of the connection string

	std::map <std::string, std::string> optionsMap;

	// Default values
	optionsMap["connect_timeout"] = Poco::NumberFormatter::format(timeout);

	std::string connString = connectionString();

	// Parse the connection string using libpq's parser
	char* errMsg = nullptr;
	const std::unique_ptr<PQconninfoOption, decltype(&PQconninfoFree)> opts(
		PQconninfoParse(connString.c_str(), &errMsg), &PQconninfoFree);
	std::string errorMsg;
	if (errMsg)
	{
		errorMsg = errMsg;
		PQfreemem(errMsg);
	}
	if (!opts)
	{
		if (errorMsg.empty())
		{
			errorMsg = "Unknown error parsing connection string";
		}
		throw PostgreSQLException("create session: bad connection string format: " + errorMsg);
	}

	// Drop defaults already specified by the caller's connection string
	// so we don't override user-provided values when we re-serialize below.
	for (PQconninfoOption* opt = opts.get(); opt->keyword != nullptr; ++opt)
	{
		if (auto it = optionsMap.find(opt->keyword); it != optionsMap.end() && opt->val != nullptr)
		{
			optionsMap.erase(it);
		}
	}

	char delimiter = ' ';
	const bool isUri = connString.find("postgresql://") == 0
					|| connString.find("postgres://") == 0;
	if (isUri)
	{
		delimiter = (connString.find('?') == std::string::npos)? '?': '&';
	}

	for (const auto& opt : optionsMap)
	{
		connString += delimiter;
		connString += opt.first;
		connString += '=';
		connString += opt.second;

		if (delimiter == '?')
		{
			delimiter = '&';
		}
	}

	// Real connect
	_sessionHandle.connect(connString);

	addFeature("autoCommit",
		&SessionImpl::autoCommit,
		&SessionImpl::isAutoCommit);

	addFeature("asynchronousCommit",
		&SessionImpl::setAutoCommit,
		&SessionImpl::isAutoCommit);

	addFeature("binaryExtraction",
		&SessionImpl::setBinaryExtraction,
		&SessionImpl::isBinaryExtraction);

	setName();
}


void SessionImpl::close()
{
	if (isConnected())
	{
		_sessionHandle.disconnect();
	}
}


void SessionImpl::reset()
{

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


void SessionImpl::autoCommit(const std::string& s, bool val)
{
	if (val != getAutoCommit(s)) {
		_sessionHandle.autoCommit(val);
		AbstractSessionImpl::setAutoCommit(s, val);
	}
}


bool SessionImpl::isAutoCommit(const std::string& s) const
{
	return AbstractSessionImpl::getAutoCommit(s);
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


void SessionImpl::setBinaryExtraction(const std::string& feature, bool enabled)
{
	if (enabled && _sessionHandle.parameterStatus("integer_datetimes") != "on")
		throw PostgreSQLException("binary extraction is not supported with this server (ingeger_datetimes must be enabled on the server)");

	_binaryExtraction = enabled;
}


} // namespace Poco::Data::PostgreSQL
