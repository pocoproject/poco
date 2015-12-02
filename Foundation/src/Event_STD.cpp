//
// Event_STD.cpp
//
// $Id: //poco/1.4/Foundation/src/Event_STD.cpp#1 $
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


#include "Poco/Event_STD.h"
#include <chrono>
#include <system_error>


namespace Poco {


EventImpl::EventImpl(EventTypeImpl type) :
	_cond(), _mutex(), _state(false), _autoreset(type == EVENT_AUTORESET_IMPL)
{
}


EventImpl::~EventImpl()
{
}


void EventImpl::setImpl()
{
	try
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_state = true;
		if (_autoreset)
			_cond.notify_one();
		else
			_cond.notify_all();
	}
	catch (std::system_error &e) {
		throw SystemException(e.what());
	}
}


void EventImpl::resetImpl()
{
	try
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_state = false;
	}
	catch (std::system_error &e) {
		throw SystemException(e.what());
	}
}


void EventImpl::waitImpl()
{
	try
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_cond.wait(lock, [this]() {return this->_state.load(); });
		if (_autoreset)
			_state = false;
	}
	catch (std::system_error &e) {
		throw SystemException(e.what());
	}
}


bool EventImpl::waitImpl(long milliseconds)
{
	try
	{
		std::unique_lock<std::mutex> lock(_mutex);
		bool ret = _cond.wait_for(lock, std::chrono::milliseconds(milliseconds), [this]() {return this->_state.load(); });
		if (ret && _autoreset)
			_state = false;
		return ret;
	}
	catch (std::system_error &e) {
		throw SystemException(e.what());
	}
}


} // namespace Poco
