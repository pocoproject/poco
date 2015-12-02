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


namespace Poco {


EventImpl::EventImpl(EventTypeImpl type) :
	_cond(), _mutex()
{
}


EventImpl::~EventImpl()
{
}


void EventImpl::waitImpl()
{
	std::unique_lock<std::mutex> lock(_mutex);

	// TODO: must handle spurious unblock?
	_cond.wait(lock);
}


bool EventImpl::waitImpl(long milliseconds)
{
	std::unique_lock<std::mutex> lock(_mutex);

	// TODO: must handle spurious unblock?
	return _cond.wait_for(lock, std::chrono::milliseconds(milliseconds)) == std::cv_status::no_timeout;
}


} // namespace Poco
