//
// WeatherClient.cpp
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "WeatherWS/WeatherClientHelper.h"
#include "Poco/RemotingNG/SOAP/TransportFactory.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include <iostream>


int main(int argc, char** argv)
{
	try
	{
		// register transport
		Poco::RemotingNG::SOAP::TransportFactory::registerFactory();
		
		// get proxy for web service
		WeatherWS::IWeather::Ptr pWeather = WeatherWS::WeatherClientHelper::find("http://wsf.cdyne.com/WeatherWS/Weather.asmx", "soap-1.2");

		// invoke web service
		std::string zip("94111");
		WeatherWS::WeatherReturn cityWeather;
		pWeather->getCityWeatherByZIP(zip, cityWeather);
		
		std::cout << "City:          " << cityWeather.getCity().value("n/a") << std::endl;
		std::cout << "Description:   " << cityWeather.getDescription().value("n/a") << std::endl;
		std::cout << "Pressure:      " << cityWeather.getPressure().value("n/a") << std::endl;
		std::cout << "Rel. Humidity: " << cityWeather.getRelativeHumidity().value("n/a") << std::endl;
		std::cout << "Temperature:   " << cityWeather.getTemperature().value("n/a") << std::endl;
		std::cout << "Text:          " << cityWeather.getResponseText().value("n/a") << std::endl;
		std::cout << "Remarks:       " << cityWeather.getRemarks().value("n/a") << std::endl;
		std::cout << std::endl;
		std::cout << "Forecast:" << std::endl;

		Poco::Optional<WeatherWS::ForecastReturn> cityForecast;
		pWeather->getCityForecastByZIP(zip, cityForecast);

		if (cityForecast.isSpecified())
		{
			if (cityForecast.value().getSuccess())
			{
				Poco::Optional<WeatherWS::ArrayOfForecast> ofc = cityForecast.value().getForecastResult();
				if (ofc.isSpecified())
				{
					const WeatherWS::ArrayOfForecast& fc = ofc.value();
					const std::vector<Poco::SharedPtr<WeatherWS::Forecast> >& vec = fc.getForecast();
					for (std::vector<Poco::SharedPtr<WeatherWS::Forecast> >::const_iterator it = vec.begin(); it != vec.end(); ++it)
					{
						if (*it)
						{
							std::cout << "Date/Time:     " << Poco::DateTimeFormatter::format((*it)->getDate(), Poco::DateTimeFormat::SORTABLE_FORMAT) << std::endl;
							std::cout << "Description:   " << (*it)->getDesciption().value("n/a") << std::endl;
							std::cout << "POP Daytime:   " << (*it)->getProbabilityOfPrecipiation().getDaytime().value("n/a") << std::endl;
							std::cout << "POP Nighttime: " << (*it)->getProbabilityOfPrecipiation().getNighttime().value("n/a") << std::endl;
							std::cout << "Daytime High:  " << (*it)->getTemperatures().getDaytimeHigh().value("n/a") << std::endl;
							std::cout << "Morning Low:   " << (*it)->getTemperatures().getMorningLow().value("n/a") << std::endl;
							std::cout << std::endl;
						}
					}
				}
			}
		}
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	return 0;
}
