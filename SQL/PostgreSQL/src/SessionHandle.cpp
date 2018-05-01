//
// SessionHandle.cpp
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  SessionHandle
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/PostgreSQL/SessionHandle.h"
#include "Poco/SQL/PostgreSQL/PostgreSQLException.h"
#include "Poco/SQL/PostgreSQL/PostgreSQLTypes.h"
#include "Poco/SQL/Session.h"
#include "Poco/NumberFormatter.h"


#define POCO_POSTGRESQL_VERSION_NUMBER ((NDB_VERSION_MAJOR<<16) | (NDB_VERSION_MINOR<<8) | (NDB_VERSION_BUILD&0xFF))


namespace Poco {
namespace SQL {
namespace PostgreSQL {


//const std::string SessionHandle::POSTGRESQL_READ_UNCOMMITTED = "READ UNCOMMITTED";
const std::string SessionHandle::POSTGRESQL_READ_COMMITTED  = "READ COMMITTED";
const std::string SessionHandle::POSTGRESQL_REPEATABLE_READ = "REPEATABLE READ";
const std::string SessionHandle::POSTGRESQL_SERIALIZABLE	= "SERIALIZABLE";


SessionHandle::SessionHandle(): _pConnection(0),
	_inTransaction(false),
	_isAutoCommit(true),
	_isAsynchronousCommit(false),
	_tranactionIsolationLevel(Session::TRANSACTION_READ_COMMITTED)
{
}

SessionHandle::~SessionHandle()
{
	try
	{
		disconnect();
	}
	catch (...)
	{
	}
}


bool SessionHandle::isConnected() const
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	return isConnectedNoLock();
}


bool SessionHandle::isConnectedNoLock() const
{
	// DO NOT ACQUIRE THE MUTEX IN PRIVATE METHODS

	if (_pConnection
		&& PQstatus(_pConnection) == CONNECTION_OK)
	{
		return true;
	}

	return false;
}


void SessionHandle::connect(const std::string& aConnectionString)
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (isConnectedNoLock())
	{
		throw ConnectionFailedException("Already Connected");
	}

	_pConnection = PQconnectdb(aConnectionString.c_str());


	if (! isConnectedNoLock())
	{
		throw ConnectionFailedException(std::string("Connection Error: ") + lastErrorNoLock());
	}

	_connectionString = aConnectionString;
}


void SessionHandle::connect(const char* aConnectionString)
{
	connect(std::string(aConnectionString));
}


void SessionHandle::connect(const char* aHost, const char* aUser, const char* aPassword,
	const char* aDatabase, 	unsigned short aPort, unsigned int aConnectionTimeout)
{
	std::string connectionString;

	connectionString.append("host=");
	connectionString.append(aHost);
	connectionString.append(" ");

	connectionString.append("user=");
	connectionString.append(aUser);
	connectionString.append(" ");

	connectionString.append("password=");
	connectionString.append(aPassword);
	connectionString.append(" ");

	connectionString.append("dbname=");
	connectionString.append(aDatabase);
	connectionString.append(" ");

	connectionString.append("port=");
	Poco::NumberFormatter::append(connectionString, aPort);
	connectionString.append(" ");

	connectionString.append("connect_timeout=");
	Poco::NumberFormatter::append(connectionString, aConnectionTimeout);

	connect(connectionString);
}


void SessionHandle::disconnect()
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (isConnectedNoLock())
	{
		PQfinish(_pConnection);

		_pConnection = 0;

		_connectionString = std::string();
		_inTransaction= false;
		_isAutoCommit = true;
		_isAsynchronousCommit = false;
		_tranactionIsolationLevel = Session::TRANSACTION_READ_COMMITTED;
	}
}

// TODO: Figure out what happens if a connection is reset with a pending transaction
bool SessionHandle::reset()
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (_pConnection)
	{
		PQreset(_pConnection);
	}

	if (isConnectedNoLock())
	{
		return true;
	}

	return false;
}


std::string SessionHandle::lastError() const
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		return std::string();
	}

	return lastErrorNoLock();
}


std::string SessionHandle::lastErrorNoLock() const
{
	// DO NOT ACQUIRE THE MUTEX IN PRIVATE METHODS
	std::string lastErrorString (0 != _pConnection ? PQerrorMessage(_pConnection) : "not connected");

	return lastErrorString;
}


