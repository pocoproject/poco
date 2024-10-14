//
// ActiveThreadPool.cpp
//
// Library: Foundation
// Package: Threading
// Module:  ActiveThreadPool
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ActiveThreadPool.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/ThreadLocal.h"
#include "Poco/ErrorHandler.h"
#include "Poco/NotificationQueue.h"
#include <sstream>
#include <ctime>
#include <utility>

namespace Poco {

class NewActionNotification: public Notification
{
public:
	using Ptr = AutoPtr<NewActionNotification>;

	NewActionNotification(Thread::Priority priority, Runnable& runnable, const std::string& name) :
		_priority(priority),
		_runnable(runnable),
		_name(name)
	{
	}

	~NewActionNotification() override = default;

	Runnable& runnable() const
	{
		return _runnable;
	}

	Thread::Priority priority() const
	{
		return _priority;
	}

	const std::string &threadName() const
	{
		return _name;
	}

	std::string threadFullName() const
	{
		std::string fullName(_name);
		if (_name.empty())
		{
		  fullName = _name;
		}
		else
		{
			fullName.append(" (");
			fullName.append(_name);
			fullName.append(")");
		}
		return fullName;
	}

private:
	std::atomic<Thread::Priority> _priority;
	Runnable& _runnable;
	std::string _name;
};

class ActiveThread: public Runnable
{
public:
	ActiveThread(const std::string& name, int stackSize = POCO_THREAD_STACK_SIZE);
	~ActiveThread() override = default;

