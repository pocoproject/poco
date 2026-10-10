// 
// Temp.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "WeatherWS/Temp.h"


namespace WeatherWS {


Temp::Temp():
	_morningLow(),
	_daytimeHigh()
{
}


Temp::Temp(const Poco::Optional<std::string>& morningLow, const Poco::Optional<std::string>& daytimeHigh):
	_morningLow(morningLow),
	_daytimeHigh(daytimeHigh)
{
}


Temp::~Temp()
{
}


} // WeatherWS


