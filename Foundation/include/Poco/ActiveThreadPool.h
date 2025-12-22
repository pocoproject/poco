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
#include "Poco/Environment.h"
#include <memory>


namespace Poco {


class Runnable;
class ActiveThreadPoolPrivate;


class Foundation_API ActiveThreadPool
	/// A thread pool manages and recycles individual Poco::Thread objects
	/// to help reduce thread creation costs in programs that use threads.
	/// 
	/// The thread pool supports a task queue.
	/// When there are no idle threads, tasks are placed in the task queue to wait for execution.
	/// Use case for this pool is running many (more than os-max-thread-count) short live tasks
{
public:
	ActiveThreadPool(int capacity = static_cast<int>(Environment::processorCount()) + 1,
		int stackSize = POCO_THREAD_STACK_SIZE);
		/// Creates a thread pool with a maximum thread count of capacity.
		/// Threads are created with given stack size.

	ActiveThreadPool(const std::string& name,
		int capacity = static_cast<int>(Environment::processorCount()) + 1,
		int stackSize = POCO_THREAD_STACK_SIZE);
		/// Creates a thread pool with the given name and a maximum thread count of capacity.
		/// Threads are created with given stack size.

	~ActiveThreadPool();
		/// Currently running threads will remain active
		/// until they complete.

	int capacity() const;
		/// Returns the capacity of threads.

	int getStackSize() const;
		/// Returns the stack size used to create new threads.

	int expiryTimeout() const;
		/// Returns the thread expiry timeout value in milliseconds.
		/// The default expiryTimeout is 30000 milliseconds (30 seconds).

	void setExpiryTimeout(int expiryTimeout);
		/// Set the thread expiry timeout value in milliseconds.
		/// The default expiryTimeout is 30000 milliseconds (30 seconds).

	void start(Runnable& target, int priority = 0);
		/// Obtains a thread and starts the target.

	void joinAll();
		/// Waits for all threads to exit and removes all threads from the thread pool.

	const std::string& name() const;
		/// Returns the name of the thread pool,
		/// or an empty string if no name has been
		/// specified in the constructor.

	static ActiveThreadPool& defaultPool();
		/// Returns a reference to the default
		/// thread pool.

private:
	ActiveThreadPool(const ActiveThreadPool& pool);
	ActiveThreadPool& operator = (const ActiveThreadPool& pool);

private:
	std::unique_ptr<ActiveThreadPoolPrivate> _impl;
};

} // namespace Poco


#endif // Foundation_ActiveThreadPool_INCLUDED
