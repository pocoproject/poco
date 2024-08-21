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
#include "Poco/ThreadLocal.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Condition.h"
#include <sstream>
#include <set>
#include <list>
#include <queue>

namespace Poco {


class RunnableList
	/// A list of the same priority runnables
{
public:
	RunnableList(Runnable& target, int priority) : _priority(priority)
	{
		push(target);
	}

	int priority() const
	{
		return _priority;
	}

	void push(Runnable& r)
	{
		_runnables.push_back(&r);
	}

	Runnable* pop()
	{
		Runnable* r = _runnables.front();
		_runnables.pop_front();
		return r;
	}

	bool empty() const
	{
		return _runnables.empty();
	}

	// for std::push_heap
	bool operator< (const RunnableList& rhs) const
	{
		return this->_priority < rhs._priority;
	}

private:
	int _priority = 0;
	std::list<Runnable*> _runnables;
};


class RunnablePriorityQueue
	/// A priority queue of runnables
{
public:
	void push(Runnable& target, int priority)
	{
		for (auto& q : _queues)
		{
			if (q->priority() == priority)
			{
				q->push(target);
				return;
			}
		}
		auto q = std::make_shared<RunnableList>(target, priority);
		_queues.push_back(q);
		std::push_heap(_queues.begin(), _queues.end());
	}

	Runnable* pop()
	{
		auto q = _queues.front();
		Runnable* r = q->pop();
		if (q->empty())
		{
			std::pop_heap(_queues.begin(), _queues.end());
			_queues.pop_back();
		}
		return r;
	}

	bool empty() const
	{
		return _queues.empty();
	}

private:
	std::vector<std::shared_ptr<RunnableList>> _queues;
};


class ActivePooledThread: public Runnable
{
public:
	ActivePooledThread(ActiveThreadPoolPrivate& pool);

	void start();
	void join();
	bool isRunning() const;

	void setRunnable(Runnable& target);
	void notifyRunnableReady();
	void registerThreadInactive();

	virtual void run() override;

private:
	ActiveThreadPoolPrivate& _pool;
	Runnable* _pTarget;
	Condition _runnableReady;
	Thread _thread;
};


class ActiveThreadPoolPrivate
{
public:
	ActiveThreadPoolPrivate(int capacity, int stackSize);
	ActiveThreadPoolPrivate(int capacity, int stackSize, const std::string& name);
	~ActiveThreadPoolPrivate();

	bool tryStart(Runnable& target);
	void enqueueTask(Runnable& target, int priority = 0);
	void startThread(Runnable& target);
	void joinAll();

	int activeThreadCount() const;

public:
	mutable FastMutex mutex;
	std::string name;
	std::set<ActivePooledThread*> allThreads;
	std::list<ActivePooledThread*> waitingThreads;
	std::list<ActivePooledThread*> expiredThreads;
	RunnablePriorityQueue runnables;
	Condition noActiveThreads;

