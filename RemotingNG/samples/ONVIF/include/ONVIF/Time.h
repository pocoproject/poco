// 
// Time.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Time_INCLUDED
#define ONVIF_Time_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Time
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Time
{
public:
	Time();

	Time(
		int hour, 
		int minute, 
		int second);

	virtual ~Time();

	int getHour() const;

	int getMinute() const;

	int getSecond() const;

	void setHour(int val);

	void setMinute(int val);

	void setSecond(int val);

private:
	//@ name=Hour
	//@ order=0
	int _hour;

	//@ name=Minute
	//@ order=1
	int _minute;

	//@ name=Second
	//@ order=2
	int _second;

};


inline int Time::getHour() const
{
	return _hour;
}


inline int Time::getMinute() const
{
	return _minute;
}


inline int Time::getSecond() const
{
	return _second;
}


inline void Time::setHour(int val)
{
	_hour = val;
}


inline void Time::setMinute(int val)
{
	_minute = val;
}


inline void Time::setSecond(int val)
{
	_second = val;
}


} // ONVIF


#endif // ONVIF_Time_INCLUDED
