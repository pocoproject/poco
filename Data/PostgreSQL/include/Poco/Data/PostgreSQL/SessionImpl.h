//
// SessionImpl.h
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


#ifndef SQL_PostgreSQL_SessionImpl_INCLUDED
#define SQL_PostgreSQL_SessionImpl_INCLUDED


#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/PostgreSQL/SessionHandle.h"
#include "Poco/Data/AbstractSessionImpl.h"
#include "Poco/Data/StatementImpl.h"
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

	std::size_t getConnectionTimeout() const;
		/// Returns the session connection timeout value.

	void open(const std::string& aConnectionString = std::string());
		/// Opens a connection to the database.

	void close();
		/// Closes the connection.

	void reset();
		/// Do nothing

	bool isConnected() const;
		/// Returns true if connected, false otherwise.

	Poco::Data::StatementImpl::Ptr createStatementImpl();
		/// Returns an PostgreSQL StatementImpl

	void begin();
		/// Starts a transaction

	void commit();
		/// Commits and ends a transaction

	void rollback();
		/// Aborts a transaction

	bool canTransact() const;
		/// Returns true if session has transaction capabilities.

	bool isTransaction() const;
		/// Returns true iff a transaction is a transaction is in progress, false otherwise.

	void setTransactionIsolation(Poco::UInt32 aTI);
		/// Sets the transaction isolation level.

	Poco::UInt32 getTransactionIsolation() const;
		/// Returns the transaction isolation level.

	bool hasTransactionIsolation(Poco::UInt32 aTI) const;
		/// Returns true iff the transaction isolation level corresponding
		/// to the supplied bitmask is supported.

	bool isTransactionIsolation(Poco::UInt32 aTI) const;
		/// Returns true iff the transaction isolation level corresponds
		/// to the supplied bitmask.

	void autoCommit(const std::string&, bool aValue);
		/// Sets autocommit property for the session.

	bool isAutoCommit(const std::string& aName = std::string()) const;
		/// Returns autocommit property value.

	void setAsynchronousCommit(const std::string&, bool aValue);
		/// Sets asynchronousCommit property for the session.

	bool isAsynchronousCommit(const std::string& aName = std::string()) const;
		/// is the connection in Asynchronous commit mode?

	void setBinaryExtraction(const std::string& feature, bool enabled);
		/// Sets the "binaryExtraction" feature. If set, column values received from
		/// PostgreSQL client will be extracted as binary values. This improves
		/// extraction performance, but will not work with all types.
		///
		/// If not set, all column values will be extracted as strings. This gives
		/// lower performance, but allows to extract also types not supported
		/// directly by Poco::Data.

	bool isBinaryExtraction(const std::string& feature = std::string()) const;
		/// Returns true if binary extraction is enabled, otherwise false.
		/// See setBinaryExtraction() for more information.

	SessionHandle& handle();
		/// Get handle

	const std::string& connectorName() const;
		/// Returns the name of the connector.

private:
	std::string	          _connectorName;
	mutable SessionHandle _sessionHandle;
	std::size_t           _timeout = 0;
	bool                  _binaryExtraction = false;
};


//
// inlines
//


inline bool SessionImpl::canTransact() const
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


inline bool SessionImpl::isTransactionIsolation(Poco::UInt32 aTI) const
{
	return getTransactionIsolation() == aTI;
}


inline std::size_t SessionImpl::getConnectionTimeout() const
{
	return _timeout;
}


inline bool SessionImpl::isBinaryExtraction(const std::string&) const
{
	return _binaryExtraction;
}


} } } // namespace Poco::Data::PostgreSQL


#endif // SQL_PostgreSQL_SessionImpl_INCLUDED
