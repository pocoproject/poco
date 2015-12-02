//
// Event_STD.h
//
// $Id: //poco/1.4/Foundation/include/Poco/Event_STD.h#1 $
//
// Library: Foundation
// Package: Threading
// Module:  Event
//
// Definition of the EventImpl class for STD.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Event_STD_INCLUDED
#define Foundation_Event_STD_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include <condition_variable>
#include <mutex>


namespace Poco {


class Foundation_API EventImpl
{
public:
	enum EventTypeImpl
	{
		EVENT_MANUALRESET_IMPL,
		EVENT_AUTORESET_IMPL,
	};

protected:
	EventImpl(EventTypeImpl type);
	~EventImpl();
	void setImpl();
	void waitImpl();
	bool waitImpl(long milliseconds);
	void resetImpl();
	
private:
	std::condition_variable _cond;
	std::mutex _mutex;
};


//
// inlines
//
inline void EventImpl::setImpl()
{
	std::unique_lock<std::mutex> lock(_mutex);

	_cond.notify_all();
}


inline void EventImpl::resetImpl()
{
	// TODO
}


} // namespace Poco


#endif // Foundation_Event_STD_INCLUDED