	void start();
	void start(Thread::Priority priority, Runnable& target);
	void start(Thread::Priority priority, Runnable& target, const std::string& name);
	void join();
	void release();
	void run() override;

private:
	NotificationQueue _pTargetQueue;
	std::string       _name;
	Thread            _thread;
	Event             _targetCompleted;
	FastMutex         _mutex;
	const long        JOIN_TIMEOUT = 10000;
	std::atomic<bool> _needToStop{false};
};


ActiveThread::ActiveThread(const std::string& name, int stackSize):
	_name(name),
	_thread(name),
	_targetCompleted(false)
{
	poco_assert_dbg (stackSize >= 0);
	_thread.setStackSize(stackSize);
}

void ActiveThread::start()
{
	_needToStop = false;
	_thread.start(*this);
}


void ActiveThread::start(Thread::Priority priority, Runnable& target)
{
	_pTargetQueue.enqueueNotification(Poco::makeAuto<NewActionNotification>(priority, target, _name));
}


void ActiveThread::start(Thread::Priority priority, Runnable& target, const std::string& name)
{
	_pTargetQueue.enqueueNotification(Poco::makeAuto<NewActionNotification>(priority, target, name));
}

void ActiveThread::join()
{
	_pTargetQueue.wakeUpAll();
	if (!_pTargetQueue.empty())
	{
		_targetCompleted.wait();
	}

}


void ActiveThread::release()
{
	// In case of a statically allocated thread pool (such
	// as the default thread pool), Windows may have already
	// terminated the thread before we got here.
	if (_thread.isRunning())
	{
		_needToStop = true;
		_pTargetQueue.wakeUpAll();
		if (!_pTargetQueue.empty())
			_targetCompleted.wait(JOIN_TIMEOUT);
	}

	if (_thread.tryJoin(JOIN_TIMEOUT))
	{
		delete this;
	}
}


void ActiveThread::run()
{
	do
	{
		AutoPtr<Notification> pN = _pTargetQueue.waitDequeueNotification();
		while (pN)
		{
			NewActionNotification::Ptr pNAN = pN.cast<NewActionNotification>();
			Runnable& target = pNAN->runnable();
			_thread.setPriority(pNAN->priority());
			_thread.setName(pNAN->name());
			try
			{
				target.run();
			}
			catch (Exception& exc)
			{
				ErrorHandler::handle(exc);
			}
			catch (std::exception& exc)
			{
				ErrorHandler::handle(exc);
			}
			catch (...)
			{
				ErrorHandler::handle();
			}
			_thread.setName(_name);
			_thread.setPriority(Thread::PRIO_NORMAL);
			ThreadLocalStorage::clear();
			pN = _pTargetQueue.waitDequeueNotification(1000);
		}
		_targetCompleted.set();
	}
	while (_needToStop == false);
}


ActiveThreadPool::ActiveThreadPool(int capacity, int stackSize):
	_capacity(capacity),
	_serial(0),
	_stackSize(stackSize),
	_lastThreadIndex(0)
{
	poco_assert (_capacity >= 1);

	_threads.reserve(_capacity);

	for (int i = 0; i < _capacity; i++)
	{
		ActiveThread* pThread = createThread();
		_threads.push_back(pThread);
		pThread->start();
	}
}


ActiveThreadPool::ActiveThreadPool(std::string  name, int capacity, int stackSize):
	_name(std::move(name)),
	_capacity(capacity),
	_serial(0),
	_stackSize(stackSize),
	_lastThreadIndex(0)
{
	poco_assert (_capacity >= 1);

	_threads.reserve(_capacity);

	for (int i = 0; i < _capacity; i++)
	{
		ActiveThread* pThread = createThread();
		_threads.push_back(pThread);
		pThread->start();
	}
}


ActiveThreadPool::~ActiveThreadPool()
{
	try
	{
		stopAll();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


int ActiveThreadPool::capacity() const
{
	return _capacity;
}


void ActiveThreadPool::start(Runnable& target)
{
	getThread()->start(Thread::PRIO_NORMAL, target);
}


void ActiveThreadPool::start(Runnable& target, const std::string& name)
{
	getThread()->start(Thread::PRIO_NORMAL, target, name);
}


void ActiveThreadPool::startWithPriority(Thread::Priority priority, Runnable& target)
{
	getThread()->start(priority, target);
}


void ActiveThreadPool::startWithPriority(Thread::Priority priority, Runnable& target, const std::string& name)
{
	getThread()->start(priority, target, name);
}


void ActiveThreadPool::stopAll()
{
	FastMutex::ScopedLock lock(_mutex);

	for (auto pThread: _threads)
	{
		pThread->release();
	}
	_threads.clear();
}


void ActiveThreadPool::joinAll()
{
	FastMutex::ScopedLock lock(_mutex);

	for (auto pThread: _threads)
	{
		pThread->join();
	}

	_threads.clear();
	_threads.reserve(_capacity);

	for (int i = 0; i < _capacity; i++)
	{
		ActiveThread* pThread = createThread();
		_threads.push_back(pThread);
		pThread->start();
	}
}

ActiveThread* ActiveThreadPool::getThread()
{
	auto thrSize = _threads.size();
	auto i = (_lastThreadIndex++) % thrSize;
	ActiveThread* pThread = _threads[i];
	return pThread;
}


ActiveThread* ActiveThreadPool::createThread()
{
	std::ostringstream name;
	name << _name << "[#active-thread-" << ++_serial << "]";
	return new ActiveThread(name.str(), _stackSize);
}


class ActiveThreadPoolSingletonHolder
{
public:
	ActiveThreadPoolSingletonHolder() = default;
	~ActiveThreadPoolSingletonHolder()
	{
		delete _pPool;
	}
	ActiveThreadPool* pool()
	{
		FastMutex::ScopedLock lock(_mutex);

		if (!_pPool)
		{
			_pPool = new ActiveThreadPool("default-active");
		}
		return _pPool;
	}

private:
	ActiveThreadPool* _pPool{nullptr};
	FastMutex   _mutex;
};


ActiveThreadPool& ActiveThreadPool::defaultPool()
{
	static ActiveThreadPoolSingletonHolder sh;
	return *sh.pool();
}


} // namespace Poco
