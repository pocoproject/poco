//
// SessionPool.h
//
// Library: Data
// Package: SessionPooling
// Module:  SessionPool
//
// Definition of the SessionPool class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_SessionPool_INCLUDED
#define Data_SessionPool_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/PooledSessionHolder.h"
#include "Poco/Data/PooledSessionImpl.h"
#include "Poco/Data/Session.h"
#include "Poco/HashMap.h"
#include "Poco/Any.h"
#include "Poco/Timer.h"
#include "Poco/Mutex.h"
#include <list>


namespace Poco::Data {


class Data_API SessionPool: public RefCountedObject
	/// This class implements session pooling for POCO Data.
	///
	/// Creating a connection to a database is often a time consuming
	/// operation. Therefore it makes sense to reuse a session object
	/// once it is no longer needed.
	///
	/// A SessionPool manages a collection of SessionImpl objects
	/// (decorated with a PooledSessionImpl).
	///
	/// When a SessionImpl object is requested, the SessionPool first
	/// looks in its set of already initialized SessionImpl for an
	/// available object. If one is found, it is returned to the
	/// client and marked as "in-use". If no SessionImpl is available,
	/// the SessionPool attempts to create a new one for the client.
	/// To avoid excessive creation of SessionImpl objects, a limit
	/// can be set on the maximum number of objects.
	/// Sessions found not to be connected to the database are purged
	/// from the pool whenever one of the following events occurs:
	///
	///   - JanitorTimer event
	///   - get() request
	///   - putBack() request
	///
	/// Not connected idle sessions can not exist.
	///
	/// When a session is returned to the pool, an open transaction is
	/// rolled back, and so is any uncommitted work of a session in
	/// manual-commit mode; settings changed with get(name, value) are
	/// reverted, the pool settings are re-applied and the auto-commit
	/// mode the session was created with is restored. The rollback runs
	/// before the session is handed back, without locking the pool; a
	/// session whose rollback fails is closed instead, and the error goes
	/// to Poco::ErrorHandler.
	///
	/// A session may be released after its pool has been shut down or
	/// destroyed: shutdown() and the destructor close the idle sessions
	/// only, and a session still in use is closed when it is released.
	/// The pool must not be destroyed while another thread still uses
	/// it, e.g. to get a session or to release one.
	///
	/// A statement created through a pooled Session must not be executed
	/// after the Session has been closed: until the pool closes the
	/// session, the statement would run on a connection that another user
	/// may have; once the pool has closed the session, executing the
	/// statement throws NotConnectedException.
	///
	/// Usage example:
	///
	///     SessionPool pool("ODBC", "...");
	///     ...
	///     Session sess(pool.get());
	///     ...
{
public:
	SessionPool(const std::string& connector,
		const std::string& connectionString,
		int minSessions = 1,
		int maxSessions = 32,
		int idleTime = 60,
		int connTimeout = 60);
		/// Creates the SessionPool for sessions with the given connector
		/// and connectionString.
		///
		/// The pool allows for at most maxSessions sessions to be created.
		/// If a session has been idle for more than idleTime seconds, and more than
		/// minSessions sessions are in the pool, the session is automatically destroyed.

	~SessionPool();
		/// Destroys the SessionPool.
		/// Closes the idle sessions; a session still in use is closed when it is released.

	[[nodiscard]] Session get();
		/// Returns a Session.
		///
		/// If there are unused sessions available, one of the
		/// unused sessions is recycled. Otherwise, a new session
		/// is created.
		///
		/// If the maximum number of sessions for this pool has
		/// already been created, a SessionPoolExhaustedException
		/// is thrown.

	template <typename T>
	[[nodiscard]] Session get(const std::string& name, const T& value)
		/// Returns a Session with requested property set.
		/// The property can be different from the default pool
		/// value, in which case it is reset back to the pool
		/// value when the session is reclaimed by the pool.
	{
		Session s = get();
		PropertyPair property(name, s.getProperty(name));
		s.setProperty(name, value);

		SessionImpl* pImpl = static_cast<PooledSessionImpl*>(s.impl())->impl();
		Poco::Mutex::ScopedLock lock(_mutex);
		_addPropertyMap[pImpl] = property;

		return s;
	}

	[[nodiscard]] Session get(const std::string& name, bool value);
		/// Returns a Session with requested feature set.
		/// The feature can be different from the default pool
		/// value, in which case it is reset back to the pool
		/// value when the session is reclaimed by the pool.

	[[nodiscard]] int capacity() const;
		/// Returns the maximum number of sessions the SessionPool will manage.

	[[nodiscard]] int used() const;
		/// Returns the number of sessions currently in use.

	[[nodiscard]] int idle() const;
		/// Returns the number of idle sessions.

	[[nodiscard]] int connTimeout() const;
		/// Returns the connection timeout.

	[[nodiscard]] int dead();
		/// Returns the number of not connected idle sessions.

	[[nodiscard]] int allocated() const;
		/// Returns the number of allocated sessions.

	[[nodiscard]] int available() const;
		/// Returns the number of available (idle + remaining capacity) sessions.

	[[nodiscard]] std::string name() const;
		/// Returns the name for this pool.

	[[nodiscard]] static std::string name(const std::string& connector,
		const std::string& connectionString);
	/// Returns the name formatted from supplied arguments as "connector:///connectionString".

	void setFeature(const std::string& name, bool state);
		/// Sets feature for all the sessions.

	[[nodiscard]] bool getFeature(const std::string& name) const;
		/// Returns the requested feature.

	void setProperty(const std::string& name, const Poco::Any& value);
		/// Sets property for all sessions.

	[[nodiscard]] Poco::Any getProperty(const std::string& name) const;
		/// Returns the requested property.

	void shutdown();
		/// Shuts down the session pool.
		/// Closes the idle sessions; a session still in use is closed when it is released.

	[[nodiscard]] bool isActive() const;
		/// Returns true if session pool is active (not shut down).

protected:
	virtual void customizeSession(Session& session);
		/// Can be overridden by subclass to perform custom initialization
		/// of a newly created database session.
		///
		/// The default implementation does nothing.

	typedef Poco::AutoPtr<PooledSessionHolder>    PooledSessionHolderPtr;
	typedef Poco::AutoPtr<PooledSessionImpl>      PooledSessionImplPtr;
	typedef std::list<PooledSessionHolderPtr>     SessionList;
	typedef Poco::HashMap<std::string, bool>      FeatureMap;
	typedef Poco::HashMap<std::string, Poco::Any> PropertyMap;

	void purgeDeadSessions();
	void applySettings(SessionImpl* pImpl);
	void putBack(PooledSessionHolderPtr pHolder);
		/// Returns the session to the pool. Does not throw: a session that
		/// can not be reset is closed and discarded, and the error is
		/// reported through Poco::ErrorHandler.
	void onJanitorTimer(Poco::Timer&);

private:
	typedef std::pair<std::string, Poco::Any> PropertyPair;
	typedef std::pair<std::string, bool> FeaturePair;
	typedef std::map<SessionImpl*, PropertyPair> AddPropertyMap;
	typedef std::map<SessionImpl*, FeaturePair> AddFeatureMap;

	SessionPool(const SessionPool&);
	SessionPool& operator = (const SessionPool&);

	void closeAll(SessionList& sessionList);

	std::string       _connector;
	std::string       _connectionString;
	std::atomic<int>  _minSessions;
	std::atomic<int>  _maxSessions;
	std::atomic<int>  _idleTime;
	std::atomic<int>  _connTimeout;
	std::atomic<int>  _nSessions;
	SessionList       _idleSessions;
	SessionList       _activeSessions;
	Poco::Timer       _janitorTimer;
	FeatureMap        _featureMap;
	PropertyMap       _propertyMap;
	std::atomic<bool> _shutdown;
	AddPropertyMap    _addPropertyMap;
	AddFeatureMap     _addFeatureMap;
	mutable
	Poco::Mutex _mutex;

	friend class PooledSessionImpl;
};


inline std::string SessionPool::name(const std::string& connector,
	const std::string& connectionString)
{
	return Session::uri(connector, connectionString);
}


inline std::string SessionPool::name() const
{
	return name(_connector, _connectionString);
}


inline bool SessionPool::isActive() const
{
	return !_shutdown;
}


} // namespace Poco::Data


#endif // Data_SessionPool_INCLUDED
