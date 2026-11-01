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
#include "Poco/ErrorHandler.h"
#include <algorithm>


namespace Poco::Data {


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
	FeaturePair feature(name, s.getFeature(name));
	s.setFeature(name, value);

	SessionImpl* pImpl = static_cast<PooledSessionImpl*>(s.impl())->impl();
	Poco::Mutex::ScopedLock lock(_mutex);
	_addFeatureMap[pImpl] = feature;

	return s;
}


Session SessionPool::get()
{
	Poco::Mutex::ScopedLock lock(_mutex);
	if (_shutdown) throw InvalidAccessException("Session pool has been shut down.");

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
			// close it now: a Session that returned it may still refer to it
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
	SessionList::iterator it = _idleSessions.begin();
	SessionList::iterator itEnd = _idleSessions.end();
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
	try
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		SessionImpl* pImpl = pHolder->session();

		// take out the settings applied at acquisition time, if any
		bool hasProperty = false;
		PropertyPair property;
		if (auto pIt = _addPropertyMap.find(pImpl); pIt != _addPropertyMap.end())
		{
			hasProperty = true;
			property = pIt->second;
			_addPropertyMap.erase(pIt);
		}

		bool hasFeature = false;
		FeaturePair feature;
		if (auto fIt = _addFeatureMap.find(pImpl); fIt != _addFeatureMap.end())
		{
			hasFeature = true;
			feature = fIt->second;
			_addFeatureMap.erase(fIt);
		}

		if (auto it = std::find(_activeSessions.begin(), _activeSessions.end(), pHolder); it != _activeSessions.end())
		{
			_activeSessions.erase(it);
			try
			{
				if (pImpl->isGood())
				{
					pImpl->reset();

					// reverse settings applied at acquisition time, if any
					if (hasProperty) pImpl->setProperty(property.first, property.second);
					if (hasFeature) pImpl->setFeature(feature.first, feature.second);

					// re-apply the default pool settings
					applySettings(pImpl);

					// restore the auto-commit mode the session was created with
					if (pImpl->hasFeature("autoCommit") && pImpl->getFeature("autoCommit") != pHolder->autoCommit())
						pImpl->setFeature("autoCommit", pHolder->autoCommit());

					pHolder->access();
					_idleSessions.push_back(pHolder);
				}
				else
				{
					// close it now: the Session that returned it still refers to it
					--_nSessions;
					try
					{
						pImpl->close();
					}
					catch (...)
					{
					}
				}
			}
			catch (...)
			{
				// the session can not be reused: discard it
				--_nSessions;
				try
				{
					pImpl->close();
				}
				catch (...)
				{
				}
				throw;
			}
		}
		else if (pHolder->isOrphaned())
		{
			// the pool was shut down while the session was being returned
			try
			{
				pImpl->close();
			}
			catch (...)
			{
			}
		}
	}
	catch (Poco::Exception& exc)
	{
		Poco::ErrorHandler::handle(exc);
	}
	catch (std::exception& exc)
	{
		Poco::ErrorHandler::handle(exc);
	}
	catch (...)
	{
		Poco::ErrorHandler::handle();
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

	// a session still in use is closed by its user, when released
	for (auto it = _activeSessions.begin(); it != _activeSessions.end();)
	{
		(*it)->orphan();
		it = _activeSessions.erase(it);
		if (_nSessions > 0) --_nSessions;
	}

	_addPropertyMap.clear();
	_addFeatureMap.clear();
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


} // namespace Poco::Data
