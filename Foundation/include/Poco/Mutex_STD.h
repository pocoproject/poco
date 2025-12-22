//
// Mutex_STD.h
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Definition of the MutexImpl and FastMutexImpl classes based on Standard library mutex and recursive mutes.
//
// Copyright (c) 2004-2023, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Mutex_STD_INCLUDED
#define Foundation_Mutex_STD_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include <mutex>


namespace Poco {


	class Foundation_API MutexImpl
	{
	protected:
		MutexImpl();
		~MutexImpl();
		void lockImpl();
		bool tryLockImpl();
		bool tryLockImpl(long milliseconds);
		void unlockImpl();

	private:
		std::recursive_mutex _mutex;
	};


	class Foundation_API FastMutexImpl
	{
	protected:
		FastMutexImpl();
		~FastMutexImpl();
		void lockImpl();
		bool tryLockImpl();
		bool tryLockImpl(long milliseconds);
		void unlockImpl();
	private:
		std::mutex _mutex;
	};


//
// inlines
//
	inline void MutexImpl::lockImpl()
	{
		try
		{
		_mutex.lock();
		}
		catch (std::exception &ex) {
			throw SystemException("cannot lock mutex", ex.what());
		}
	}


	inline bool MutexImpl::tryLockImpl()
	{
		try
		{
			return _mutex.try_lock();
		}
		catch (std::exception &ex)
		{
			throw SystemException("cannot lock mutex", ex.what());
		}
	}


	inline void MutexImpl::unlockImpl()
	{
		try
		{
			_mutex.unlock();
		}
		catch (std::exception &ex) {
			throw SystemException("cannot unlock mutex");
		}
	}

	inline void FastMutexImpl::lockImpl()
	{
		try
		{
			_mutex.lock();
		}
		catch (std::exception &ex) {
			throw SystemException("cannot lock mutex", ex.what());
		}
	}


	inline bool FastMutexImpl::tryLockImpl()
	{
		try
		{
			return _mutex.try_lock();
		}
		catch (std::exception &ex)
		{
			throw SystemException("cannot lock mutex", ex.what());
		}
	}


	inline void FastMutexImpl::unlockImpl()
	{
		try
		{
			_mutex.unlock();
		}
		catch (std::exception &ex) {
			throw SystemException("cannot unlock mutex");
		}
	}

} // namespace Poco

#endif //Foundation_Mutex_STD_INCLUDED
