//
// PooledSessionImpl.cpp
//
// Library: Data
// Package: SessionPooling
// Module:  PooledSessionImpl
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/PooledSessionImpl.h"
#include "Poco/Data/DataException.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/ErrorHandler.h"


namespace Poco::Data {


namespace {
	// Returns the session of the holder; checks the holder first,
	// as the base class is initialized from it.
	[[nodiscard]] SessionImpl& holderSession(PooledSessionHolder* pHolder)
	{
		poco_check_ptr (pHolder);
		return *pHolder->session();
	}
} // namespace


PooledSessionImpl::PooledSessionImpl(PooledSessionHolder* pHolder):
	SessionImpl(holderSession(pHolder).connectionString(),
		holderSession(pHolder).getLoginTimeout()),
	_pHolder(pHolder, true)
{
}


PooledSessionImpl::~PooledSessionImpl()
{
	try
	{
		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


StatementImpl::Ptr PooledSessionImpl::createStatementImpl()
{
	return access()->createStatementImpl();
}


void PooledSessionImpl::begin()
{
	return access()->begin();
}


void PooledSessionImpl::commit()
{
	return access()->commit();
}


bool PooledSessionImpl::isConnected() const
{
	return access()->isConnected();
}


bool PooledSessionImpl::isGood() const
{
	return access()->isGood();
}


void PooledSessionImpl::setConnectionTimeout(std::size_t timeout)
{
	return access()->setConnectionTimeout(timeout);
}


std::size_t PooledSessionImpl::getConnectionTimeout() const
{
	return access()->getConnectionTimeout();
}


bool PooledSessionImpl::canTransact() const
{
	return access()->canTransact();
}


bool PooledSessionImpl::isTransaction() const
{
	Poco::AutoPtr<PooledSessionHolder> pHolder = holder();
	if (pHolder == nullptr) return false;

	pHolder->access();
	return pHolder->session()->isTransaction();
}


void PooledSessionImpl::setTransactionIsolation(Poco::UInt32 ti)
{
	access()->setTransactionIsolation(ti);
}


Poco::UInt32 PooledSessionImpl::getTransactionIsolation() const
{
	return access()->getTransactionIsolation();
}


bool PooledSessionImpl::hasTransactionIsolation(Poco::UInt32 ti) const
{
	return access()->hasTransactionIsolation(ti);
}


bool PooledSessionImpl::isTransactionIsolation(Poco::UInt32 ti) const
{
	return access()->isTransactionIsolation(ti);
}


void PooledSessionImpl::rollback()
{
	return access()->rollback();
}


void PooledSessionImpl::open(const std::string& connect)
{
	access()->open(connect);
}


void PooledSessionImpl::close()
{
	Poco::AutoPtr<PooledSessionHolder> pHolder;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		pHolder.swap(_pHolder);
	}
	if (pHolder == nullptr) return;

	SessionImpl* pImpl = pHolder->session();
	bool discard = false;
	try
	{
		// roll back an open transaction and, in manual-commit mode, work done outside
		// a transaction begun through the session: the pool switches auto-commit back on,
		// which would commit it, and the next user of the session must not inherit it
		if (pImpl->isConnected() && (pImpl->isTransaction() ||
			(pImpl->hasFeature("autoCommit") && !pImpl->getFeature("autoCommit"))))
		{
			pImpl->rollback();
		}
	}
	catch (Poco::Exception& exc)
	{
		// something is wrong with the session: get rid of it
		discard = true;
		Poco::ErrorHandler::handle(exc);
	}
	catch (std::exception& exc)
	{
		discard = true;
		Poco::ErrorHandler::handle(exc);
	}
	catch (...)
	{
		discard = true;
		Poco::ErrorHandler::handle();
	}

	// An orphaned session is not returned: its pool has been shut down and may be gone.
	bool orphaned = pHolder->isOrphaned();
	if (discard || orphaned)
	{
		try
		{
			pImpl->close();
		}
		catch (...)
		{
		}
	}
	if (!orphaned) pHolder->owner().putBack(pHolder);
}


void PooledSessionImpl::reset()
{
	access()->reset();
}


const std::string& PooledSessionImpl::connectorName() const
{
	return access()->connectorName();
}


bool PooledSessionImpl::hasFeature(const std::string& name) const
{
	return access()->hasFeature(name);
}


void PooledSessionImpl::setFeature(const std::string& name, bool state)
{
	access()->setFeature(name, state);
}


bool PooledSessionImpl::getFeature(const std::string& name) const
{
	return access()->getFeature(name);
}


bool PooledSessionImpl::hasProperty(const std::string& name) const
{
	return access()->hasProperty(name);
}


void PooledSessionImpl::setProperty(const std::string& name, const Poco::Any& value)
{
	access()->setProperty(name, value);
}


Poco::Any PooledSessionImpl::getProperty(const std::string& name) const
{
	return access()->getProperty(name);
}


SessionImpl::Ptr PooledSessionImpl::access() const
{
	Poco::AutoPtr<PooledSessionHolder> pHolder = holder();
	if (pHolder != nullptr)
	{
		pHolder->access();
		return SessionImpl::Ptr(pHolder->session(), true);
	}
	else throw SessionUnavailableException();
}


Poco::AutoPtr<PooledSessionHolder> PooledSessionImpl::holder() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _pHolder;
}


} // namespace Poco::Data
