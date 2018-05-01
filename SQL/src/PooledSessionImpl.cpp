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


#include "Poco/SQL/PooledSessionImpl.h"
#include "Poco/SQL/SQLException.h"
#include "Poco/SQL/SessionPool.h"


namespace Poco {
namespace SQL {


PooledSessionImpl::PooledSessionImpl(PooledSessionHolder::Ptr pHolder):
	SessionImpl(pHolder->session()->connectionString(),
		pHolder->session()->getLoginTimeout()),
	_pHolder(pHolder)
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
	return access()->isTransaction();
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


void PooledSessionImpl::putBack()
{
	if (_pHolder)
	{
		_pHolder->owner().putBack(_pHolder);
		_pHolder = nullptr;
	}
}


void PooledSessionImpl::close()
{
	if (_pHolder)
	{
		if (isTransaction())
		{
			try
			{
				rollback();
			}
			catch (...)
			{
				// Something's wrong with the session. Get rid of it.
				access()->close();
			}
		}
		putBack();
	}
}


const std::string& PooledSessionImpl::connectorName() const
{
	return access()->connectorName();
}


void PooledSessionImpl::setFeature(const std::string& name, bool state)	
{
	access()->setFeature(name, state);
}


bool PooledSessionImpl::getFeature(const std::string& name) const
{
	return access()->getFeature(name);
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
	if (_pHolder)
	{
		_pHolder->access();
		return impl();
	}
	else throw SessionUnavailableException();
}


} } // namespace Poco::SQL
