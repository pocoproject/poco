//
// Event.cpp
//
// Library: Foundation
// Package: Threading
// Module:  Event
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Event.h"


namespace Poco {


Event::Event(EventType type): _state(false),
	_autoreset(type == EVENT_AUTORESET)
{
}


Event::Event(bool autoReset): _state(false),
	_autoreset(autoReset)
{
}


Event::~Event()
{
}


void Event::set()
{
	try
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_state = true;
		if (_autoreset) _cond.notify_one();
		else            _cond.notify_all();
	}
	catch (std::system_error &e)
	{
		throw SystemException(e.what());
	}
}


void Event::reset()
{
	try
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_state = false;
	}
	catch (std::system_error &e)
	{
		throw SystemException(e.what());
	}
}


void Event::wait()
{
	try
	{
		std::unique_lock<std::mutex> lock(_mutex);
		while (!_state)
			_cond.wait(lock, [this]() { return this->_state.load(); });
		if (_autoreset) _state = false;
	}
	catch (std::system_error &e)
	{
		throw SystemException(e.what());
	}
}


bool Event::waitImpl(long milliseconds)
{
	try
	{
		std::unique_lock<std::mutex> lock(_mutex);
		bool ret = _cond.wait_for(lock, std::chrono::milliseconds(milliseconds), [this]() { return this->_state.load(); });
		if (ret && _autoreset) _state = false;
		return ret;
	}
	catch (std::system_error &e)
	{
		throw SystemException(e.what());
	}
}


} // namespace Poco
