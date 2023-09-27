//
// ActiveThreadPool.h
//
// Library: Foundation
// Package: Threading
// Module:  ActiveThreadPool
//
// Definition of the ActiveThreadPool class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ActiveThreadPool_INCLUDED
#define Foundation_ActiveThreadPool_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/Environment.h"
#include <vector>


namespace Poco {


class Runnable;
class ActiveThread;


class Foundation_API ActiveThreadPool
	/// A thread pool always keeps a number of threads running, ready
	/// to accept work.
	/// Threads in an active thread pool are re-used
	/// Every thread in the pool has own notification-queue with Runnable
	/// Every Runnable executes on next thread (round-robin model)
	/// The thread pool always keeps fixed number of threads running.
	/// Use case for this pool is running many (more than os-max-thread-count) short live tasks
	/// Round-robin model allow efficiently utilize cpu cores
{
public:
	ActiveThreadPool(int capacity = static_cast<int>(Environment::processorCount()) + 1,
		int stackSize = POCO_THREAD_STACK_SIZE);
		/// Creates a thread pool with fixed capacity threads.
		/// Threads are created with given stack size.

	ActiveThreadPool(std::string  name,
		int capacity = static_cast<int>(Environment::processorCount()) + 1,
		int stackSize = POCO_THREAD_STACK_SIZE);
		/// Creates a thread pool with the given name and fixed capacity threads.
		/// Threads are created with given stack size.

	~ActiveThreadPool();
		/// Currently running threads will remain active
		/// until they complete.

	int capacity() const;
		/// Returns the capacity of threads.

	int getStackSize() const;
		/// Returns the stack size used to create new threads.

	void start(Runnable& target);
		/// Obtains a thread and starts the target.

	void start(Runnable& target, const std::string& name);
		/// Obtains a thread and starts the target.
		/// Assigns the given name to the thread.

	void startWithPriority(Thread::Priority priority, Runnable& target);
		/// Obtains a thread, adjusts the thread's priority, and starts the target.

	void startWithPriority(Thread::Priority priority, Runnable& target, const std::string& name);
		/// Obtains a thread, adjusts the thread's priority, and starts the target.
		/// Assigns the given name to the thread.

	void stopAll();
		/// Stops all running threads and waits for their completion.
		///
		/// Will also delete all thread objects.
		/// If used, this method should be the last action before
		/// the thread pool is deleted.
		///
		/// Note: If a thread fails to stop within 10 seconds
		/// (due to a programming error, for example), the
		/// underlying thread object will not be deleted and
		/// this method will return anyway. This allows for a
		/// more or less graceful shutdown in case of a misbehaving
		/// thread.

	void joinAll();
		/// Waits for all threads to complete.
		///
		/// Note that this will join() underlying
		/// threads and restart them for next tasks.

	const std::string& name() const;
		/// Returns the name of the thread pool,
		/// or an empty string if no name has been
		/// specified in the constructor.

	static ActiveThreadPool& defaultPool();
		/// Returns a reference to the default
		/// thread pool.

protected:
	ActiveThread* getThread();
	ActiveThread* createThread();

private:
	ActiveThreadPool(const ActiveThreadPool& pool);
	ActiveThreadPool& operator = (const ActiveThreadPool& pool);

	typedef std::vector<ActiveThread*> ThreadVec;

	std::string _name;
	int _capacity;
	int _serial;
	int _stackSize;
	ThreadVec _threads;
	mutable FastMutex _mutex;
	std::atomic<size_t> _lastThreadIndex{0};
};


inline int ActiveThreadPool::getStackSize() const
{
	return _stackSize;
}


inline const std::string& ActiveThreadPool::name() const
{
	return _name;
}


} // namespace Poco


#endif // Foundation_ActiveThreadPool_INCLUDED
