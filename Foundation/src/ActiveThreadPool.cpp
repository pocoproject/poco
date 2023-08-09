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
#if defined(_WIN32_WCE) && _WIN32_WCE < 0x800
#include "wce_time.h"
#endif


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
	bool idle();
	int idleTime();
	void join();
	void activate();
	void release();
	void run() override;

private:
	volatile std::atomic<bool> _idle{};
	volatile std::time_t _idleTime{};
	NotificationQueue    _pTargetQueue;
	std::string          _name;
	Thread               _thread;
	Event                _targetCompleted;
	Event                _started;
	FastMutex            _mutex;
};


ActiveThread::ActiveThread(const std::string& name, int stackSize):
	_idle(true),
	_name(name),
	_thread(name),
	_targetCompleted(false)
{
	poco_assert_dbg (stackSize >= 0);
	_thread.setStackSize(stackSize);
#if defined(_WIN32_WCE) && _WIN32_WCE < 0x800
	_idleTime = wceex_time(nullptr);
#else
	_idleTime = std::time(nullptr);
#endif
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


inline bool ActiveThread::idle()
{
	return _idle;
}


int ActiveThread::idleTime()
{
	FastMutex::ScopedLock lock(_mutex);

#if defined(_WIN32_WCE) && _WIN32_WCE < 0x800
	return (int) (wceex_time(nullptr) - _idleTime);
#else
	return (int) (time(nullptr) - _idleTime);
#endif
}


void ActiveThread::join()
{
	_pTargetQueue.wakeUpAll();
	if (!_pTargetQueue.empty())
		_targetCompleted.wait();
}


void ActiveThread::activate()
{
	_idle = true;
	_targetCompleted.reset();
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

		_idle = false;

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
		_idle = true;
		_pTarget = dynamic_cast<NewActionNotification*>(_pTargetQueue.dequeueNotification());
		if (_pTarget != nullptr)
		{
			continue;
		}
		else
		{
			FastMutex::ScopedLock lock(_mutex);
#if defined(_WIN32_WCE) && _WIN32_WCE < 0x800
			_idleTime = wceex_time(nullptr);
#else
			_idleTime = time(nullptr);
#endif
		}
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
	_idleTime(idleTime),
	_serial(0),
	_age(0),
	_stackSize(stackSize),
	_lastThreadIndex(0)
{
	poco_assert (minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0);

	for (int i = 0; i < _minCapacity; i++)
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
	_idleTime(idleTime),
	_serial(0),
	_age(0),
	_stackSize(stackSize),
	_lastThreadIndex(0)
{
	poco_assert (minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0);

	for (int i = 0; i < _minCapacity; i++)
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


void ActiveThreadPool::addCapacity(int n)
{
	FastMutex::ScopedLock lock(_mutex);

	poco_assert (_maxCapacity + n >= _minCapacity);
	_maxCapacity += n;
	housekeep();
}


int ActiveThreadPool::capacity() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _maxCapacity;
}


int ActiveThreadPool::available() const
{
	FastMutex::ScopedLock lock(_mutex);

	int count = 0;
	for (auto pThread: _threads)
	{
		if (pThread->idle()) ++count;
	}
	return (int) (count + _maxCapacity - _threads.size());
}


int ActiveThreadPool::used() const
{
	FastMutex::ScopedLock lock(_mutex);

	int count = 0;
	for (auto pThread: _threads)
	{
		if (!pThread->idle()) ++count;
	}
	return count;
}


int ActiveThreadPool::allocated() const
{
	FastMutex::ScopedLock lock(_mutex);

	return int(_threads.size());
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
	housekeep();
}


void ActiveThreadPool::collect()
{
	FastMutex::ScopedLock lock(_mutex);
	housekeep();
}


void ActiveThreadPool::housekeep()
{
	_age = 0;
	if (_threads.size() <= _minCapacity)
		return;

	ThreadVec idleThreads;
	ThreadVec expiredThreads;
	ThreadVec activeThreads;
	idleThreads.reserve(_threads.size());
	activeThreads.reserve(_threads.size());

	for (auto pThread: _threads)
	{
		if (pThread->idle())
		{
			if (pThread->idleTime() < _idleTime)
				idleThreads.push_back(pThread);
			else
				expiredThreads.push_back(pThread);
		}
		else activeThreads.push_back(pThread);
	}
	int n = (int) activeThreads.size();
	int limit = (int) idleThreads.size() + n;
	if (limit < _minCapacity) limit = _minCapacity;
	idleThreads.insert(idleThreads.end(), expiredThreads.begin(), expiredThreads.end());
	_threads.clear();
	for (auto pIdle: idleThreads)
	{
		if (n < limit)
		{
			_threads.push_back(pIdle);
			++n;
		}
		else pIdle->release();
	}
	_threads.insert(_threads.end(), activeThreads.begin(), activeThreads.end());
}


ActiveThread* ActiveThreadPool::getThread()
{
	FastMutex::ScopedLock lock(_mutex);

	if (++_age == 32)
		housekeep();
	

	ActiveThread* pThread = nullptr;
	auto thrCount = _threads.size();
	auto thrSize = _threads.size();
	while (thrCount-- > 0)
	{
		auto i = (thrCount + _lastThreadIndex) % thrSize;
		if (_threads[i]->idle())
		{
			pThread = _threads[i];
			break;
		}
	}
	_lastThreadIndex = (_lastThreadIndex + 1) % _maxCapacity;
	if (!pThread)
	{
		if (_threads.size() < _maxCapacity)
		{
			pThread = createThread();
			try
			{
				pThread->start();
				_threads.push_back(pThread);
			}
			catch (...)
			{
				delete pThread;
				throw;
			}
		}
		else
		{
			auto i = (thrCount + _lastThreadIndex) % thrSize;
			pThread = _threads[i];
		}
	}
	pThread->activate();
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
			if (POCO_THREAD_STACK_SIZE > 0)
				_pPool->setStackSize(POCO_THREAD_STACK_SIZE);
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
