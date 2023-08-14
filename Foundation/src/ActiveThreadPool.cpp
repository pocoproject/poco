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
	NewActionNotification(Thread::Priority priority, Runnable &runnable, std::string name)
	: _priority(priority), _runnable(runnable), _name(std::move(name))
	{ }

	~NewActionNotification() override = default;

	Runnable& runnable() const
	{
		return _runnable;
	}

	Thread::Priority priotity() const
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
	Thread::Priority _priority;
	Runnable &_runnable;
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
	NotificationQueue    _pTargetQueue;
	std::string          _name;
	Thread               _thread;
	Event                _targetCompleted;
	Event                _started;
	FastMutex            _mutex;
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
    _thread.start(*this);
    _started.wait();
}


void ActiveThread::start(Thread::Priority priority, Runnable& target)
{
	auto notification = Poco::makeAuto<NewActionNotification>(priority, target, _name);
	_pTargetQueue.enqueueNotification(notification);
}


void ActiveThread::start(Thread::Priority priority, Runnable& target, const std::string& name)
{
	auto notification = Poco::makeAuto<NewActionNotification>(priority, target, name);
	_pTargetQueue.enqueueNotification(notification);
}

void ActiveThread::join()
{
	_pTargetQueue.wakeUpAll();
	if (!_pTargetQueue.empty())
		_targetCompleted.wait();
}


void ActiveThread::release()
{
	const long JOIN_TIMEOUT = 10000;
	// In case of a statically allocated thread pool (such
	// as the default thread pool), Windows may have already
	// terminated the thread before we got here.
	if (_thread.isRunning())
	{
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
	_started.set();
	
	auto *_pTarget = dynamic_cast<NewActionNotification*>(_pTargetQueue.waitDequeueNotification());
	while (_pTarget)
	{
		Runnable* pTarget = &_pTarget->runnable();
		_thread.setPriority(_pTarget->priotity());
		_thread.setName(_pTarget->name());
		try
		{
			pTarget->run();
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
		_pTarget->release();
		_thread.setName(_name);
		_thread.setPriority(Thread::PRIO_NORMAL);
		ThreadLocalStorage::clear();
		_pTarget = dynamic_cast<NewActionNotification*>(_pTargetQueue.waitDequeueNotification());
	}
	_targetCompleted.set();
}


ActiveThreadPool::ActiveThreadPool(int minCapacity,
	int maxCapacity,
	int idleTime,
	int stackSize):
	_minCapacity(minCapacity),
	_maxCapacity(maxCapacity),
	_serial(0),
	_stackSize(stackSize),
	_lastThreadIndex(0)
{
	poco_assert (minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0);

	_threads.reserve(_maxCapacity);

	for (int i = 0; i < _maxCapacity; i++)
	{
		ActiveThread* pThread = createThread();
		_threads.push_back(pThread);
		pThread->start();
	}
}


ActiveThreadPool::ActiveThreadPool(std::string  name,
	int minCapacity,
	int maxCapacity,
	int idleTime,
	int stackSize):
	_name(std::move(name)),
	_minCapacity(minCapacity),
	_maxCapacity(maxCapacity),
	_serial(0),
	_stackSize(stackSize),
	_lastThreadIndex(0)
{
	poco_assert (minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0);

	_threads.reserve(_maxCapacity);

	for (int i = 0; i < _maxCapacity; i++)
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
	return _maxCapacity;
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
	name << _name << "[#active" << ++_serial << "]";
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


namespace
{
	static ActiveThreadPoolSingletonHolder sh;
}


ActiveThreadPool& ActiveThreadPool::defaultPool()
{
	return *sh.pool();
}


} // namespace Poco
