//
// ConvertTemperatureClient.cpp
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "WebServiceX/ConvertTemperatureClientHelper.h"
#include "Poco/RemotingNG/SOAP/TransportFactory.h"
#include "Poco/RemotingNG/SOAP/Transport.h"
#include <iostream>


int main(int argc, char** argv)
{
	try
	{
		// register transport
		Poco::RemotingNG::SOAP::TransportFactory::registerFactory();
		
		// get proxy for CheckVatService web service
		WebServiceX::IConvertTemperature::Ptr pConvTemp = WebServiceX::ConvertTemperatureClientHelper::find("http://www.webservicex.net/ConvertTemperature.asmx", "soap-1.2");

		// invoke web service
		double convertedTemp;		
		pConvTemp->convertTemp(24, "degreeCelsius", "degreeFahrenheit", convertedTemp);
		std::cout << convertedTemp << std::endl;
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	return 0;
}