void SessionHandle::startTransaction()
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	if (_inTransaction)
	{
		return; // NO-OP
	}

	PGresult* pPQResult = PQexec(_pConnection, "BEGIN");

	PQResultClear resultClearer(pPQResult);

	if (PQresultStatus(pPQResult) != PGRES_COMMAND_OK)
	{
		throw StatementException(std::string("BEGIN statement failed:: ") + lastErrorNoLock());
	}

	_inTransaction = true;
}


void SessionHandle::commit()
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	PGresult* pPQResult = PQexec(_pConnection, "COMMIT");

	PQResultClear resultClearer(pPQResult);

	if (PQresultStatus(pPQResult) != PGRES_COMMAND_OK)
	{
		throw StatementException(std::string("COMMIT statement failed:: ") + lastErrorNoLock());
	}

	_inTransaction = false;

	deallocateStoredPreparedStatements();
}


void SessionHandle::rollback()
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	PGresult* pPQResult = PQexec(_pConnection, "ROLLBACK");

	PQResultClear resultClearer(pPQResult);

	if (PQresultStatus(pPQResult) != PGRES_COMMAND_OK)
	{
		throw StatementException(std::string("ROLLBACK statement failed:: ") + lastErrorNoLock());
	}

	_inTransaction = false;

	deallocateStoredPreparedStatements();
}


void SessionHandle::setAutoCommit(bool aShouldAutoCommit)
{
	if (aShouldAutoCommit == _isAutoCommit)
	{
		return;
	}

	if (aShouldAutoCommit)
	{
		commit();  // end any in process transaction
	}
	else
	{
		startTransaction();  // start a new transaction
	}

	_isAutoCommit = aShouldAutoCommit;
}


void SessionHandle::setAsynchronousCommit(bool aShouldAsynchronousCommit)
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	if (aShouldAsynchronousCommit == _isAsynchronousCommit)
	{
		return;
	}

	PGresult* pPQResult = PQexec(_pConnection, aShouldAsynchronousCommit ? "SET SYNCHRONOUS COMMIT TO OFF" : "SET SYNCHRONOUS COMMIT TO ON");

	PQResultClear resultClearer(pPQResult);

	if (PQresultStatus(pPQResult) != PGRES_COMMAND_OK)
	{
		throw StatementException(std::string("SET SYNCHRONUS COMMIT statement failed:: ") + lastErrorNoLock());
	}

	_isAsynchronousCommit = aShouldAsynchronousCommit;
}


void SessionHandle::cancel()
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	PGcancel* ptrPGCancel = PQgetCancel(_pConnection);

	PGCancelFree cancelFreer(ptrPGCancel);

	PQcancel(ptrPGCancel, 0, 0); // no error buffer
}


void SessionHandle::setTransactionIsolation(Poco::UInt32 aTI)
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	if (aTI == _tranactionIsolationLevel)
	{
		return;
	}

	if (! hasTransactionIsolation(aTI))
	{
		throw Poco::InvalidArgumentException("setTransactionIsolation()");
	}

	std::string isolationLevel;

	switch (aTI)
	{
		case Session::TRANSACTION_READ_COMMITTED:
			isolationLevel = POSTGRESQL_READ_COMMITTED; break;
		case Session::TRANSACTION_REPEATABLE_READ:
			isolationLevel = POSTGRESQL_REPEATABLE_READ; break;
		case Session::TRANSACTION_SERIALIZABLE:
			isolationLevel = POSTGRESQL_SERIALIZABLE; break;
	}

	PGresult* pPQResult = PQexec(_pConnection, Poco::format("SET SESSION CHARACTERISTICS AS TRANSACTION ISOLATION LEVEL %s", isolationLevel).c_str());

	PQResultClear resultClearer(pPQResult);

	if (PQresultStatus(pPQResult) != PGRES_COMMAND_OK)
	{
		throw StatementException(std::string("set transaction isolation statement failed: ") + lastErrorNoLock());
	}

	_tranactionIsolationLevel = aTI;
}


Poco::UInt32
SessionHandle::transactionIsolation()
{
	return _tranactionIsolationLevel;
}


bool SessionHandle::hasTransactionIsolation(Poco::UInt32 aTI)
{
	return Session::TRANSACTION_READ_COMMITTED	== aTI
		|| Session::TRANSACTION_REPEATABLE_READ	== aTI
		|| Session::TRANSACTION_SERIALIZABLE	== aTI;
}


