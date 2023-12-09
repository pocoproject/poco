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
	for (auto& pTask: _taskList)
		pTask->setOwner(nullptr);

	if (_ownPool) delete &_threadPool;
}


bool TaskManager::start(Task* pTask)
{
	TaskPtr pAutoTask(pTask); // take ownership immediately
	if (pTask->getOwner())
		throw IllegalStateException("Task already owned by another TaskManager");

	if (pTask->state() == Task::TASK_IDLE)
	{
		pTask->setOwner(this);
		pTask->setState(Task::TASK_STARTING);
		try
		{
			_threadPool.start(*pTask, pTask->name());
			ScopedLockT lock(_mutex);
			_taskList.push_back(pAutoTask);
			return true;
		}
		catch (...)
		{
			pTask->setOwner(nullptr);
			throw;
		}
	}

	pTask->setOwner(nullptr);
	return false;
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
			pTask->setOwner(nullptr);
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
