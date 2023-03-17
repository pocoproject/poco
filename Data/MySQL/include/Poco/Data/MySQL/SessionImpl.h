//
// SessionImpl.h
//
// Library: Data/MySQL
// Package: MySQL
// Module:  SessionImpl
//
// Definition of the SessionImpl class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_MySQL_SessionImpl_INCLUDED
#define Data_MySQL_SessionImpl_INCLUDED


#include "Poco/Data/MySQL/MySQL.h"
#include "Poco/Data/AbstractSessionImpl.h"
#include "Poco/Data/MySQL/SessionHandle.h"
#include "Poco/Data/MySQL/StatementExecutor.h"
#include "Poco/Data/MySQL/ResultMetadata.h"
#include "Poco/SharedPtr.h"
#include "Poco/Mutex.h"


namespace Poco {
namespace Data {
namespace MySQL {


class MySQL_API SessionImpl: public Poco::Data::AbstractSessionImpl<SessionImpl>
	/// Implements SessionImpl interface
{
public:
	static const std::string MYSQL_READ_UNCOMMITTED;
	static const std::string MYSQL_READ_COMMITTED;
	static const std::string MYSQL_REPEATABLE_READ;
	static const std::string MYSQL_SERIALIZABLE;

	SessionImpl(const std::string& connectionString, std::size_t loginTimeout = LOGIN_TIMEOUT_DEFAULT);
		/// Creates the SessionImpl. Opens a connection to the database.
		///
		/// Connection string format:
		///     <str> == <assignment> | <assignment> ';' <str>
		///     <assignment> == <name> '=' <value>
		///     <name> == 'host' | 'port' | 'user' | 'password' | 'db' | 'compress' | 'auto-reconnect' | 'reset' | 'fail-readonly'
		///     <value> == [~;]*
		///
		/// The following settings are supported:
		///   - host: MySQL server hostname or IP address (default: localhost)
		///   - port: MySQL server port number (default: 3306)
		///   - user: MySQL user name
		///   - password: MySQL password
		///   - compress: enable compression (true/false; default: false)
		///   - auto-reconnect: enable automatic reconnect (true/false; default: false)
		///   - secure-auth: use secure authentication (true/false; default: false)
		///   - character-set: connection character set (default: utf8)
		///   - reset: reset connection when returned to SessionPool by calling
		///     mysql_reset_connection().
		///   - fail-readonly: if set to true, the session will fail
		///     if the database becomes read-only. This corresponds to
		///     setFailIfInnoReadOnly(true).
		///
		/// Warning: Due to a bug in MySQL, resetting the connection with mysql_reset_connection()
		/// could change the character encoding used for the connection. Therefore the
		/// reset option should be used with caution.

	~SessionImpl();
		/// Destroys the SessionImpl.

	Poco::SharedPtr<Poco::Data::StatementImpl> createStatementImpl();
		/// Returns an MySQL StatementImpl

	void open(const std::string& connection = "");
		/// Opens a connection to the database.

	void close();
		/// Closes the connection.

	void reset();
		/// Reset connection with dababase and clears session state, but without disconnecting

	bool isConnected() const;
		/// Returns true if connected, false otherwise.

	bool isGood() const;
		/// Returns true iff the database session is good.
		/// For the session to be considered good:
		///   - it must be connected
		///   - and it's last error code must be 0,
		///     or mysql_ping() must be okay.
		///
		/// Furthermore, if the "failIfInnoReadOnly" property
		/// has been set to true, the innodb_read_only setting
		/// must be false. The flag is only checked if the
		/// session has a non-zero error code.

	void setConnectionTimeout(std::size_t timeout);
		/// Sets the session connection timeout value.

	std::size_t getConnectionTimeout() const;
		/// Returns the session connection timeout value.

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

	void setTransactionIsolation(Poco::UInt32 ti);
		/// Sets the transaction isolation level.

	Poco::UInt32 getTransactionIsolation() const;
		/// Returns the transaction isolation level.

	bool hasTransactionIsolation(Poco::UInt32 ti) const;
		/// Returns true iff the transaction isolation level corresponding
		/// to the supplied bitmask is supported.

	bool isTransactionIsolation(Poco::UInt32 ti) const;
		/// Returns true iff the transaction isolation level corresponds
		/// to the supplied bitmask.

	void autoCommit(const std::string&, bool val);
		/// Sets autocommit property for the session.

	bool isAutoCommit(const std::string& name="") const;
		/// Returns autocommit property value.

	void setInsertId(const std::string&, const Poco::Any&);
		/// Try to set insert id - do nothing.

	Poco::Any getInsertId(const std::string&) const;
		/// Get insert id

	void setFailIfInnoReadOnly(const std::string&, bool value);
		/// Sets the "failIfInnoReadOnly" feature. If set, isGood() will
		/// return false if the database is in read-only mode.

	bool getFailIfInnoReadOnly(const std::string&) const;
		/// Returns the state of the "failIfInnoReadOnly" feature.

	void setLastError(int err);
		/// Sets an error code. If a non-zero error code is set, the session
		/// is considered bad.

	int getLastError() const;
		/// Returns the last set error code.

	SessionHandle& handle();
		// Get handle

	const std::string& connectorName() const;
		/// Returns the name of the connector.

private:
	template <typename T>
	static inline T& getValue(MYSQL_BIND* pResult, T& val)
	{
		return val = *((T*) pResult->buffer);
	}

	template <typename T>
	T& getSetting(const std::string& name, T& val) const
		/// Returns required setting.
		/// Limited to one setting at a time.
	{
		StatementExecutor ex(_handle);
		ResultMetadata metadata;
		metadata.reset();
		ex.prepare(Poco::format("SELECT @@%s", name));
		metadata.init(ex);

		if (metadata.columnsReturned() > 0)
			ex.bindResult(metadata.row());
		else
			throw InvalidArgumentException("No data returned.");

		ex.execute(); ex.fetch();
		MYSQL_BIND* pResult = metadata.row();
		return getValue<T>(pResult, val);
	}

	std::string           _connector;
	mutable SessionHandle _handle;
	bool                  _reset;
	bool                  _connected;
	bool                  _inTransaction;
	bool                  _failIfInnoReadOnly;
	std::size_t           _timeout;
	mutable int           _lastError;
	Poco::FastMutex       _mutex;
};


//
// inlines
//
inline bool SessionImpl::canTransact() const
{
	return true;
}


inline void SessionImpl::setInsertId(const std::string&, const Poco::Any&)
{
}


inline Poco::Any SessionImpl::getInsertId(const std::string&) const
{
	return Poco::Any(Poco::UInt64(mysql_insert_id(_handle)));
}


inline void SessionImpl::setFailIfInnoReadOnly(const std::string&, bool value)
{
	_failIfInnoReadOnly = value;
}


inline bool SessionImpl::getFailIfInnoReadOnly(const std::string&) const
{
	return _failIfInnoReadOnly;
}


inline void SessionImpl::setLastError(int err)
{
	_lastError = err;
}


inline int SessionImpl::getLastError() const
{
	return _lastError;
}


inline SessionHandle& SessionImpl::handle()
{
	return _handle;
}


inline const std::string& SessionImpl::connectorName() const
{
	return _connector;
}


inline bool SessionImpl::isTransaction() const
{
	return _inTransaction;
}


inline bool SessionImpl::isTransactionIsolation(Poco::UInt32 ti) const
{
	return getTransactionIsolation() == ti;
}


inline std::size_t SessionImpl::getConnectionTimeout() const
{
	return _timeout;
}


template <>
inline std::string& SessionImpl::getValue(MYSQL_BIND* pResult, std::string& val)
{
	val.assign((char*) pResult->buffer, pResult->buffer_length);
	return val;
}


} } } // namespace Poco::Data::MySQL


#endif // Data_MySQL_SessionImpl_INCLUDED
