//
// TimeClient.cpp
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "TimeServiceClientHelper.h"
#include "Poco/RemotingNG/TCP/TransportFactory.h"
#include "Poco/RemotingNG/TCP/Listener.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Delegate.h"
#include "Poco/Format.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Thread.h"
#include "Poco/Process.h"
#include "Poco/File.h"
#include "Poco/TemporaryFile.h"
#include <iostream>


void onWakeUp(const Services::WakeUpEvent& ev)
{
	std::cout << "WakeUp: " << ev.message << std::endl;
}


int main(int argc, char** argv)
{
	try
	{
		// register transport
		Poco::RemotingNG::TCP::TransportFactory::registerFactory();

		// delete socket file if it already exists
		Poco::File socketFile(Poco::format("/tmp/TimeClient.%s.sock", Poco::NumberFormatter::format(Poco::Process::id())));
		if (socketFile.exists()) socketFile.remove();
		Poco::TemporaryFile::registerForDeletion(socketFile.path());

		Poco::RemotingNG::TCP::Listener::Ptr pListener = new Poco::RemotingNG::TCP::Listener(socketFile.path());
		pListener->start();
		pListener->makeDefaultListener();

		// get proxy for remote object
		Services::ITimeService::Ptr pClock = Services::TimeServiceClientHelper::find("remoting.unix://%2Ftmp%2FTimeServer.sock/tcp/Services.TimeService/TheClock");

		// invoke method on remote object
		Poco::DateTime dt = pClock->currentTime();
		std::cout << Poco::DateTimeFormatter::format(dt, Poco::DateTimeFormat::SORTABLE_FORMAT) << std::endl;

		// enable events
		pClock->remoting__enableEvents(pListener);
		pClock->wakeUp += Poco::delegate(onWakeUp);

		// schedule wake up call
		Poco::DateTime wakeUpTime;
		wakeUpTime += Poco::Timespan(5000000); // 5 seconds
		pClock->wakeMeUp(wakeUpTime, "Good morning!");

		// wait for event
		Poco::Thread::sleep(10000);

		// disable events
		pClock->wakeUp -= Poco::delegate(onWakeUp);
		pClock->remoting__enableEvents(pListener, false);
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	return 0;
}
