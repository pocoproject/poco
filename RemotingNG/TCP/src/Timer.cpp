//
// Timer.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  Timer
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/TCP/Timer.h"
#include "Poco/ErrorHandler.h"


using Poco::ErrorHandler;


namespace Poco {
namespace RemotingNG {
namespace TCP {


TimerTask::TimerTask():
	_isCancelled(false)
{
}


TimerTask::~TimerTask()
{
}


void TimerTask::cancel()
{
	_isCancelled = true;
}


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
	
	virtual bool execute() = 0;
	
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
	
	bool execute()
	{
		queue().clear();
		return false;
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
	
	bool execute()
	{
		queue().clear();
		_finished.set();
		return true;
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
	
	bool execute()
	{
		if (!_pTask->isCancelled())
		{
			try
			{
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
		return true;
	}
			
private:
	TimerTask::Ptr _pTask;
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
	
	bool execute()
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
		return true;			
	}
	
private:
	long _interval;
	Poco::Clock _nextExecution;
};


Timer::Timer()
{
	_thread.start(*this);
}


Timer::~Timer()
{
	try
	{
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


void Timer::scheduleAtFixedRate(TimerTask::Ptr pTask, long delay, long interval)
{
	Poco::Clock time;
	time += static_cast<Poco::Clock::ClockDiff>(delay)*1000;
	_queue.enqueueNotification(new FixedRateTaskNotification(_queue, pTask, interval, time), time);
}


void Timer::run()
{
	bool cont = true;
	while (cont)
	{
		Poco::AutoPtr<TimerNotification> pNf = static_cast<TimerNotification*>(_queue.waitDequeueNotification());
		cont = pNf->execute();
	}
}


} } } // namespace Poco::RemotingNG::TCP
