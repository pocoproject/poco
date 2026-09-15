//
// TimeService.h
//
// Definition of the TimeService class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef TimeService_INCLUDED
#define TimeService_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/BasicEvent.h"
#include "Poco/DateTime.h"
#include "Poco/Util/Timer.h"


namespace Services {


//@ serialize
struct WakeUpEvent
{
	std::string message;
};


//@ remote
class TimeService
{
public:
	TimeService();
		/// Creates the TimeService.

	~TimeService();
		/// Destroys the TimeService.

	Poco::BasicEvent<const WakeUpEvent> wakeUp;

	Poco::DateTime currentTime() const;
		/// Returns the current date and time.

	void wakeMeUp(const Poco::DateTime& time, const std::string& message);
		/// Schedules a wakeup call.
		///
		/// Fires the wakeUp event at the given time, passing
		/// the given message as argument.

private:
	Poco::Util::Timer _timer;
};


} // namespace Services


#endif // TimeService_INCLUDED
