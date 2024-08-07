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

namespace Poco {

class QueuePage
{
public:
	enum
	{
		MaxPageSize = 256
	};

	QueuePage(Runnable& runnable, int priority):
		_priority(priority)
	{
		push(runnable);
	}

	bool isFull()
	{
		return _lastIndex >= MaxPageSize - 1;
	}

	bool isFinished()
	{
		return _firstIndex > _lastIndex;
	}

	void push(Runnable& runnable)
	{
		poco_assert(!isFull());
		_lastIndex += 1;
		_entries[_lastIndex] = &runnable;
	}

	void skipToNextOrEnd()
	{
		while (!isFinished() && _entries[_firstIndex] == nullptr)
		{
			_firstIndex += 1;
		}
	}

	Runnable* first()
	{
		poco_assert(!isFinished());
		Runnable* runnable = _entries[_firstIndex];
		poco_assert(runnable);
		return runnable;
	}

	Runnable* pop()
	{
		poco_assert(!isFinished());
		Runnable* runnable = first();
		poco_assert(runnable);

		// clear the entry although this should not be necessary
		_entries[_firstIndex] = nullptr;
		_firstIndex += 1;

		// make sure the next runnable returned by first() is not a nullptr
		skipToNextOrEnd();

		return runnable;
	}

	bool tryTake(Runnable* runnable)
	{
		poco_assert(!isFinished());
		for (int i = _firstIndex; i <= _lastIndex; i++)
		{
			if (_entries[i] == runnable)
			{
				_entries[i] = nullptr;
				if (i == _firstIndex)
				{
					// make sure first() does not return a nullptr
					skipToNextOrEnd();
				}
				return true;
			}
		}
		return false;
	}

	int priority() const
	{
		return _priority;
	}

private:
	int _priority = 0;
	int _firstIndex = 0;
	int _lastIndex = -1;
	Runnable* _entries[MaxPageSize];
};


class ActivePooledThread: public Runnable
{
public:
	ActivePooledThread(ActiveThreadPoolPrivate* pool);

	void start();
	void join();
	bool isRunning() const;

	void setRunnable(Runnable& target);
	void notifyRunnableReady();
	void registerThreadInactive();

	virtual void run() override;

private:
	ActiveThreadPoolPrivate* _pool;
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
	std::list<QueuePage*> runnables;
	Condition noActiveThreads;

	int expiryTimeout = 30000;
	int maxThreadCount;
	int stackSize;
	int activeThreads = 0;
	int serial = 0;
};


ActivePooledThread::ActivePooledThread(ActiveThreadPoolPrivate* pool):
	_pool(pool),
	_pTarget(nullptr)
{
	poco_assert(_pool != nullptr);
	std::ostringstream name;
	name << _pool->name << "[#" << ++_pool->serial << "]";
	_thread.setName(name.str());
	_thread.setStackSize(_pool->stackSize);
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
	FastMutex::ScopedLock lock(_pool->mutex);
	for (;;)
	{
		Runnable* r = _pTarget;
		_pTarget = nullptr;

		do
		{
			if (r)
			{
				_pool->mutex.unlock();
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
				_pool->mutex.lock();
			}

			if (_pool->runnables.empty())
			{
				r = nullptr;
				break;
			}

			QueuePage* page = _pool->runnables.front();
			r = page->pop();

			if (page->isFinished())
			{
				_pool->runnables.pop_front();
				delete page;
			}
		} while (true);

		_pool->waitingThreads.push_back(this);
		registerThreadInactive();
		// wait for work, exiting after the expiry timeout is reached
		_runnableReady.tryWait(_pool->mutex, _pool->expiryTimeout);
		++_pool->activeThreads;

		auto it = std::find(_pool->waitingThreads.begin(), _pool->waitingThreads.end(), this);
		if (it != _pool->waitingThreads.end())
		{
			_pool->waitingThreads.erase(it);
			_pool->expiredThreads.push_back(this);
			registerThreadInactive();
			break;
		}

		if (!_pool->allThreads.count(this))
		{
			registerThreadInactive();
			break;
		}
	}
}


void ActivePooledThread::registerThreadInactive()
{
	if (--_pool->activeThreads == 0)
	{
		_pool->noActiveThreads.broadcast();
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


inline bool comparePriority(int priority, const QueuePage* p)
{
	return p->priority() < priority;
}


void ActiveThreadPoolPrivate::enqueueTask(Runnable& target, int priority)
{
	for (QueuePage* page : runnables)
	{
		if (page->priority() == priority && !page->isFull())
		{
			page->push(target);
			return;
		}
	}
	auto it = std::upper_bound(runnables.begin(), runnables.end(), priority, comparePriority);
	runnables.insert(it, new QueuePage(target, priority));
}


int ActiveThreadPoolPrivate::activeThreadCount() const
{
	return (int)(allThreads.size() - expiredThreads.size() - waitingThreads.size());
}


void ActiveThreadPoolPrivate::startThread(Runnable& target)
{
	auto pThread = new ActivePooledThread(this);
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
	delete m_impl;
	m_impl = nullptr;
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
