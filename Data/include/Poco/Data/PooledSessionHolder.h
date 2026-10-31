//
// PooledSessionHolder.h
//
// Library: Data
// Package: SessionPooling
// Module:  PooledSessionHolder
//
// Definition of the PooledSessionHolder class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_PooledSessionHolder_INCLUDED
#define Data_PooledSessionHolder_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/SessionImpl.h"
#include "Poco/AutoPtr.h"
#include "Poco/Timestamp.h"
#include "Poco/Mutex.h"
#include <atomic>


namespace Poco::Data {


class SessionPool;


class Data_API PooledSessionHolder: public Poco::RefCountedObject
	/// This class is used by SessionPool to manage SessionImpl objects.
{
public:
	PooledSessionHolder(SessionPool& owner, SessionImpl* pSessionImpl);
		/// Creates the PooledSessionHolder and records the auto-commit mode
		/// of the session, which the pool restores when the session is returned.
		///
		/// Throws NullPointerException if pSessionImpl is null.

	~PooledSessionHolder();
		/// Destroys the PooledSessionHolder.

	[[nodiscard]] SessionImpl* session();
		/// Returns a pointer to the SessionImpl.

	[[nodiscard]] SessionPool& owner();
		/// Returns a reference to the SessionHolder's owner.
		/// Must not be called once the holder is orphaned: the owner may be gone.

	void access();
		/// Updates the last access timestamp.

	[[nodiscard]] int idle() const;
		/// Returns the number of seconds the session has not been used.

	[[nodiscard]] bool autoCommit() const;
		/// Returns the auto-commit mode the session had when the holder was created.

	void orphan();
		/// Marks the holder as orphaned: the owner was shut down while the
		/// session was in use, and no longer manages the session.

	[[nodiscard]] bool isOrphaned() const;
		/// Returns true if the holder is orphaned.

private:
	SessionPool& _owner;
	Poco::AutoPtr<SessionImpl> _pImpl;
	Poco::Timestamp _lastUsed;
	bool _autoCommit = false;
	std::atomic<bool> _orphaned{false};
	mutable Poco::FastMutex _mutex;
};


//
// inlines
//
inline SessionImpl* PooledSessionHolder::session()
{
	return _pImpl;
}


inline SessionPool& PooledSessionHolder::owner()
{
	return _owner;
}


inline void PooledSessionHolder::access()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	_lastUsed.update();
}


inline int PooledSessionHolder::idle() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	return (int) (_lastUsed.elapsed()/Poco::Timestamp::resolution());
}


inline bool PooledSessionHolder::autoCommit() const
{
	return _autoCommit;
}


inline void PooledSessionHolder::orphan()
{
	_orphaned = true;
}


inline bool PooledSessionHolder::isOrphaned() const
{
	return _orphaned;
}


} // namespace Poco::Data


#endif // Data_PooledSessionHolder_INCLUDED