void SessionHandle::deallocatePreparedStatement(const std::string& aPreparedStatementToDeAllocate)
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	if (! _inTransaction)
	{
		deallocatePreparedStatementNoLock(aPreparedStatementToDeAllocate);
	}
	else
	{
		try
		{
			_preparedStatementsToBeDeallocated.push_back(aPreparedStatementToDeAllocate);
		}
		catch (std::bad_alloc&)
		{
		}
	}
}


void SessionHandle::deallocatePreparedStatementNoLock(const std::string& aPreparedStatementToDeAllocate)
{
	PGresult* pPQResult = PQexec(_pConnection, (std::string("DEALLOCATE ") + aPreparedStatementToDeAllocate).c_str());
	
	PQResultClear resultClearer(pPQResult);
	
	if (PQresultStatus(pPQResult) != PGRES_COMMAND_OK)
	{
		throw StatementException(std::string("DEALLOCATE statement failed: ") + lastErrorNoLock());
	}
}


void SessionHandle::deallocateStoredPreparedStatements()
{
	// DO NOT ACQUIRE THE MUTEX IN PRIVATE METHODS
	while (! _preparedStatementsToBeDeallocated.empty())
	{
		deallocatePreparedStatementNoLock(_preparedStatementsToBeDeallocated.back());

		_preparedStatementsToBeDeallocated.pop_back();
	}
}


int SessionHandle::serverVersion() const
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	return PQserverVersion(_pConnection);
}


int SessionHandle::serverProcessID() const
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	return PQbackendPID(_pConnection);
}


int SessionHandle::protocoVersion() const
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	return PQprotocolVersion(_pConnection);
}


std::string SessionHandle::clientEncoding() const
{
	Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

	if (! isConnectedNoLock())
	{
		throw NotConnectedException();
	}

	return pg_encoding_to_char(PQclientEncoding(_pConnection));
}


int SessionHandle::libpqVersion() const
{
	return PQlibVersion();
}


SessionParametersMap SessionHandle::setConnectionInfoParameters(PQconninfoOption* pConnInfOpt)
{
	SessionParametersMap sessionParametersMap;

	while (0 != pConnInfOpt->keyword)
	{
		try
		{
			std::string keyword						= pConnInfOpt->keyword  ? pConnInfOpt->keyword  : std::string();
			std::string environmentVariableVersion	= pConnInfOpt->envvar   ? pConnInfOpt->envvar   : std::string();
			std::string compiledVersion				= pConnInfOpt->compiled ? pConnInfOpt->compiled : std::string();
			std::string currentValue				= pConnInfOpt->val  ? pConnInfOpt->val  : std::string();
			std::string dialogLabel					= pConnInfOpt->label? pConnInfOpt->label: std::string();
			std::string dialogDisplayCharacter		= pConnInfOpt->dispchar ? pConnInfOpt->dispchar : std::string();
			int dialogDisplaysize					= pConnInfOpt->dispsize;

			SessionParameters connParams(keyword, environmentVariableVersion, compiledVersion,
				currentValue, dialogLabel, dialogDisplayCharacter, dialogDisplaysize);

			sessionParametersMap.insert(SessionParametersMap::value_type(connParams.keyword(), connParams));
		}
		catch (std::bad_alloc&)
		{
		}

		++pConnInfOpt;
	}

	return sessionParametersMap;
}


SessionParametersMap SessionHandle::connectionDefaultParameters()
{
	PQconninfoOption* ptrConnInfoOptions = PQconndefaults();

	PQConnectionInfoOptionsFree connectionOptionsFreeer(ptrConnInfoOptions);

	return setConnectionInfoParameters(ptrConnInfoOptions);
}


SessionParametersMap SessionHandle::connectionParameters() const
{
	if (! isConnected())
	{
		throw NotConnectedException();
	}

	PQconninfoOption* ptrConnInfoOptions = 0;
	{
		Poco::FastMutex::ScopedLock mutexLocker(_sessionMutex);

		ptrConnInfoOptions = PQconninfo(_pConnection);
	}

	PQConnectionInfoOptionsFree connectionOptionsFreeer(ptrConnInfoOptions);

	return setConnectionInfoParameters(ptrConnInfoOptions);
}


}}} // Poco::SQL::PostgreSQL
