//
// TaskManager.h
//
// Library: Foundation
// Package: Tasks
// Module:  Tasks
//
// Definition of the TaskManager class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_TaskManager_INCLUDED
#define Foundation_TaskManager_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Mutex.h"
#include "Poco/Task.h"
#include "Poco/AutoPtr.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Timestamp.h"
#include <list>


namespace Poco {


class Notification;
class ThreadPool;
class Exception;


class Foundation_API TaskManager
	/// The TaskManager manages a collection of tasks
	/// and monitors their lifetime.
	///
	/// A TaskManager has a built-in NotificationCenter that
	/// is used to send out notifications on task progress
	/// and task states. See the TaskNotification class and its
	/// subclasses for the various events that result in a notification.
	/// To keep the number of notifications small, a TaskProgressNotification
	/// will only be sent out once in 100 milliseconds.
{
public:
	using TaskPtr = AutoPtr<Task>;
	using TaskList = std::list<TaskPtr>;

	TaskManager(const std::string& name = "",
		int minCapacity = 2,
		int maxCapacity = 16,
		int idleTime = 60,
		int stackSize = POCO_THREAD_STACK_SIZE);
		/// Creates the TaskManager.

	TaskManager(ThreadPool& pool);
		/// Creates the TaskManager, using the
		/// given ThreadPool (should be used
		/// by this TaskManager exclusively).

	~TaskManager();
		/// Destroys the TaskManager.

	bool start(Task* pTask);
		/// Starts the given task in a thread obtained
		/// from the thread pool; returns true if successful.
		///
		/// If this method returns false, the task was cancelled
		/// before it could be started, or it was already running;
		/// in any case, a false return means refusal of ownership
		/// and indicates that the task pointer may not be valid
		/// anymore (it will only be valid if it was duplicated
		/// prior to this call).
		///
		/// The TaskManager takes ownership of the Task object
		/// and deletes it when it is finished.

	void cancelAll();
		/// Requests cancellation of all tasks.

	void joinAll();
		/// Waits for the completion of all the threads
		/// in the TaskManager's thread pool.
		///
		/// Note: joinAll() will wait for ALL tasks in the
		/// TaskManager's ThreadPool to complete. If the
		/// ThreadPool has threads created by other
		/// facilities, these threads must also complete
		/// before joinAll() can return.

	TaskList taskList() const;
		/// Returns a copy of the internal task list.

	int count() const;
		/// Returns the number of tasks in the internal task list.

	void addObserver(const AbstractObserver& observer);
		/// Registers an observer with the NotificationCenter.
		/// Usage:
		///     Observer<MyClass, MyNotification> obs(*this, &MyClass::handleNotification);
		///     notificationCenter.addObserver(obs);

	void removeObserver(const AbstractObserver& observer);
		/// Unregisters an observer with the NotificationCenter.

	static const int MIN_PROGRESS_NOTIFICATION_INTERVAL;

protected:
	void postNotification(const Notification::Ptr& pNf);
		/// Posts a notification to the task manager's
		/// notification center.

	void taskStarted(Task* pTask);
	void taskProgress(Task* pTask, float progress);
	void taskCancelled(Task* pTask);
	void taskFinished(Task* pTask);
	void taskFailed(Task* pTask, const Exception& exc);

private:
	using MutexT = FastMutex;
	using ScopedLockT = MutexT::ScopedLock;

	ThreadPool&        _threadPool;
	bool               _ownPool;
	TaskList           _taskList;
	Timestamp          _lastProgressNotification;
	NotificationCenter _nc;
	mutable MutexT     _mutex;

	friend class Task;
};


//
// inlines
//
inline int TaskManager::count() const
{
	ScopedLockT lock(_mutex);

	return (int) _taskList.size();
}


} // namespace Poco


#endif // Foundation_TaskManager_INCLUDED
