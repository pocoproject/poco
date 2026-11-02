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


#ifndef Data_PooledSessionImpl_INCLUDED
#define Data_PooledSessionImpl_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/SessionImpl.h"
#include "Poco/Data/PooledSessionHolder.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/AutoPtr.h"
#include "Poco/Mutex.h"


namespace Poco::Data {


class SessionPool;


class Data_API PooledSessionImpl: public SessionImpl
	/// PooledSessionImpl is a decorator created by
	/// SessionPool that adds session pool
	/// management to SessionImpl objects.
{
public:
	PooledSessionImpl(PooledSessionHolder* pHolder);
		/// Creates the PooledSessionImpl.
		/// Throws NullPointerException if pHolder is null.

	~PooledSessionImpl();
		/// Destroys the PooledSessionImpl.

	StatementImpl::Ptr createStatementImpl();
	void begin();
	void commit();
	void rollback();
	void open(const std::string& connect = "");
	void close();
		/// Returns the session to its pool, or closes it if the pool has been shut down.
	void reset();
	[[nodiscard]] bool isConnected() const;
		/// Returns false once the session has been returned to its pool.
		/// The other members, connectorName() among them, then throw
		/// SessionUnavailableException, so creating a statement through the
		/// Session, begin(), commit() and rollback() throw it too.
	[[nodiscard]] bool isGood() const;
	void setConnectionTimeout(std::size_t timeout);
	[[nodiscard]] std::size_t getConnectionTimeout() const;
	[[nodiscard]] bool canTransact() const;
	[[nodiscard]] bool isTransaction() const;
		/// Returns false once the session has been returned to its pool.
	void setTransactionIsolation(Poco::UInt32);
	[[nodiscard]] Poco::UInt32 getTransactionIsolation() const;
	[[nodiscard]] bool hasTransactionIsolation(Poco::UInt32) const;
	[[nodiscard]] bool isTransactionIsolation(Poco::UInt32) const;
	[[nodiscard]] const std::string& connectorName() const;
	bool hasFeature(const std::string& name) const;
	void setFeature(const std::string& name, bool state);
	[[nodiscard]] bool getFeature(const std::string& name) const;
	[[nodiscard]] bool hasProperty(const std::string& name) const;
	void setProperty(const std::string& name, const Poco::Any& value);
	[[nodiscard]] Poco::Any getProperty(const std::string& name) const;

protected:
	[[nodiscard]] SessionImpl::Ptr access() const;
		/// Updates the last access timestamp,
		/// verifies validity of the session
		/// and returns the session if it is valid.
		///
		/// Throws an SessionUnavailableException if the
		/// session is no longer valid.

	[[nodiscard]] SessionImpl* impl() const;
		/// Returns a pointer to the SessionImpl, or nullptr
		/// if the session has been returned to its pool.

private:
	[[nodiscard]] Poco::AutoPtr<PooledSessionHolder> holder() const;
		/// Returns the holder, or a null pointer if the
		/// session has been returned to its pool.

	[[nodiscard]] SessionImpl::Ptr heldSession() const;
		/// Updates the last access timestamp and returns the session,
		/// or a null pointer once the session has been returned to its pool.

	mutable Poco::AutoPtr<PooledSessionHolder> _pHolder;
	mutable Poco::FastMutex _mutex;
	SessionImpl::Ptr _pSessionImpl;
		/// The session of the holder, kept after close():
		/// the statements created through this object refer to it.

	friend class SessionPool;
};


//
// inlines
//
inline SessionImpl* PooledSessionImpl::impl() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _pHolder != nullptr ? _pHolder->session() : nullptr;
}


} // namespace Poco::Data


#endif // Data_PooledSessionImpl_INCLUDED
