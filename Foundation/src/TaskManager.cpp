//
// TaskManager.cpp
//
// Library: Foundation
// Package: Tasks
// Module:  Tasks
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/TaskManager.h"
#include "Poco/TaskNotification.h"
#include "Poco/ThreadPool.h"
#include "Poco/Timespan.h"


namespace Poco {


const int TaskManager::MIN_PROGRESS_NOTIFICATION_INTERVAL = 100000; // 100 milliseconds


TaskManager::TaskManager(const std::string& name,
		int minCapacity,
		int maxCapacity,
		int idleTime,
		int stackSize):
	_threadPool(*new ThreadPool(name, minCapacity, maxCapacity, idleTime, stackSize)),
	_ownPool(true)
{
	// prevent skipping the first progress update
	_lastProgressNotification -= Timespan(MIN_PROGRESS_NOTIFICATION_INTERVAL*2);
}


TaskManager::TaskManager(ThreadPool& pool):
	_threadPool(pool),
	_ownPool(false)
{
	// prevent skipping the first progress update
	_lastProgressNotification -= Timespan(MIN_PROGRESS_NOTIFICATION_INTERVAL*2);
}


TaskManager::~TaskManager()
{
	if (_ownPool) delete &_threadPool;
}


void TaskManager::start(Task* pTask)
{
	TaskPtr pAutoTask(pTask); // take ownership immediately
	pAutoTask->setOwner(this);
	pAutoTask->setState(Task::TASK_STARTING);

	ScopedLockT lock(_mutex);
	_taskList.push_back(pAutoTask);
	try
	{
		_threadPool.start(*pAutoTask, pAutoTask->name());
	}
	catch (...)
	{
		// Make sure that we don't act like we own the task since
		// we never started it.  If we leave the task on our task
		// list, the size of the list is incorrect.
		_taskList.pop_back();
		throw;
	}
}


void TaskManager::cancelAll()
{
	ScopedLockT lock(_mutex);

	for (auto& pTask: _taskList)
	{
		pTask->cancel();
	}
}


void TaskManager::joinAll()
{
	_threadPool.joinAll();
}


TaskManager::TaskList TaskManager::taskList() const
{
	ScopedLockT lock(_mutex);

	return _taskList;
}


void TaskManager::addObserver(const AbstractObserver& observer)
{
	_nc.addObserver(observer);
}


void TaskManager::removeObserver(const AbstractObserver& observer)
{
	_nc.removeObserver(observer);
}


void TaskManager::postNotification(const Notification::Ptr& pNf)
{
	_nc.postNotification(pNf);
}


void TaskManager::taskStarted(Task* pTask)
{
	_nc.postNotification(new TaskStartedNotification(pTask));
}


void TaskManager::taskProgress(Task* pTask, float progress)
{
	ScopedLockWithUnlock<MutexT> lock(_mutex);

	if (_lastProgressNotification.isElapsed(MIN_PROGRESS_NOTIFICATION_INTERVAL))
	{
		_lastProgressNotification.update();
		lock.unlock();
		_nc.postNotification(new TaskProgressNotification(pTask, progress));
	}
}


void TaskManager::taskCancelled(Task* pTask)
{
	_nc.postNotification(new TaskCancelledNotification(pTask));
}


void TaskManager::taskFinished(Task* pTask)
{
	_nc.postNotification(new TaskFinishedNotification(pTask));

	ScopedLockT lock(_mutex);
	for (TaskList::iterator it = _taskList.begin(); it != _taskList.end(); ++it)
	{
		if (*it == pTask)
		{
			_taskList.erase(it);
			break;
		}
	}
}


void TaskManager::taskFailed(Task* pTask, const Exception& exc)
{
	_nc.postNotification(new TaskFailedNotification(pTask, exc));
}


} // namespace Poco
