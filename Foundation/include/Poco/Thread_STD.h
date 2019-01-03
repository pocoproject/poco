//
// Thread_STD.h
//
// Library: Foundation
// Package: Threading
// Module:  Thread
//
// Definition of the ThreadImpl class for STD Threads.
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Thread_STD_INCLUDED
#define Foundation_Thread_STD_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Runnable.h"
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
#include "Poco/SignalHandler.h"
#endif
#include "Poco/Event.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/SharedPtr.h"
#include <thread>


#ifdef __APPLE__
#define thread_local __thread
#elif defined(_MSC_VER)
#include "Poco/UnWindows.h"
#endif


namespace Poco {


class Foundation_API ThreadImpl
{
public:

	typedef std::thread::native_handle_type TIDImpl;

	typedef void (*Callable)(void*);

	enum Priority
	{
		PRIO_LOWEST_IMPL,
		PRIO_LOW_IMPL,
		PRIO_NORMAL_IMPL,
		PRIO_HIGH_IMPL,
		PRIO_HIGHEST_IMPL
	};

	enum Policy
	{
		POLICY_DEFAULT_IMPL = 0
	};

	ThreadImpl();
	~ThreadImpl();

	TIDImpl tidImpl() const;
	void setPriorityImpl(int prio);
	int getPriorityImpl() const;
	void setOSPriorityImpl(int prio, int policy = 0);
	int getOSPriorityImpl() const;
	static int getMinOSPriorityImpl(int policy);
	static int getMaxOSPriorityImpl(int policy);
	void setStackSizeImpl(int size);
	int getStackSizeImpl() const;
	void setAffinityImpl(int cpu);
	int getAffinityImpl() const;
	void startImpl(SharedPtr<Runnable> pTarget);
	void joinImpl();
	bool joinImpl(long milliseconds);
	bool isRunningImpl() const;
	static void sleepImpl(long milliseconds);
	static void yieldImpl();
	static ThreadImpl* currentImpl();
	static TIDImpl currentTidImpl();
	static long currentOsTidImpl();

protected:
	static void* runnableEntry(void* pThread);

private:
	class CurrentThreadHolder
	{
	public:
		CurrentThreadHolder()
		{
		}
		~CurrentThreadHolder()
		{
		}
		ThreadImpl* get() const
		{
			return _pThread;
		}
		void set(ThreadImpl* pThread)
		{
			_pThread = pThread;
		}

	private:
		static thread_local ThreadImpl* _pThread;
	};

	struct ThreadData: public RefCountedObject
	{
		ThreadData():
			thread(),
			prio(PRIO_NORMAL_IMPL),
			policy(0),
			task(0),
			done(Event::EVENT_MANUALRESET),
			stackSize(POCO_THREAD_STACK_SIZE),
			cpu(-1),
			started(false),
			joined(false)
		{
		}

		SharedPtr<Runnable> pRunnableTarget;
		std::unique_ptr<std::thread>     thread;
		TIDImpl       tid;
		int           prio;
		int           osPrio;
		int           policy;
		int           task;
		Event         done;
		std::size_t   stackSize;
		int			  cpu;
		bool          started;
		bool          joined;
	};

	AutoPtr<ThreadData> _pData;

	static CurrentThreadHolder _currentThreadHolder;

#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
	SignalHandler::JumpBufferVec _jumpBufferVec;
	friend class SignalHandler;
#endif
};


//
// inlines
//
inline int ThreadImpl::getPriorityImpl() const
{
	return _pData->prio;
}


inline int ThreadImpl::getOSPriorityImpl() const
{
	return _pData->osPrio;
}


inline bool ThreadImpl::isRunningImpl() const
{
	return !_pData->pRunnableTarget.isNull();
}


inline void ThreadImpl::yieldImpl()
{
	std::this_thread::yield();
}


inline int ThreadImpl::getStackSizeImpl() const
{
	return static_cast<int>(_pData->stackSize);
}


inline ThreadImpl::TIDImpl ThreadImpl::tidImpl() const
{
	return _pData->tid;
}


} // namespace Poco


#endif // Foundation_Thread_STD_INCLUDED
