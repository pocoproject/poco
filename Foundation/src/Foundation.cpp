//
// Foundation.cpp
//
// Library: Foundation
// Package: Core
// Module:  Foundation
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Foundation.h"
#include "Poco/MemoryPool.h"
#include "Poco/RefCountedObject.h"
#include "Poco/Logger.h"
#include "Poco/ThreadPool.h"


// Important note:
//
// This file contains definitions of static and/or global variables
// which depend on each other and require deterministic order of
// creation and destruction. Therefore, the definition order in
// this file should not be changed, nor should these definitions be
// moved elsewhere without a good reason *and* proper understanding
// of the consequences.


namespace Poco {


//
// FastMemoryPool
//

namespace
{
	FastMemoryPool<WeakRefCounter> wrcFMP(POCO_FAST_MEMORY_POOL_PREALLOC);
}


template <typename T>
FastMemoryPool<T>& getFastMemoryPool()
{
	poco_assert(false);
}


template <>
FastMemoryPool<WeakRefCounter>& getFastMemoryPool()
{
	return wrcFMP;
}


#ifdef ENABLE_REFCOUNT_DC


//
// RefCountDiagnosticContext
//

// static RefCountDiagnosticContext members
RCDC::TraceMap RCDC::_traceMap;
SpinlockMutex  RCDC::_mutex;
bool           RCDC::_full = false;


namespace
{
	RefCountDiagnosticContext rcdc;
}


RefCountDiagnosticContext& RefCountDiagnosticContext::get()
{
	return rcdc;
}


#endif // ENABLE_REFCOUNT_DC


//
// Logger
//

// static Logger members
Logger::LoggerMapPtr Logger::_pLoggerMap;
Mutex                Logger::_mapMtx;
const std::string    Logger::ROOT;


class AutoLoggerShutdown
	/// Ensures proper Logger termination.
{
public:
	AutoLoggerShutdown() { }
	~AutoLoggerShutdown()
	{
		try
		{
			Logger::shutdown();
		}
		catch (...)
		{
			poco_unexpected();
		}
	}
};


namespace
{
	static AutoLoggerShutdown als;
}


//
// ThreadPool
//

class ThreadPoolSingletonHolder
{
public:
	ThreadPoolSingletonHolder()
	{
		_pPool = 0;
	}

	~ThreadPoolSingletonHolder()
	{
		delete _pPool;
	}

	ThreadPool* pool(ThreadPool::ThreadAffinityPolicy affinityPolicy = ThreadPool::TAP_DEFAULT)
	{
		FastMutex::ScopedLock lock(_mutex);

		if (!_pPool)
		{
			_pPool = new ThreadPool("default");
			_pPool->setAffinityPolicy(affinityPolicy);
#if (POCO_THREAD_STACK_SIZE > 0)
			_pPool->setStackSize(POCO_THREAD_STACK_SIZE);
#endif
		}
		return _pPool;
	}

private:
	ThreadPool* _pPool;
	FastMutex   _mutex;
};


namespace
{
	static ThreadPoolSingletonHolder sh;
}


ThreadPool& ThreadPool::defaultPool(ThreadAffinityPolicy affinityPolicy)
{
	return *sh.pool(affinityPolicy);
}


} // namespace Poco
