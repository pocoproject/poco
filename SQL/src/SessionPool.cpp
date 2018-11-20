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


#include "Poco/SQL/SessionPool.h"
#include "Poco/SQL/SessionFactory.h"
#include "Poco/SQL/SQLException.h"
#include <algorithm>


namespace Poco {
namespace SQL {


SessionPool::SessionPool(const std::string& connector, const std::string& connectionString, int minSessions, int maxSessions, int idleTime):
	_connector(connector),
	_connectionString(connectionString),
	_minSessions(minSessions),
	_maxSessions(maxSessions),
	_idleTime(idleTime),
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


Session SessionPool::get(const std::string& rName, bool value)
{
	Session s = get();
	_addFeatureMap.insert(AddFeatureMap::value_type(s.impl(),
		std::make_pair(rName, s.getFeature(rName))));
	s.setFeature(rName, value);

	return s;
}


Session SessionPool::get()
{
	if (_shutdown) throw InvalidAccessException("Session pool has been shut down.");

	purgeDeadSessions();
	Poco::Mutex::ScopedLock lock(_mutex);
	if (_idleSessions.empty())
	{
		if (_nSessions < _maxSessions)
		{
			Session newSession(SessionFactory::instance().create(_connector, _connectionString));
			applySettings(newSession.impl());
			customizeSession(newSession);

			PooledSessionHolderPtr pHolder(new PooledSessionHolder(*this, newSession.impl()));
			_idleSessions[pHolder.get()] = pHolder;
			++_nSessions;
		}
		else throw SessionPoolExhaustedException(_connector);
	}

	PooledSessionHolderPtr pHolder(_idleSessions.begin()->second);
	PooledSessionImplPtr pPSI(new PooledSessionImpl(pHolder));

	_activeSessions[pHolder.get()] = pHolder;
	_idleSessions.erase(pHolder.get());
	return Session(pPSI);
}


void SessionPool::purgeDeadSessions()
{
	if (_shutdown) return;

	Poco::Mutex::ScopedLock lock(_mutex);
	SessionList::iterator it = _idleSessions.begin();
	for (; it != _idleSessions.end(); )
	{
		PooledSessionHolderPtr pHolder = it->second;
		if (!pHolder->session()->isConnected())
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


int SessionPool::dead()
{
	int count = 0;

	Poco::Mutex::ScopedLock lock(_mutex);
	SessionList::iterator it = _activeSessions.begin();
	SessionList::iterator itEnd = _activeSessions.end();
	for (; it != itEnd; ++it)
	{
		PooledSessionHolderPtr pHolder = it->second;
		if (!pHolder->session()->isConnected())
			++count;
	}

	return count;
}


int SessionPool::allocated() const
{
	Poco::Mutex::ScopedLock lock(_mutex);
	return _nSessions;
}


int SessionPool::available() const
{
	if (_shutdown) return 0;
	return _maxSessions - used();
}


void SessionPool::setFeature(const std::string& rName, bool state)
{
	if (_shutdown) throw InvalidAccessException("Session pool has been shut down.");

	Poco::Mutex::ScopedLock lock(_mutex);
	if (_nSessions > 0)
		throw InvalidAccessException("Features can not be set after the first session was created.");

	_featureMap.insert(FeatureMap::value_type(rName, state));
}


bool SessionPool::getFeature(const std::string& rName)
{
	if (_shutdown) throw InvalidAccessException("Session pool has been shut down.");

	FeatureMap::const_iterator it = _featureMap.find(rName);
	if (_featureMap.end() == it)
		throw NotFoundException("Feature not found:" + rName);

	return it->second;
}


void SessionPool::setProperty(const std::string& rName, const Poco::Any& value)
{
	if (_shutdown) throw InvalidAccessException("Session pool has been shut down.");

	Poco::Mutex::ScopedLock lock(_mutex);
	if (_nSessions > 0)
		throw InvalidAccessException("Properties can not be set after first session was created.");

	_propertyMap.insert(PropertyMap::value_type(rName, value));
}


Poco::Any SessionPool::getProperty(const std::string& rName)
{
	PropertyMap::const_iterator it = _propertyMap.find(rName);
	if (_propertyMap.end() == it)
		throw NotFoundException("Property not found:" + rName);

	return it->second;
}


void SessionPool::applySettings(SessionImpl::Ptr pImpl)
{
	FeatureMap::iterator fmIt = _featureMap.begin();
	FeatureMap::iterator fmEnd = _featureMap.end();
	for (; fmIt != fmEnd; ++fmIt) pImpl->setFeature(fmIt->first, fmIt->second);

	PropertyMap::iterator pmIt = _propertyMap.begin();
	PropertyMap::iterator pmEnd = _propertyMap.end();
	for (; pmIt != pmEnd; ++pmIt) pImpl->setProperty(pmIt->first, pmIt->second);
}


void SessionPool::customizeSession(Session&)
{
}


void SessionPool::putBack(PooledSessionHolderPtr pHolder)
{
	if (_shutdown) return;

	Poco::Mutex::ScopedLock lock(_mutex);

	PooledSessionHolder* psh = pHolder.get();
	SessionList::iterator it = _activeSessions.find(psh);
	if (it != _activeSessions.end())
	{
		if (pHolder->session()->isConnected())
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
			_idleSessions[pHolder.get()] = pHolder;
		}
		else --_nSessions;

		_activeSessions.erase(it);
	}
	else
	{
		it = _idleSessions.find(psh);
		if (it != _idleSessions.end()) return;
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
		PooledSessionHolderPtr pHolder = it->second;
		if (pHolder->idle() > _idleTime || !pHolder->session()->isConnected())
		{	
			try	{ pHolder->session()->close(); }
			catch (...) { }
			it = _idleSessions.erase(it);
			--_nSessions;
		}
		else ++it;
	}
}


void SessionPool::shutdown()
{
	if (_shutdown) return;
	Poco::Mutex::ScopedLock lock(_mutex);
	_shutdown = true;
	_janitorTimer.stop();
	closeAll(_idleSessions);
	closeAll(_activeSessions);
}


void SessionPool::closeAll(SessionList& sessionList)
{
	SessionList::iterator it = sessionList.begin();
	for (; it != sessionList.end();)
	{
		PooledSessionHolderPtr pHolder = it->second;
		try	{ pHolder->session()->close(); }
		catch (...) { }
		it = sessionList.erase(it);
		if (_nSessions > 0) --_nSessions;
	}
}


} } // namespace Poco::SQL
