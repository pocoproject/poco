//
// SessionImpl.h
//
// Library: Data/ODBC
// Package: ODBC
// Module:  SessionImpl
//
// Definition of the SessionImpl class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_ODBC_SessionImpl_INCLUDED
#define SQL_ODBC_SessionImpl_INCLUDED


#include "Poco/SQL/ODBC/ODBC.h"
#include "Poco/SQL/ODBC/Connector.h"
#include "Poco/SQL/ODBC/TypeInfo.h"
#include "Poco/SQL/ODBC/Binder.h"
#include "Poco/SQL/ODBC/Handle.h"
#include "Poco/SQL/ODBC/ODBCException.h"
#include "Poco/SQL/AbstractSessionImpl.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SharedPtr.h"
#include "Poco/Mutex.h"
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqltypes.h>


namespace Poco {
namespace SQL {
namespace ODBC {


class ODBC_API SessionImpl: public Poco::SQL::AbstractSessionImpl<SessionImpl>
	/// Implements SessionImpl interface
{
public:
	static const std::size_t ODBC_MAX_FIELD_SIZE = 1024u;

	enum TransactionCapability
	{
		ODBC_TXN_CAPABILITY_UNKNOWN = -1,
		ODBC_TXN_CAPABILITY_FALSE = 0,
		ODBC_TXN_CAPABILITY_TRUE = 1
	};

	SessionImpl(const std::string& connect,
		std::size_t loginTimeout,
		std::size_t maxFieldSize = ODBC_MAX_FIELD_SIZE,
		bool autoBind = true,
		bool autoExtract = true);
		/// Creates the SessionImpl. Opens a connection to the database.
		/// Throws NotConnectedException if connection was not successful.

	//@ deprecated
	SessionImpl(const std::string& connect,
		Poco::Any maxFieldSize = ODBC_MAX_FIELD_SIZE,
		bool enforceCapability=false,
		bool autoBind = true,
		bool autoExtract = true);
		/// Creates the SessionImpl. Opens a connection to the database.

	~SessionImpl();
		/// Destroys the SessionImpl.

	StatementImpl::Ptr createStatementImpl();
		/// Returns an ODBC StatementImpl

	void open(const std::string& connect = "");
		/// Opens a connection to the Database

	void close();
		/// Closes the connection
		/// Throws ODBCException if close was not successful.

	bool isConnected() const;
		/// Returns true if session is connected

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

	bool isTransaction() const;
		/// Returns true iff a transaction is in progress.

	const std::string& connectorName() const;
		/// Returns the name of the connector.

	bool canTransact() const;
		/// Returns true if connection is transaction-capable.

	void setTransactionIsolation(Poco::UInt32 ti);
		/// Sets the transaction isolation level.

	Poco::UInt32 getTransactionIsolation() const;
		/// Returns the transaction isolation level.

	bool hasTransactionIsolation(Poco::UInt32) const;
		/// Returns true iff the transaction isolation level corresponding
		/// to the supplied bitmask is supported.

	bool isTransactionIsolation(Poco::UInt32) const;
		/// Returns true iff the transaction isolation level corresponds
		/// to the supplied bitmask.

	void autoCommit(const std::string&, bool val);
		/// Sets autocommit property for the session.

	bool isAutoCommit(const std::string& name="") const;
		/// Returns autocommit property value.

	void autoBind(const std::string&, bool val);
		/// Sets automatic binding for the session.

	bool isAutoBind(const std::string& name="") const;
		/// Returns true if binding is automatic for this session.

	void autoExtract(const std::string&, bool val);
		/// Sets automatic extraction for the session.

	bool isAutoExtract(const std::string& name="") const;
		/// Returns true if extraction is automatic for this session.

	void setMaxFieldSize(const std::string& rName, const Poco::Any& rValue);
		/// Sets the max field size (the default used when column size is unknown).
		
	Poco::Any getMaxFieldSize(const std::string& rName="") const;
		/// Returns the max field size (the default used when column size is unknown).

	int maxStatementLength() const;
		/// Returns maximum length of SQL statement allowed by driver.

	void setQueryTimeout(const std::string&, const Poco::Any& value);
		/// Sets the timeout (in seconds) for queries.
		/// Value must be of type int.
		
	Poco::Any getQueryTimeout(const std::string&) const;
		/// Returns the timeout (in seconds) for queries,
		/// or -1 if no timeout has been set.

	int queryTimeout() const;
		/// Returns the timeout (in seconds) for queries,
		/// or -1 if no timeout has been set.

	const ConnectionHandle& dbc() const;
		/// Returns the connection handle.

	Poco::Any dataTypeInfo(const std::string& rName = "") const;
		/// Returns the data types information.

private:
	void setDataTypeInfo(const std::string& rName, const Poco::Any& rValue);
		/// No-op. Throws InvalidAccessException.

	void init();

	static const int FUNCTIONS = SQL_API_ODBC3_ALL_FUNCTIONS_SIZE;

	void checkError(SQLRETURN rc, const std::string& msg="") const;

	Poco::UInt32 getDefaultTransactionIsolation() const;

	Poco::UInt32 transactionIsolation(SQLULEN isolation) const;

	void setTransactionIsolationImpl(Poco::UInt32 ti) const;

	std::string            _connector;
	const ConnectionHandle _db;
	Poco::Any              _maxFieldSize;
	bool                   _autoBind;
	bool                   _autoExtract;
	TypeInfo               _dataTypes;
	mutable char           _canTransact;
	bool                   _inTransaction;
	int                    _queryTimeout;
	Poco::FastMutex        _mutex;
};


///
/// inlines
///
inline void SessionImpl::checkError(SQLRETURN rc, const std::string& msg) const
{
	if (Utility::isError(rc))
		throw ConnectionException(_db, msg);
}


inline const ConnectionHandle& SessionImpl::dbc() const
{
	return _db;
}


inline void SessionImpl::setMaxFieldSize(const std::string& rName, const Poco::Any& rValue)
{
	_maxFieldSize = rValue;
}


inline Poco::Any SessionImpl::getMaxFieldSize(const std::string& rName) const
{
	return _maxFieldSize;
}


inline void SessionImpl::setDataTypeInfo(const std::string& rName, const Poco::Any& rValue)
{
	throw InvalidAccessException();
}


inline Poco::Any SessionImpl::dataTypeInfo(const std::string& rName) const
{
	return const_cast<TypeInfo*>(&_dataTypes);
}


inline void SessionImpl::autoBind(const std::string&, bool val)
{
	_autoBind = val;
}


inline bool SessionImpl::isAutoBind(const std::string& name) const
{
	return _autoBind;
}


inline void SessionImpl::autoExtract(const std::string&, bool val)
{
	_autoExtract = val;
}


inline bool SessionImpl::isAutoExtract(const std::string& name) const
{
	return _autoExtract;
}


inline const std::string& SessionImpl::connectorName() const
{
	return _connector;
}


inline void SessionImpl::setTransactionIsolation(Poco::UInt32 ti)
{
	setTransactionIsolationImpl(ti);
}


inline bool SessionImpl::isTransactionIsolation(Poco::UInt32 ti) const
{
	return 0 != (ti & getTransactionIsolation());
}


inline void SessionImpl::setQueryTimeout(const std::string&, const Poco::Any& value)
{
	_queryTimeout = Poco::AnyCast<int>(value);
}


inline Poco::Any SessionImpl::getQueryTimeout(const std::string&) const
{
	return _queryTimeout;
}


inline int SessionImpl::queryTimeout() const
{
	return _queryTimeout;
}


} } } // namespace Poco::SQL::ODBC


#endif // Data_ODBC_SessionImpl_INCLUDED
