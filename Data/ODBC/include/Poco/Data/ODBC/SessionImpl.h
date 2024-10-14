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


#ifndef Data_ODBC_SessionImpl_INCLUDED
#define Data_ODBC_SessionImpl_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/TypeInfo.h"
#include "Poco/Data/ODBC/Binder.h"
#include "Poco/Data/ODBC/Handle.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/AbstractSessionImpl.h"
#include "Poco/TextEncoding.h"
#include "Poco/SharedPtr.h"
#include "Poco/Mutex.h"
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqltypes.h>


namespace Poco {
namespace Data {
namespace ODBC {


class ODBC_API SessionImpl: public Poco::Data::AbstractSessionImpl<SessionImpl>
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

	enum CursorUse
	{
		ODBC_CURSOR_USE_ALWAYS = Poco::Data::SessionImpl::CURSOR_USE_ALWAYS,
		ODBC_CURSOR_USE_IF_NEEDED = Poco::Data::SessionImpl::CURSOR_USE_IF_NEEDED,
		ODBC_CURSOR_USE_NEVER = Poco::Data::SessionImpl::CURSOR_USE_NEVER
	};

	SessionImpl(const std::string& connect,
		std::size_t loginTimeout,
		std::size_t maxFieldSize = ODBC_MAX_FIELD_SIZE,
		bool autoBind = true,
		bool autoExtract = true);
		/// Creates the SessionImpl. Opens a connection to the database.
		/// Throws NotConnectedException if connection was not succesful.

	//@ deprecated
	SessionImpl(const std::string& connect,
		Poco::Any maxFieldSize = ODBC_MAX_FIELD_SIZE,
		bool enforceCapability=false,
		bool autoBind = true,
		bool autoExtract = true);
		/// Creates the SessionImpl. Opens a connection to the database.

	~SessionImpl();
		/// Destroys the SessionImpl.

	Poco::SharedPtr<Poco::Data::StatementImpl> createStatementImpl();
		/// Returns an ODBC StatementImpl

	void addFeatures();
		/// Adds the ODBC session features and properties.

	void open(const std::string& connect = "");
		/// Opens a connection to the Database

	void close();
		/// Closes the connection

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

	void reset();
		/// Do nothing

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

	void setLoginTimeout(const std::string&, const Poco::Any& value);
		/// Sets the timeout (in seconds) for the session login.
		/// Value must be of type (unsigned) int.
		/// It must be set prior to logging in.

	Poco::Any getLoginTimeout(const std::string&) const;
		/// Returns the timeout (in seconds) for the session login.

	void setQueryTimeout(const std::string&, const Poco::Any& value);
		/// Sets the timeout (in seconds) for queries.
		/// Value must be of type int.

	Poco::Any getQueryTimeout(const std::string&) const;
		/// Returns the timeout (in seconds) for queries,
		/// or -1 if no timeout has been set.

	void setCursorUse(const std::string&, const Poco::Any& value);
		/// Sets the use of cursors:
		///   - SQL_CUR_USE_ODBC - always
		///   - SQL_CUR_USE_IF_NEEDED - if needed
		///   - SQL_CUR_USE_DRIVER - never

	Poco::Any getCursorUse(const std::string&) const;
		/// Returns the use of cursors.

	int queryTimeout() const;
		/// Returns the timeout (in seconds) for queries,
		/// or -1 if no timeout has been set.

	void setDBEncoding(const std::string&, const Poco::Any& value);
		/// Sets the database encoding.
		/// Value must be of type std::string.

	Poco::Any getDBEncoding(const std::string&) const;
		/// Returns the database encoding.

	const std::string& dbEncoding() const;
		/// Returns the database encoding.

	void setMultiActiveResultset(const std::string&, bool value);
		/// Sets the multiple active resultset capability, if available.
		/// Does nothing, if feature is not available.

	bool getMultiActiveResultset(const std::string&) const;
		/// Returns the multiple active resultset capability, if available.
		/// Returns false, if feature is not available.

	const ConnectionHandle& dbc() const;
		/// Returns the connection handle.

	Poco::Any dataTypeInfo(const std::string& rName="") const;
		/// Returns the data types information.

private:
	void setDataTypeInfo(const std::string& rName, const Poco::Any& rValue);
		/// No-op. Throws InvalidAccessException.

	static const int FUNCTIONS = SQL_API_ODBC3_ALL_FUNCTIONS_SIZE;

	void checkError(SQLRETURN rc, const std::string& msg="") const;

	Poco::UInt32 getDefaultTransactionIsolation() const;

	static Poco::UInt32 transactionIsolation(SQLULEN isolation);

	void setTransactionIsolationImpl(Poco::UInt32 ti) const;
		/// Sets the transaction isolation level.
		/// Called internally from getTransactionIsolation()

	std::string                   _connector;
	mutable ConnectionHandle      _db;
	Poco::Any                     _maxFieldSize;
	bool                          _autoBind;
	bool                          _autoExtract;
	TypeInfo                      _dataTypes;
	mutable TransactionCapability _canTransact;
	std::atomic<bool>             _inTransaction;
	int                           _queryTimeout;
	std::string                   _dbEncoding;
	Poco::FastMutex               _mutex;
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
	return &_dataTypes;
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


inline bool SessionImpl::isTransactionIsolation(Poco::UInt32 ti) const
{
	return 0 != (ti & getTransactionIsolation());
}


inline Poco::Any SessionImpl::getLoginTimeout(const std::string&) const
{
	return _db.getLoginTimeout();
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


inline Poco::Any SessionImpl::getDBEncoding(const std::string&) const
{
	return _dbEncoding;
}


inline const std::string& SessionImpl::dbEncoding() const
{
	return _dbEncoding;
}


} } } // namespace Poco::Data::ODBC


#endif // Data_ODBC_SessionImpl_INCLUDED
