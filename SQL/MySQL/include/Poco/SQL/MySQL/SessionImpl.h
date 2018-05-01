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


#ifndef SQL_MySQL_SessionImpl_INCLUDED
#define SQL_MySQL_SessionImpl_INCLUDED


#include "Poco/SQL/MySQL/MySQL.h"
#include "Poco/SQL/AbstractSessionImpl.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SQL/MySQL/SessionHandle.h"
#include "Poco/SQL/MySQL/StatementExecutor.h"
#include "Poco/SQL/MySQL/ResultMetadata.h"
#include "Poco/Mutex.h"


namespace Poco {
namespace SQL {
namespace MySQL {


class MySQL_API SessionImpl: public Poco::SQL::AbstractSessionImpl<SessionImpl>
	/// Implements SessionImpl interface
{
public:
	static const std::string MYSQL_READ_UNCOMMITTED;
	static const std::string MYSQL_READ_COMMITTED;
	static const std::string MYSQL_REPEATABLE_READ;
	static const std::string MYSQL_SERIALIZABLE;

	SessionImpl(const std::string& connectionString,
		std::size_t loginTimeout = LOGIN_TIMEOUT_DEFAULT);
		/// Creates the SessionImpl. Opens a connection to the database
		///
		/// Connection string format:
		///     <str> == <assignment> | <assignment> ';' <str>
		///     <assignment> == <name> '=' <value>
		///     <name> == 'host' | 'port' | 'user' | 'password' | 'db' } 'compress' | 'auto-reconnect'
		///     <value> == [~;]*
		///
		/// for compress and auto-reconnect correct values are true/false
		/// for port - numeric in decimal notation
		///
		
	~SessionImpl();
		/// Destroys the SessionImpl.

	StatementImpl::Ptr createStatementImpl();
		/// Returns an MySQL StatementImpl

	void open(const std::string& connection = "");
		/// Opens a connection to the database.

	void close();
		/// Closes the connection.
		
	bool isConnected() const;
		/// Returns true if connected, false otherwise.

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

	std::string             _connector;
	mutable SessionHandle   _handle;
	bool                    _connected;
	bool                    _inTransaction;
	std::size_t             _timeout;
	Poco::FastMutex         _mutex;
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


inline bool SessionImpl::isConnected() const
{
	return _connected;
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


} } } // namespace Poco::SQL::MySQL


#endif // Data_MySQL_SessionImpl_INCLUDED
