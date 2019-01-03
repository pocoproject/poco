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


#ifndef SQL_PooledSessionHolder_INCLUDED
#define SQL_PooledSessionHolder_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/SQL/SessionImpl.h"
#include "Poco/AutoPtr.h"
#include "Poco/Timestamp.h"
#include "Poco/Mutex.h"


namespace Poco {
namespace SQL {


class SessionPool;


class Poco_SQL_API PooledSessionHolder: public Poco::RefCountedObject
	/// This class is used by SessionPool to manage SessionImpl objects.
{
public:
	typedef AutoPtr<PooledSessionHolder> Ptr;

	PooledSessionHolder(SessionPool& owner, SessionImpl::Ptr pSessionImpl);
		/// Creates the PooledSessionHolder.

	~PooledSessionHolder();
		/// Destroys the PooledSessionHolder.

	SessionImpl::Ptr session();
		/// Returns a pointer to the SessionImpl.

	SessionPool& owner();
		/// Returns a reference to the SessionHolder's owner.

	void access();
		/// Updates the last access timestamp.

	int idle() const;
		/// Returns the number of seconds the session has not been used.

private:
	SessionPool&            _owner;
	SessionImpl::Ptr        _pImpl;
	Poco::Timestamp         _lastUsed;
	mutable Poco::FastMutex _mutex;
};


//
// inlines
//
inline SessionImpl::Ptr PooledSessionHolder::session()
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


} } // namespace Poco::SQL


#endif // Data_PooledSessionHolder_INCLUDED
