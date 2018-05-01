//
// PooledSessionImpl.h
//
// Library: Data
// Package: SessionPooling
// Module:  PooledSessionImpl
//
// Definition of the PooledSessionImpl class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_PooledSessionImpl_INCLUDED
#define SQL_PooledSessionImpl_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/SQL/SessionImpl.h"
#include "Poco/SQL/StatementImpl.h"
#include "Poco/SQL/PooledSessionHolder.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace SQL {


class SessionPool;


class Poco_SQL_API PooledSessionImpl: public SessionImpl
	/// PooledSessionImpl is a decorator created by
	/// SessionPool that adds session pool
	/// management to SessionImpl objects.
{
public:
	PooledSessionImpl(PooledSessionHolder::Ptr pHolder);
		/// Creates the PooledSessionImpl.

	~PooledSessionImpl();
		/// Destroys the PooledSessionImpl.

	// SessionImpl
	StatementImpl::Ptr createStatementImpl();
	void begin();
	void commit();
	void rollback();
	void open(const std::string& connect = "");
	void close();
	bool isConnected() const;
	void setConnectionTimeout(std::size_t timeout);
	std::size_t getConnectionTimeout() const;
	bool canTransact() const;
	bool isTransaction() const;
	void setTransactionIsolation(Poco::UInt32);
	Poco::UInt32 getTransactionIsolation() const;
	bool hasTransactionIsolation(Poco::UInt32) const;
	bool isTransactionIsolation(Poco::UInt32) const;
	const std::string& connectorName() const;
	void setFeature(const std::string& name, bool state);
	bool getFeature(const std::string& name) const;
	void setProperty(const std::string& name, const Poco::Any& value);
	Poco::Any getProperty(const std::string& name) const;

	virtual void putBack();

protected:
	SessionImpl::Ptr access() const;
		/// Updates the last access timestamp,
		/// verifies validity of the session
		/// and returns the session if it is valid.
		///
		/// Throws an SessionUnavailableException if the
		/// session is no longer valid.
		
	SessionImpl::Ptr impl() const;
		/// Returns a pointer to the SessionImpl.

private:
	mutable PooledSessionHolder::Ptr _pHolder;
};


//
// inlines
//
inline SessionImpl::Ptr PooledSessionImpl::impl() const
{
	return _pHolder->session();
}


} } // namespace Poco::SQL


#endif // Data_PooledSessionImpl_INCLUDED