	int expiryTimeout = 30000;
	int maxThreadCount;
	int stackSize;
	int activeThreads = 0;
	int serial = 0;
};


ActivePooledThread::ActivePooledThread(ActiveThreadPoolPrivate& pool):
	_pool(pool),
	_pTarget(nullptr)
{
	std::ostringstream name;
	name << _pool.name << "[#" << ++_pool.serial << "]";
	_thread.setName(name.str());
	_thread.setStackSize(_pool.stackSize);
}


void ActivePooledThread::start()
{
	_thread.start(*this);
}


void ActivePooledThread::setRunnable(Runnable& target)
{
	_pTarget = &target;
}


void ActivePooledThread::notifyRunnableReady()
{
	_runnableReady.signal();
}


bool ActivePooledThread::isRunning() const
{
	return _thread.isRunning();
}


void ActivePooledThread::join()
{
	_thread.join();
}


void ActivePooledThread::run()
{
	FastMutex::ScopedLock lock(_pool.mutex);
	for (;;)
	{
		Runnable* r = _pTarget;
		_pTarget = nullptr;

		do
		{
			if (r)
			{
				_pool.mutex.unlock();
				try
				{
					r->run();
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
				ThreadLocalStorage::clear();
				_pool.mutex.lock();
			}

			if (_pool.runnables.empty())
			{
				r = nullptr;
				break;
			}

			r = _pool.runnables.pop();
		} while (true);

		_pool.waitingThreads.push_back(this);
		registerThreadInactive();
		// wait for work, exiting after the expiry timeout is reached
		_runnableReady.tryWait(_pool.mutex, _pool.expiryTimeout);
		++_pool.activeThreads;

		auto it = std::find(_pool.waitingThreads.begin(), _pool.waitingThreads.end(), this);
		if (it != _pool.waitingThreads.end())
		{
			_pool.waitingThreads.erase(it);
			_pool.expiredThreads.push_back(this);
			registerThreadInactive();
			break;
		}

		if (!_pool.allThreads.count(this))
		{
			registerThreadInactive();
			break;
		}
	}
}


void ActivePooledThread::registerThreadInactive()
{
	if (--_pool.activeThreads == 0)
	{
		_pool.noActiveThreads.broadcast();
	}
}


ActiveThreadPoolPrivate::ActiveThreadPoolPrivate(int capacity, int stackSize_):
	maxThreadCount(capacity),
	stackSize(stackSize_)
{
}


ActiveThreadPoolPrivate::ActiveThreadPoolPrivate(int capacity, int stackSize_, const std::string& name_):
	name(name_),
	maxThreadCount(capacity),
	stackSize(stackSize_)
{
}


ActiveThreadPoolPrivate::~ActiveThreadPoolPrivate()
{
	joinAll();
}


bool ActiveThreadPoolPrivate::tryStart(Runnable& target)
{
	if (allThreads.empty())
	{
		startThread(target);
		return true;
	}

	if (activeThreadCount() >= maxThreadCount)
	{
		return false;
	}

	if (!waitingThreads.empty())
	{
		// recycle an available thread
		enqueueTask(target);
		ActivePooledThread* pThread = waitingThreads.front();
		waitingThreads.pop_front();
		pThread->notifyRunnableReady();
		return true;
	}

	if (!expiredThreads.empty())
	{
		// restart an expired thread
		ActivePooledThread* pThread = expiredThreads.front();
		expiredThreads.pop_front();

		++activeThreads;

		// an expired thread must call join() before restart it, or it will cost thread leak
		pThread->join();
		pThread->setRunnable(target);
		pThread->start();
		return true;
	}

	// start a new thread
	startThread(target);
	return true;
}


void ActiveThreadPoolPrivate::enqueueTask(Runnable& target, int priority)
{
	runnables.push(target, priority);
}


int ActiveThreadPoolPrivate::activeThreadCount() const
{
	std::size_t count = allThreads.size() - expiredThreads.size() - waitingThreads.size();
	return static_cast<int>(count);
}


void ActiveThreadPoolPrivate::startThread(Runnable& target)
{
	auto pThread = new ActivePooledThread(*this);
	allThreads.insert(pThread);
	++activeThreads;
	pThread->setRunnable(target);
	pThread->start();
}


void ActiveThreadPoolPrivate::joinAll()
{
	FastMutex::ScopedLock lock(mutex);

	do {
		while (!runnables.empty() || activeThreads != 0)
		{
			noActiveThreads.wait(mutex);
		}

		// move the contents of the set out so that we can iterate without the lock
		std::set<ActivePooledThread*> allThreadsCopy;
		allThreadsCopy.swap(allThreads);
		expiredThreads.clear();
		waitingThreads.clear();
		mutex.unlock();

		for (ActivePooledThread* pThread : allThreadsCopy)
		{
			if (pThread->isRunning())
			{
				pThread->notifyRunnableReady();
			}

			// we must call join() before thread destruction, or it will cost thread leak
			pThread->join();
			delete pThread;
		}

		mutex.lock();

		// More threads can be started during reset(), in that case continue
		// waiting if we still have time left.
	} while (!runnables.empty() || activeThreads != 0);

	while (!runnables.empty() || activeThreads != 0)
	{
		noActiveThreads.wait(mutex);
	}
}


ActiveThreadPool::ActiveThreadPool(int capacity, int stackSize):
	m_impl(new ActiveThreadPoolPrivate(capacity, stackSize))
{
}


ActiveThreadPool::ActiveThreadPool(const std::string& name, int capacity, int stackSize):
	m_impl(new ActiveThreadPoolPrivate(capacity, stackSize, name))
{
}


ActiveThreadPool::~ActiveThreadPool()
{
}


int ActiveThreadPool::capacity() const
{
	return m_impl->maxThreadCount;
}


void ActiveThreadPool::start(Runnable& target, int priority)
{
	FastMutex::ScopedLock lock(m_impl->mutex);

	if (!m_impl->tryStart(target))
	{
		m_impl->enqueueTask(target, priority);

		if (!m_impl->waitingThreads.empty())
		{
			ActivePooledThread* pThread = m_impl->waitingThreads.front();
			m_impl->waitingThreads.pop_front();
			pThread->notifyRunnableReady();
		}
	}
}


void ActiveThreadPool::joinAll()
{
	m_impl->joinAll();
}


ActiveThreadPool& ActiveThreadPool::defaultPool()
{
	static ActiveThreadPool thePool;
	return thePool;
}


int ActiveThreadPool::getStackSize() const
{
	return m_impl->stackSize;
}


int ActiveThreadPool::expiryTimeout() const
{
	return m_impl->expiryTimeout;
}


void ActiveThreadPool::setExpiryTimeout(int expiryTimeout)
{
	if (m_impl->expiryTimeout != expiryTimeout)
	{
		m_impl->expiryTimeout = expiryTimeout;
	}
}


const std::string& ActiveThreadPool::name() const
{
	return m_impl->name;
}

} // namespace Poco
