//
// EventFilter.h
//
// Library: RemotingNG
// Package: ORB
// Module:  EventFilter
//
// Definition of the EventFilter and friends class templates.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_EventFilter_INCLUDED
#define RemotingNG_EventFilter_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/Clock.h"
#include <cmath>
#include <cstdlib>
#include <limits>


namespace Poco {
namespace RemotingNG {


template <typename T>
class EventFilter: public Poco::RefCountedObject
	/// The interface template for event filters.
{
public:
	using ArgType = T;
	using Ptr = Poco::AutoPtr<EventFilter<T>>;

	EventFilter()
		/// Creates the EventFilter.
	{
	}
	
	virtual ~EventFilter()
		/// Destroys the EventFilter.
	{
	}

	virtual bool accept(const T& value) = 0;
		/// Returns true if the event should be delivered,
		/// false otherwise.
		
private:
	EventFilter(const EventFilter&);
	EventFilter& operator = (const EventFilter&);	
};


template <typename T>
class IsGreaterThanFilter: public EventFilter<T>
	/// This filter only accepts values greater than a given limit.
{
public:
	IsGreaterThanFilter(const T& limit):
		_limit(limit)
	{
	}
	
	// EventFilter
	bool accept(const T& value)
	{
		return value > _limit;
	}
	
private:
	T _limit;
};


template <typename T>
class IsGreaterThanOrEqualToFilter: public EventFilter<T>
	/// This filter only accepts values greater than or equal to a given limit.
{
public:
	IsGreaterThanOrEqualToFilter(const T& limit):
		_limit(limit)
	{
	}
	
	// EventFilter
	bool accept(const T& value)
	{
		return value >= _limit;
	}
	
private:
	T _limit;
};


template <typename T>
class IsLessThanFilter: public EventFilter<T>
	/// This filter only accepts values less than a given limit.
{
public:
	IsLessThanFilter(const T& limit):
		_limit(limit)
	{
	}
	
	// EventFilter
	bool accept(const T& value)
	{
		return value < _limit;
	}
	
private:
	T _limit;
};


template <typename T>
class IsLessThanOrEqualToFilter: public EventFilter<T>
	/// This filter only accepts values less than or equal to a given limit.
{
public:
	IsLessThanOrEqualToFilter(const T& limit):
		_limit(limit)
	{
	}
	
	// EventFilter
	bool accept(const T& value)
	{
		return value <= _limit;
	}
	
private:
	T _limit;
};


template <typename T>
class LogicalOrFilter: public EventFilter<T>
	/// This filter only accepts values if at least one of the sub filters accepts the value.
{
public:
	LogicalOrFilter(typename EventFilter<T>::Ptr pFirst, typename EventFilter<T>::Ptr pSecond):
		_pFirst(pFirst),
		_pSecond(pSecond)
	{
	}
	
	// EventFilter
	bool accept(const T& value)
	{
		return _pFirst->accept(value) || _pSecond->accept(value);
	}
	
private:
	typename EventFilter<T>::Ptr _pFirst;
	typename EventFilter<T>::Ptr _pSecond;
};


template <typename T>
class LogicalAndFilter: public EventFilter<T>
	/// This filter only accepts values if both of the sub filters accept the value.
{
public:
	LogicalAndFilter(typename EventFilter<T>::Ptr pFirst, typename EventFilter<T>::Ptr pSecond):
		_pFirst(pFirst),
		_pSecond(pSecond)
	{
	}
	
	// EventFilter
	bool accept(const T& value)
	{
		return _pFirst->accept(value) && _pSecond->accept(value);
	}
	
private:
	typename EventFilter<T>::Ptr _pFirst;
	typename EventFilter<T>::Ptr _pSecond;
};


template <typename T>
class LogicalXorFilter: public EventFilter<T>
	/// This filter only accepts values if exactly one the sub filters accepts the value.
{
public:
	LogicalXorFilter(typename EventFilter<T>::Ptr pFirst, typename EventFilter<T>::Ptr pSecond):
		_pFirst(pFirst),
		_pSecond(pSecond)
	{
	}
	
	// EventFilter
	bool accept(const T& value)
	{
		return _pFirst->accept(value) != _pSecond->accept(value);
	}
	
private:
	typename EventFilter<T>::Ptr _pFirst;
	typename EventFilter<T>::Ptr _pSecond;
};


template <typename T>
class MinimumDeltaFilter: public EventFilter<T>
	/// This filter only accepts values different from the previous accepted value by
	/// at least a given delta.
{
public:
	MinimumDeltaFilter(const T& delta):
		_delta(delta),
		_lastValue((std::numeric_limits<T>::max)())
	{
	}

	MinimumDeltaFilter(const T& delta, const T& currentValue):
		_delta(delta),
		_lastValue(currentValue)
	{
	}

	// EventFilter
	bool accept(const T& value)
	{
		if (std::abs(value - _lastValue) >= _delta)
		{
			_lastValue = value;
			return true;
		}
		else return false;
	}
	
private:
	T _delta;
	T _lastValue;
};


template <typename T>
class MinimumIntervalFilter: public EventFilter<T>
	/// This filter enforces a minimum time interval (in microseconds)
	/// between accepting values.
{
public:
	MinimumIntervalFilter(Poco::Clock::ClockDiff interval):
		_interval(interval),
		_lastClock(0)
	{
	}

	// EventFilter
	bool accept(const T&)
	{
		if (_lastClock.isElapsed(_interval))
		{
			_lastClock.update();
			return true;
		}
		else return false;
	}
	
private:
	Poco::Clock::ClockDiff _interval;
	Poco::Clock _lastClock;
};


template <typename T>
class MinimumIntervalOrDeltaFilter: public EventFilter<T>
	/// This filter enforces a minimum time interval (in microseconds)
	/// or minimum delta between accepting values.
{
public:
	MinimumIntervalOrDeltaFilter(Poco::Clock::ClockDiff interval, const T& delta):
		_interval(interval),
		_delta(delta),
		_lastClock(0),
		_lastValue((std::numeric_limits<T>::max)())
	{
	}

	MinimumIntervalOrDeltaFilter(Poco::Clock::ClockDiff interval, const T& delta, const T& currentValue):
		_interval(interval),
		_delta(delta),
		_lastClock(0),
		_lastValue(currentValue)
	{
	}

	// EventFilter
	bool accept(const T& value)
	{
		if (_lastClock.isElapsed(_interval) || std::abs(value - _lastValue) >= _delta)
		{
			_lastClock.update();
			_lastValue = value;
			return true;
		}
		else return false;
	}
	
private:
	Poco::Clock::ClockDiff _interval;
	T _delta;
	Poco::Clock _lastClock;
	T _lastValue;
};


template <typename T>
class MinimumIntervalAndDeltaFilter: public EventFilter<T>
	/// This filter enforces a minimum time interval (in microseconds)
	/// and minimum delta between accepting values.
{
public:
	MinimumIntervalAndDeltaFilter(Poco::Clock::ClockDiff interval, const T& delta):
		_interval(interval),
		_delta(delta),
		_lastClock(0),
		_lastValue((std::numeric_limits<T>::max)())
	{
	}

	MinimumIntervalAndDeltaFilter(Poco::Clock::ClockDiff interval, const T& delta, const T& currentValue):
		_interval(interval),
		_delta(delta),
		_lastClock(0),
		_lastValue(currentValue)
	{
	}

	// EventFilter
	bool accept(const T& value)
	{
		if (_lastClock.isElapsed(_interval) && std::abs(value - _lastValue) >= _delta)
		{
			_lastClock.update();
			_lastValue = value;
			return true;
		}
		else return false;
	}
	
private:
	Poco::Clock::ClockDiff _interval;
	T _delta;
	Poco::Clock _lastClock;
	T _lastValue;
};


template <typename T>
class HysteresisFilter: public EventFilter<T>
	/// This filter accepts the value if it falls below the lower threshold
	/// or rises above the upper threshold. This can be used to implement
	/// a "thermostat"-like behavior.
{
	enum State
	{
		STATE_INITIAL, /// initial state, before current value is known
		STATE_ON,      /// i.e. temperature below lower threshold, turn heating on
		STATE_OFF      /// i.e. temperature above upper threshold, turn heating off
	};
	
public:
	HysteresisFilter(const T& lowerThreshold, const T& upperThreshold):
		_lowerThreshold(lowerThreshold),
		_upperThreshold(upperThreshold),
		_state(STATE_INITIAL)
	{
	}

	HysteresisFilter(const T& lowerThreshold, const T& upperThreshold, const T& currentValue):
		_lowerThreshold(lowerThreshold),
		_upperThreshold(upperThreshold),
		_state(STATE_INITIAL)
	{
		accept(currentValue);
	}

	// EventFilter
	bool accept(const T& value)
	{
		switch (_state)
		{
		case STATE_INITIAL:
			if (value < _lowerThreshold)
			{
				_state = STATE_ON;
				return true;
			}
			else if (value > _upperThreshold)
			{
				_state = STATE_OFF;
				return true;
			}
			else
			{
				_state = STATE_ON;
				return false;
			}
		
		case STATE_OFF:
			if (value < _lowerThreshold)
			{
				_state = STATE_ON;
				return true;
			}
			else return false;
			
		case STATE_ON:
			if (value > _upperThreshold)
			{
				_state = STATE_OFF;
				return true;
			}
			else return false;
			
		default:
			poco_bugcheck();
			return false;
		}
	}
	
private:
	T _lowerThreshold;
	T _upperThreshold;
	State _state;
};


} } // namespace Poco::RemotingNG


#endif // RemotingNG_EventFilter_INCLUDED
