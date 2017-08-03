//
// SessionImpl.h
//
// $Id: //poco/1.5/Data/PostgreSQL/include/Poco/Data/PostgreSQL/SessionImpl.h#1 $
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  SessionImpl
//
// Definition of the SessionImpl class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_PostgreSQL_SessionImpl_INCLUDED
#define Data_PostgreSQL_SessionImpl_INCLUDED


#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/PostgreSQL/SessionHandle.h"

#include "Poco/Data/AbstractSessionImpl.h"

#include <string>

namespace Poco {
namespace Data {
namespace PostgreSQL {


class PostgreSQL_API SessionImpl: public Poco::Data::AbstractSessionImpl<SessionImpl>
	/// Implements SessionImpl interface
{
public:

	SessionImpl(const std::string& aConnectionString,
				std::size_t aLoginTimeout = LOGIN_TIMEOUT_DEFAULT);
		/// Creates the SessionImpl. Opens a connection to the database
		///
		/// Connection string format:
		/// <str> == <assignment> | <assignment> ' ' <str>
		/// <assignment> == <name> '=' <value>
		/// <name> == 'host' | 'port' | 'user' | 'password' | 'dbname' | 'connect_timeout'
		/// <value> == [~;]*
		///
		/// consult postgres documentation for other parameters

	~SessionImpl();
		/// Destroys the SessionImpl.

	void setConnectionTimeout(std::size_t aTimeout);
		/// Sets the session connection timeout value.

	std::size_t getConnectionTimeout();
		/// Returns the session connection timeout value.

	void open(const std::string& aConnectionString = std::string());
		/// Opens a connection to the database.

	void close();
		/// Closes the connection.

	bool isConnected();
		/// Returns true if connected, false otherwise.

	Poco::Data::StatementImpl* createStatementImpl();
		/// Returns an PostgreSQL StatementImpl

	void begin();
		/// Starts a transaction

	void commit();
		/// Commits and ends a transaction		

	void rollback();
		/// Aborts a transaction

	bool canTransact();
		/// Returns true if session has transaction capabilities.

	bool isTransaction();
		/// Returns true iff a transaction is a transaction is in progress, false otherwise.

	void setTransactionIsolation(Poco::UInt32 aTI);
		/// Sets the transaction isolation level.

	Poco::UInt32 getTransactionIsolation();
		/// Returns the transaction isolation level.

	bool hasTransactionIsolation(Poco::UInt32 aTI);
		/// Returns true iff the transaction isolation level corresponding
		/// to the supplied bitmask is supported.

	bool isTransactionIsolation(Poco::UInt32 aTI);
		/// Returns true iff the transaction isolation level corresponds
		/// to the supplied bitmask.

	void setAutoCommit(const std::string&, bool aValue);
		/// Sets autocommit property for the session.

	bool isAutoCommit(const std::string& aName = std::string());
		/// Returns autocommit property value.

	void setAsynchronousCommit(const std::string&, bool aValue);
		/// Sets asynchronousCommit property for the session.

	bool isAsynchronousCommit(const std::string& aName = std::string());
		/// is the connection in Asynchronous commit mode?

	SessionHandle& handle();
		/// Get handle

	const std::string& connectorName() const;
		/// Returns the name of the connector.

private:
	std::string		_connectorName;
	SessionHandle	_sessionHandle;

	std::size_t		_timeout;
};


//
// inlines
//
inline
bool
SessionImpl::canTransact()
{
	return true;
}

inline SessionHandle& SessionImpl::handle()
{
	return _sessionHandle;
}

inline const std::string& SessionImpl::connectorName() const
{
	return _connectorName;
}


inline bool SessionImpl::isTransactionIsolation(Poco::UInt32 aTI)
{
	return getTransactionIsolation() == aTI;
}
	

inline std::size_t SessionImpl::getConnectionTimeout()
{
	return _timeout;
}


} } } // namespace Poco::Data::PostgreSQL


#endif // Data_PostgreSQL_SessionImpl_INCLUDED
