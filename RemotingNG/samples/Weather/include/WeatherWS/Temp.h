// 
// Temp.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_Temp_INCLUDED
#define WeatherWS_Temp_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace WeatherWS {


//@ name=temp
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class Temp
{
public:
	Temp();

	Temp(
		const Poco::Optional<std::string>& morningLow, 
		const Poco::Optional<std::string>& daytimeHigh);

	virtual ~Temp();

	const Poco::Optional<std::string>& getDaytimeHigh() const;

	const Poco::Optional<std::string>& getMorningLow() const;

	void setDaytimeHigh(const Poco::Optional<std::string>& val);

	void setMorningLow(const Poco::Optional<std::string>& val);

private:
	//@ mandatory=false
	//@ name=MorningLow
	//@ order=0
	Poco::Optional<std::string> _morningLow;

	//@ mandatory=false
	//@ name=DaytimeHigh
	//@ order=1
	Poco::Optional<std::string> _daytimeHigh;

};


inline const Poco::Optional<std::string>& Temp::getDaytimeHigh() const
{
	return _daytimeHigh;
}


inline const Poco::Optional<std::string>& Temp::getMorningLow() const
{
	return _morningLow;
}


inline void Temp::setDaytimeHigh(const Poco::Optional<std::string>& val)
{
	_daytimeHigh = val;
}


inline void Temp::setMorningLow(const Poco::Optional<std::string>& val)
{
	_morningLow = val;
}


} // WeatherWS


#endif // WeatherWS_Temp_INCLUDED
