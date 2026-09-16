// 
// POP.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WeatherWS_POP_INCLUDED
#define WeatherWS_POP_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace WeatherWS {


//@ name=POP
//@ namespace="http://ws.cdyne.com/WeatherWS/"
//@ serialize
class POP
{
public:
	POP();

	POP(
		const Poco::Optional<std::string>& nighttime, 
		const Poco::Optional<std::string>& daytime);

	virtual ~POP();

	const Poco::Optional<std::string>& getDaytime() const;

	const Poco::Optional<std::string>& getNighttime() const;

	void setDaytime(const Poco::Optional<std::string>& val);

	void setNighttime(const Poco::Optional<std::string>& val);

private:
	//@ mandatory=false
	//@ name=Nighttime
	//@ order=0
	Poco::Optional<std::string> _nighttime;

	//@ mandatory=false
	//@ name=Daytime
	//@ order=1
	Poco::Optional<std::string> _daytime;

};


inline const Poco::Optional<std::string>& POP::getDaytime() const
{
	return _daytime;
}


inline const Poco::Optional<std::string>& POP::getNighttime() const
{
	return _nighttime;
}


inline void POP::setDaytime(const Poco::Optional<std::string>& val)
{
	_daytime = val;
}


inline void POP::setNighttime(const Poco::Optional<std::string>& val)
{
	_nighttime = val;
}


} // WeatherWS


#endif // WeatherWS_POP_INCLUDED
