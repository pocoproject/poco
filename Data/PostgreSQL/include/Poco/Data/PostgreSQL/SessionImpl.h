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

	~SessionImpl() override;
		/// Destroys the SessionImpl.

	void setConnectionTimeout(std::size_t aTimeout) override;
		/// Sets the session connection timeout value.

	[[nodiscard]]
	std::size_t getConnectionTimeout() const override;
		/// Returns the session connection timeout value.

	void open(const std::string& aConnectionString = std::string()) override;
		/// Opens a connection to the database.

	void close() override;
		/// Closes the connection.

	void reset() override;
		/// Do nothing

	[[nodiscard]]
	bool isConnected() const override;
		/// Returns true if connected, false otherwise.

	[[nodiscard]]
	Poco::Data::StatementImpl::Ptr createStatementImpl() override;
		/// Returns an PostgreSQL StatementImpl

	void begin() override;
		/// Starts a transaction

	void commit() override;
		/// Commits and ends a transaction

	void rollback() override;
		/// Aborts a transaction

	[[nodiscard]]
	bool canTransact() const override;
		/// Returns true if session has transaction capabilities.

	[[nodiscard]]
	bool isTransaction() const override;
		/// Returns true iff a transaction is a transaction is in progress, false otherwise.

	void setTransactionIsolation(Poco::UInt32 aTI) override;
		/// Sets the transaction isolation level.

	[[nodiscard]]
	Poco::UInt32 getTransactionIsolation() const override;
		/// Returns the transaction isolation level.

	[[nodiscard]]
	bool hasTransactionIsolation(Poco::UInt32 aTI) const override;
		/// Returns true iff the transaction isolation level corresponding
		/// to the supplied bitmask is supported.

	[[nodiscard]]
	bool isTransactionIsolation(Poco::UInt32 aTI) const override;
		/// Returns true iff the transaction isolation level corresponds
		/// to the supplied bitmask.

	void autoCommit(const std::string&, bool aValue);
		/// Sets autocommit property for the session.

	[[nodiscard]]
	bool isAutoCommit(const std::string& aName = std::string()) const;
		/// Returns autocommit property value.

	void setAsynchronousCommit(const std::string&, bool aValue);
		/// Sets asynchronousCommit property for the session.

	[[nodiscard]]
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

	[[nodiscard]]
	bool isBinaryExtraction(const std::string& feature = std::string()) const;
		/// Returns true if binary extraction is enabled, otherwise false.
		/// See setBinaryExtraction() for more information.

	[[nodiscard]]
	SessionHandle& handle();
		/// Get handle

	[[nodiscard]]
	const std::string& connectorName() const override;
		/// Returns the name of the connector.

	void setName();

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
