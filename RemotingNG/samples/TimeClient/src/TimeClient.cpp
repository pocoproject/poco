//
// TimeClient.cpp
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "TimeServiceClientHelper.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/RemotingNG/SOAP/TransportFactory.h"
#include <iostream>


int main(int argc, char** argv)
{
	try
	{
		// register transport
		Poco::RemotingNG::SOAP::TransportFactory::registerFactory();

		// get proxy for remote object
		Services::ITimeService::Ptr pClock = Services::TimeServiceClientHelper::find("http://127.0.0.1:8080/soap/Services.TimeService/TheClock");

		// invoke method on remote object
		Poco::DateTime dt = pClock->currentTime();
		std::cout << Poco::DateTimeFormatter::format(dt, Poco::DateTimeFormat::SORTABLE_FORMAT) << std::endl;
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	return 0;
}
