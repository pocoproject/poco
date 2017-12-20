//
// Timer.cpp
//
// Library: Util
// Package: Timer
// Module:  Timer
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/Timer.h"
#include "Poco/Notification.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Event.h"


using Poco::ErrorHandler;


namespace Poco {
namespace Util {


class TimerNotification: public Poco::Notification
{
public:
	TimerNotification(Poco::TimedNotificationQueue& queue):
		_queue(queue)
	{
	}
	
	~TimerNotification()
	{
	}
	
	virtual void execute() = 0;
	
	Poco::TimedNotificationQueue& queue()
	{
		return _queue;
	}
	
private:
	Poco::TimedNotificationQueue& _queue;
};


class StopNotification: public TimerNotification
{
public:
	StopNotification(Poco::TimedNotificationQueue& queue):
		TimerNotification(queue)
	{
	}
	
	~StopNotification()
	{
	}
	
	void execute()
	{
		queue().clear();
		return;
	}
};


class CancelNotification: public TimerNotification
{
public:
	CancelNotification(Poco::TimedNotificationQueue& queue):
		TimerNotification(queue)
	{
	}
	
	~CancelNotification()
	{
	}
	
	void execute()
	{
		queue().clear();
		_finished.set();
		return;
	}
	
	void wait()
	{
		_finished.wait();
	}
	
private:
	Poco::Event _finished;
};


class TaskNotification: public TimerNotification
{
public:
	TaskNotification(Poco::TimedNotificationQueue& queue, TimerTask::Ptr pTask):
		TimerNotification(queue),
		_pTask(pTask)
	{
	}
	
	~TaskNotification()
	{
	}
	
	TimerTask::Ptr task()
	{
		return _pTask;	
	}
	
	void execute()
	{
		if (!_pTask->isCancelled())
		{
			try
			{
				_pTask->_lastExecution.update();
				_pTask->run();
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
		}
		return;
	}
			
private:
	TimerTask::Ptr _pTask;
};


class PeriodicTaskNotification: public TaskNotification
{
public:
	PeriodicTaskNotification(Poco::TimedNotificationQueue& queue, TimerTask::Ptr pTask, long interval):
		TaskNotification(queue, pTask),
		_interval(interval)
	{
	}
	
	~PeriodicTaskNotification()
	{
	}
	
	void execute()
	{	
		TaskNotification::execute();

		if (!task()->isCancelled())
		{
			Poco::Clock now;
			Poco::Clock nextExecution;
			nextExecution += static_cast<Poco::Clock::ClockDiff>(_interval)*1000;
			if (nextExecution < now) nextExecution = now;
			queue().enqueueNotification(this, nextExecution);
			duplicate();
		}
		return;		
	}
	
private:
	long _interval;
};


class FixedRateTaskNotification: public TaskNotification
{
public:
	FixedRateTaskNotification(Poco::TimedNotificationQueue& queue, TimerTask::Ptr pTask, long interval, Poco::Clock clock):
		TaskNotification(queue, pTask),
		_interval(interval),
		_nextExecution(clock)
	{
	}
	
	~FixedRateTaskNotification()
	{
	}
	
	void execute()
	{	
		TaskNotification::execute();

		if (!task()->isCancelled())
		{
			Poco::Clock now;
			_nextExecution += static_cast<Poco::Clock::ClockDiff>(_interval)*1000;
			if (_nextExecution < now) _nextExecution = now;
			queue().enqueueNotification(this, _nextExecution);
			duplicate();
		}
		return;			
	}
	
private:
	long _interval;
	Poco::Clock _nextExecution;
};


Timer::Timer() : _stop( false )
{
	_thread.start(*this);
}


Timer::Timer(Poco::Thread::Priority priority) : _stop(false)
{
	_thread.setPriority(priority);
	_thread.start(*this);
}


Timer::Timer(int prio, int policy) : _stop( false )
{
	_thread.setOSPriority(prio, policy);
	_thread.start(*this);
}


Timer::~Timer()
{
	try
	{
		_stop = true;
		_queue.enqueueNotification(new StopNotification(_queue), Poco::Clock(0));
		_thread.join();
	}
	catch (...)
	{
		poco_unexpected();
	}
}

	
void Timer::cancel(bool wait)
{
	Poco::AutoPtr<CancelNotification> pNf = new CancelNotification(_queue);
	_queue.enqueueNotification(pNf, Poco::Clock(0));
	if (wait)
	{
		pNf->wait();
	}
}


void Timer::schedule(TimerTask::Ptr pTask, Poco::Timestamp time)
{
	validateTask(pTask);
	_queue.enqueueNotification(new TaskNotification(_queue, pTask), time);
}


void Timer::schedule(TimerTask::Ptr pTask, Poco::Clock clock)
{
	validateTask(pTask);
	_queue.enqueueNotification(new TaskNotification(_queue, pTask), clock);
}

	
void Timer::schedule(TimerTask::Ptr pTask, long delay, long interval)
{
	Poco::Clock clock;
	clock += static_cast<Poco::Clock::ClockDiff>(delay)*1000;
	schedule(pTask, clock, interval);
}


void Timer::schedule(TimerTask::Ptr pTask, Poco::Timestamp time, long interval)
{
	validateTask(pTask);
	_queue.enqueueNotification(new PeriodicTaskNotification(_queue, pTask, interval), time);
}


void Timer::schedule(TimerTask::Ptr pTask, Poco::Clock clock, long interval)
{
	validateTask(pTask);
	_queue.enqueueNotification(new PeriodicTaskNotification(_queue, pTask, interval), clock);
}

	
void Timer::scheduleAtFixedRate(TimerTask::Ptr pTask, long delay, long interval)
{
	Poco::Clock clock;
	clock += static_cast<Poco::Clock::ClockDiff>(delay)*1000;
	scheduleAtFixedRate(pTask, clock, interval);
}


void Timer::scheduleAtFixedRate(TimerTask::Ptr pTask, Poco::Timestamp time, long interval)
{
	validateTask(pTask);
	Poco::Timestamp tsNow;
	Poco::Clock clock;
	Poco::Timestamp::TimeDiff diff = time - tsNow;
	clock += diff;
	_queue.enqueueNotification(new FixedRateTaskNotification(_queue, pTask, interval, clock), clock);
}


void Timer::scheduleAtFixedRate(TimerTask::Ptr pTask, Poco::Clock clock, long interval)
{
	validateTask(pTask);
	_queue.enqueueNotification(new FixedRateTaskNotification(_queue, pTask, interval, clock), clock);
}


void Timer::run()
{
	bool terminateThread;
	while (!( terminateThread = _stop && _queue.empty()))
	{
		Poco::AutoPtr<TimerNotification> pNf = static_cast<TimerNotification*>(_queue.waitDequeueNotification());
		pNf->execute();
	}
}


void Timer::validateTask(const TimerTask::Ptr& pTask)
{
	if (pTask->isCancelled())
	{
		throw Poco::IllegalStateException("A cancelled task must not be rescheduled");
	}
}


} } // namespace Poco::Util
