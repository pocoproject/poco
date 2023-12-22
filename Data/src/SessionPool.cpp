//
// SessionPool.cpp
//
// Library: Data
// Package: SessionPooling
// Module:  SessionPool
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/SessionPool.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/DataException.h"
#include <algorithm>


namespace Poco {
namespace Data {


SessionPool::SessionPool(const std::string& connector, const std::string& connectionString, int minSessions, int maxSessions, int idleTime, int connTimeout):
	_connector(connector),
	_connectionString(connectionString),
	_minSessions(minSessions),
	_maxSessions(maxSessions),
	_idleTime(idleTime),
	_connTimeout(connTimeout),
	_nSessions(0),
	_janitorTimer(1000*idleTime, 1000*idleTime/4),
	_shutdown(false)
{
	Poco::TimerCallback<SessionPool> callback(*this, &SessionPool::onJanitorTimer);
	_janitorTimer.start(callback);
}


SessionPool::~SessionPool()
{
	try
	{
		shutdown();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


Session SessionPool::get(const std::string& name, bool value)
{
	Session s = get();
	_addFeatureMap.insert(AddFeatureMap::value_type(s.impl(),
		std::make_pair(name, s.getFeature(name))));
	s.setFeature(name, value);

	return s;
}


Session SessionPool::get()
{
	if (_shutdown) throw InvalidAccessException("Session pool has been shut down.");

	Poco::Mutex::ScopedLock lock(_mutex);
	purgeDeadSessions();

	if (_idleSessions.empty())
	{
		if (_nSessions < _maxSessions)
		{
			Session newSession(SessionFactory::instance().create(_connector, _connectionString, static_cast<std::size_t>(_connTimeout)));
			applySettings(newSession.impl());
			customizeSession(newSession);

			PooledSessionHolderPtr pHolder(new PooledSessionHolder(*this, newSession.impl()));
			_idleSessions.push_back(pHolder);
			++_nSessions;
		}
		else throw SessionPoolExhaustedException(_connector);
	}

	PooledSessionHolderPtr pHolder(_idleSessions.front());
	PooledSessionImplPtr pPSI(new PooledSessionImpl(pHolder));

	_activeSessions.push_front(pHolder);
	_idleSessions.pop_front();
	return Session(pPSI);
}


void SessionPool::purgeDeadSessions()
{
	if (_shutdown) return;

	SessionList::iterator it = _idleSessions.begin();
	for (; it != _idleSessions.end(); )
	{
		if (!(*it)->session()->isGood())
		{
			it = _idleSessions.erase(it);
			--_nSessions;
		}
		else ++it;
	}
}


int SessionPool::capacity() const
{
	return _maxSessions;
}


int SessionPool::used() const
{
	Poco::Mutex::ScopedLock lock(_mutex);
	return (int) _activeSessions.size();
}


int SessionPool::idle() const
{
	Poco::Mutex::ScopedLock lock(_mutex);
	return (int) _idleSessions.size();
}


int SessionPool::connTimeout() const
{
	return _connTimeout;
}


int SessionPool::dead()
{
	int count = 0;

	Poco::Mutex::ScopedLock lock(_mutex);
	SessionList::iterator it = _activeSessions.begin();
	SessionList::iterator itEnd = _activeSessions.end();
	for (; it != itEnd; ++it)
	{
		if (!(*it)->session()->isGood())
			++count;
	}

	return count;
}


int SessionPool::allocated() const
{
	return _nSessions;
}


int SessionPool::available() const
{
	if (_shutdown) return 0;
	return _maxSessions - used();
}


void SessionPool::setFeature(const std::string& name, bool state)
{
	if (_shutdown) throw InvalidAccessException("Session pool has been shut down.");

	if (_nSessions > 0)
		throw InvalidAccessException("Features can not be set after the first session was created.");

	Poco::Mutex::ScopedLock lock(_mutex);
	_featureMap.insert(FeatureMap::ValueType(name, state));
}


bool SessionPool::getFeature(const std::string& name) const
{

	if (_shutdown) throw InvalidAccessException("Session pool has been shut down.");

	Poco::Mutex::ScopedLock lock(_mutex);
	FeatureMap::ConstIterator it = _featureMap.find(name);

	if (_featureMap.end() == it)
		throw NotFoundException("Feature not found:" + name);

	return it->second;
}


void SessionPool::setProperty(const std::string& name, const Poco::Any& value)
{
	if (_shutdown) throw InvalidAccessException("Session pool has been shut down.");

	if (_nSessions > 0)
		throw InvalidAccessException("Properties can not be set after first session was created.");

	Poco::Mutex::ScopedLock lock(_mutex);
	_propertyMap.insert(PropertyMap::ValueType(name, value));
}


Poco::Any SessionPool::getProperty(const std::string& name) const
{
	Poco::Mutex::ScopedLock lock(_mutex);
	PropertyMap::ConstIterator it = _propertyMap.find(name);

	if (_propertyMap.end() == it)
		throw NotFoundException("Property not found:" + name);

	return it->second;
}


void SessionPool::applySettings(SessionImpl* pImpl)
{
	FeatureMap::Iterator fmIt = _featureMap.begin();
	FeatureMap::Iterator fmEnd = _featureMap.end();
	for (; fmIt != fmEnd; ++fmIt) pImpl->setFeature(fmIt->first, fmIt->second);

	PropertyMap::Iterator pmIt = _propertyMap.begin();
	PropertyMap::Iterator pmEnd = _propertyMap.end();
	for (; pmIt != pmEnd; ++pmIt) pImpl->setProperty(pmIt->first, pmIt->second);
}


void SessionPool::customizeSession(Session&)
{
}


void SessionPool::putBack(PooledSessionHolderPtr pHolder)
{
	if (_shutdown) return;

	Poco::Mutex::ScopedLock lock(_mutex);
	SessionList::iterator it = std::find(_activeSessions.begin(), _activeSessions.end(), pHolder);
	if (it != _activeSessions.end())
	{
		try
		{
			if (pHolder->session()->isGood())
			{
				pHolder->session()->reset();

				// reverse settings applied at acquisition time, if any
				AddPropertyMap::iterator pIt = _addPropertyMap.find(pHolder->session());
				if (pIt != _addPropertyMap.end())
					pHolder->session()->setProperty(pIt->second.first, pIt->second.second);

				AddFeatureMap::iterator fIt = _addFeatureMap.find(pHolder->session());
				if (fIt != _addFeatureMap.end())
					pHolder->session()->setFeature(fIt->second.first, fIt->second.second);

				// re-apply the default pool settings
				applySettings(pHolder->session());

				pHolder->access();
				_idleSessions.push_back(pHolder);
			}
			else --_nSessions;

			_activeSessions.erase(it);
		}
		catch (const Poco::Exception& e)
		{
			--_nSessions;
			_activeSessions.erase(it);
			poco_bugcheck_msg(format("Exception in SessionPool::putBack(): %s", e.displayText()).c_str());
		}
		catch (...)
		{
			--_nSessions;
			_activeSessions.erase(it);
			poco_bugcheck_msg("Unknown exception in SessionPool::putBack()");
		}
	}
	else
	{
		poco_bugcheck_msg("Unknown session passed to SessionPool::putBack()");
	}
}


void SessionPool::onJanitorTimer(Poco::Timer&)
{
	if (_shutdown) return;

	Poco::Mutex::ScopedLock lock(_mutex);
	SessionList::iterator it = _idleSessions.begin();
	while (_nSessions > _minSessions && it != _idleSessions.end())
	{
		if ((*it)->idle() > _idleTime || !(*it)->session()->isGood())
		{
			try
			{
				(*it)->session()->close();
			}
			catch (...)
			{
			}
			it = _idleSessions.erase(it);
			--_nSessions;
		}
		else ++it;
	}
}


void SessionPool::shutdown()
{
	if (_shutdown.exchange(true)) return;
	_shutdown = true;
	_janitorTimer.stop();

	Poco::Mutex::ScopedLock lock(_mutex);
	closeAll(_idleSessions);
	closeAll(_activeSessions);
}


void SessionPool::closeAll(SessionList& sessionList)
{
	SessionList::iterator it = sessionList.begin();
	for (; it != sessionList.end();)
	{
		try
		{
			(*it)->session()->close();
		}
		catch (...)
		{
		}
		it = sessionList.erase(it);
		if (_nSessions > 0) --_nSessions;
	}
}


} } // namespace Poco::Data
