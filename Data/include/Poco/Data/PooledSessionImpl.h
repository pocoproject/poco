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


namespace Poco {
namespace Data {


class SessionPool;


class Data_API PooledSessionImpl: public SessionImpl
	/// PooledSessionImpl is a decorator created by
	/// SessionPool that adds session pool
	/// management to SessionImpl objects.
{
public:
	PooledSessionImpl(PooledSessionHolder* pHolder);
		/// Creates the PooledSessionImpl.

	~PooledSessionImpl() override;
		/// Destroys the PooledSessionImpl.

	StatementImpl::Ptr createStatementImpl() override;
	void begin() override;
	void commit() override;
	void rollback() override;
	void open(const std::string& connect = "") override;
	void close() override;
	void reset() override;
	bool isConnected() const override;
	bool isGood() const override;
	void setConnectionTimeout(std::size_t timeout) override;
	std::size_t getConnectionTimeout() const override;
	bool canTransact() const override;
	bool isTransaction() const override;
	void setTransactionIsolation(Poco::UInt32) override;
	Poco::UInt32 getTransactionIsolation() const override;
	bool hasTransactionIsolation(Poco::UInt32) const override;
	bool isTransactionIsolation(Poco::UInt32) const override;
	const std::string& connectorName() const override;
	bool hasFeature(const std::string& name) const override;
	void setFeature(const std::string& name, bool state) override;
	bool getFeature(const std::string& name) const override;
	bool hasProperty(const std::string& name) const override;
	void setProperty(const std::string& name, const Poco::Any& value) override;
	Poco::Any getProperty(const std::string& name) const override;

protected:
	SessionImpl* access() const;
		/// Updates the last access timestamp,
		/// verifies validity of the session
		/// and returns the session if it is valid.
		///
		/// Throws an SessionUnavailableException if the
		/// session is no longer valid.

	SessionImpl* impl() const;
		/// Returns a pointer to the SessionImpl.

private:
	mutable Poco::AutoPtr<PooledSessionHolder> _pHolder;
};


//
// inlines
//
inline SessionImpl* PooledSessionImpl::impl() const
{
	return _pHolder->session();
}


} } // namespace Poco::Data


#endif // Data_PooledSessionImpl_INCLUDED
