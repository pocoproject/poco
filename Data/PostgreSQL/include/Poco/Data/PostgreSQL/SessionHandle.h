//
// SesssionHandle.h
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  SessionHandle
//
// Definition of the SessionHandle class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_PostgreSQL_SessionHandle_INCLUDED
#define SQL_PostgreSQL_SessionHandle_INCLUDED


#include "Poco/Mutex.h"
#include "Poco/Types.h"
#include <map>
#include <string>
#include <vector>
#include <libpq-fe.h>


namespace Poco {
namespace Data {
namespace PostgreSQL {


class SessionParameters
	/// PostgreSQL session parameters
{
public:
	enum HowToDisplay
	{
		HTD_ASIS,   // as is
		HTD_HIDE,   // do not display (e.g. passwords)
		HID_DEBUG   // debug use only
	};

	SessionParameters(const std::string& aKeyword,
		const std::string& anEnvironmentVariable,
		const std::string& aCompiledDefault,
		const std::string& aCurrentValue,
		const std::string& aDisplayLabel,
		const std::string& aHowToDisplay,
		int aDisplaySize);

	~SessionParameters();

	std::string keyword() const;
	std::string enviromentVariable() const;
	std::string compiledDefault() const;
	std::string currentValue() const;
	std::string displayLabel() const;
	HowToDisplay howToDisplay() const;
	int displaySize() const;

private:
	std::string _keyword;             // The keyword of the option
	std::string _environmentVariable; // Fallback environment variable name
	std::string _compiledDefault;     // Fallback compiled in default value
	std::string _currentValue;        // Option's current value, or NULL
	std::string _displayLabel;        // Label for field in a connect dialog
	HowToDisplay _howToDisplay;       // Indicates how to display this field
	int _displaySize;                 // Field size in characters for connect dialog
};


using SessionParametersMap = std::map<std::string, SessionParameters>;


class SessionHandle
	/// PostgreSQL connection(session) handle
{
public:
	explicit SessionHandle();
		/// Creates session handle

	~SessionHandle();
		/// Destroy handle, close connection

	void connect(const std::string& aConnectionString);
		/// Connect to server

	void connect(const char* aConnectionString);

	void connect(const char* aHost, const char* aUser, const char* aPassword, const char* aDatabase, unsigned short aPort, unsigned int aConnectionTimeout);

	bool isConnected() const;
		/// is a connection established?

	void disconnect();
		/// Close connection

	bool reset();
		/// reset the connection

	std::string lastError() const;
		/// last error on the connection

	void startTransaction();
		/// Start transaction

	bool isTransaction() const;
		/// Returns true iff a transaction is a transaction is in progress, false otherwise.

	void commit();
		/// Commit transaction

	void rollback();
		/// Rollback trabsaction

	bool isAutoCommit() const;
		/// is the connection in auto commit mode?

	void autoCommit(bool val);
		/// is the connection in auto commit mode?

	bool isAsynchronousCommit() const;
		/// is the connection in Asynchronous commit mode?

	void setAsynchronousCommit(bool aShouldAsynchronousCommit = true);
		/// is the connection in Asynchronous commit mode?

	void cancel();
		/// Attempts to cancel in-process statements

	void setTransactionIsolation(Poco::UInt32 aTI);
		/// Sets the transaction isolation level.

	Poco::UInt32 transactionIsolation() const;
		/// Returns the transaction isolation level.

	static bool hasTransactionIsolation(Poco::UInt32 aTI);
		/// Returns true iff the transaction isolation level corresponding
		/// to the supplied bitmask is supported.

	void deallocatePreparedStatement(const std::string& aPreparedStatementToDeAllocate);
		/// deallocates a previously prepared statement

	int serverVersion() const;
		/// remote server version

	int serverProcessID() const;
		/// the process ID of the remotee server process

	int protocoVersion() const;
		/// the protocol version between the client and the server

	std::string clientEncoding() const;
		/// returns the client encoding

	std::string parameterStatus(const std::string& param) const;
		/// Returns the value configured on the server for the given parameter.

	int libpqVersion() const;
		/// returns the version of libpq

	static SessionParametersMap connectionDefaultParameters();
		/// returns the default parameters used on a connection

	SessionParametersMap connectionParameters() const;
		/// returns the parameters used on the connection

	std::string connectionString() const;
		/// returns the string used to connect

	operator PGconn* ();
		/// Get the PostgreSQL connection pointer

	Poco::FastMutex& mutex();
		/// Get the sessionHandle mutex to protect the connection pointer

private:
	static SessionParametersMap setConnectionInfoParameters(PQconninfoOption* aConnectionInfoOptionsPtr);

	void deallocateStoredPreparedStatements();

	void deallocatePreparedStatementNoLock(const std::string& aPreparedStatementToDeAllocate);
	bool isConnectedNoLock() const;
	std::string lastErrorNoLock() const;

	SessionHandle(const SessionHandle&);
	SessionHandle& operator= (const SessionHandle&);

private:
	mutable Poco::FastMutex   _sessionMutex;
	PGconn*                   _pConnection;
	std::string               _connectionString;
	bool                      _inTransaction;
	bool                      _isAsynchronousCommit;
	Poco::UInt32              _tranactionIsolationLevel;
	std::vector <std::string> _preparedStatementsToBeDeallocated;

//	static const std::string POSTGRESQL_READ_UNCOMMITTED;  // NOT SUPPORTED
	static const std::string POSTGRESQL_READ_COMMITTED;
	static const std::string POSTGRESQL_REPEATABLE_READ;
	static const std::string POSTGRESQL_SERIALIZABLE;
};


//
// inlines
//


inline SessionParameters::SessionParameters(const std::string& aKeyword,
	const std::string& anEnvironmentVariable,
	const std::string& aCompiledDefault,
	const std::string& aCurrentValue,
	const std::string& aDisplayLabel,
	const std::string& aHowToDisplay,
	int aDisplaySize):_keyword(aKeyword),
		_environmentVariable(anEnvironmentVariable),
		_compiledDefault(aCompiledDefault),
		_currentValue(aCurrentValue),
		_displayLabel(aDisplayLabel),
		_howToDisplay(HTD_ASIS),
		_displaySize(aDisplaySize)
{
	if (aHowToDisplay == "*")
	{
		_howToDisplay = HTD_HIDE;
	}

	if (aHowToDisplay == "D")
	{
		_howToDisplay = HID_DEBUG;
	}
}


inline SessionParameters::~SessionParameters()
{
}


inline std::string SessionParameters::keyword() const
{
	return _keyword;
}


inline std::string SessionParameters::enviromentVariable() const
{
	return _environmentVariable;
}


inline std::string SessionParameters::compiledDefault() const
{
	return _compiledDefault;
}


inline std::string SessionParameters::currentValue() const
{
	return _currentValue;
}


inline std::string SessionParameters::displayLabel() const
{
	return _displayLabel;
}


inline SessionParameters::HowToDisplay SessionParameters::howToDisplay() const
{
	return _howToDisplay;
}

inline int SessionParameters::displaySize() const
{
	return _displaySize;
}


inline SessionHandle::operator PGconn * ()
{
	return _pConnection;
}


inline Poco::FastMutex& SessionHandle::mutex()
{
	return _sessionMutex;
}


inline std::string SessionHandle::connectionString() const
{
	return _connectionString;
}


inline bool SessionHandle::isTransaction() const
{
	return _inTransaction;
}


inline bool SessionHandle::isAsynchronousCommit() const
{
	return _isAsynchronousCommit;
}


} } } // namespace Poco::Data::PostgreSQL


#endif // SQL_PostgreSQL_SessionHandle_INCLUDED
