// 
// POP.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "WeatherWS/POP.h"


namespace WeatherWS {


POP::POP():
	_nighttime(),
	_daytime()
{
}


POP::POP(const Poco::Optional<std::string>& nighttime, const Poco::Optional<std::string>& daytime):
	_nighttime(nighttime),
	_daytime(daytime)
{
}


POP::~POP()
{
}


} // WeatherWS


