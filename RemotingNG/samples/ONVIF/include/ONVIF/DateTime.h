// 
// DateTime.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DateTime_INCLUDED
#define ONVIF_DateTime_INCLUDED


#include "ONVIF/Date.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/Time.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=DateTime
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DateTime
{
public:
	DateTime();

	DateTime(
		const Time& time, 
		const Date& date);

	virtual ~DateTime();

	const Date& getDate() const;

	const Time& getTime() const;

	void setDate(const Date& val);

	void setDate(Date&& val);

	void setTime(const Time& val);

	void setTime(Time&& val);

private:
	//@ name=Time
	//@ order=0
	Time _time;

	//@ name=Date
	//@ order=1
	Date _date;

};


inline const Date& DateTime::getDate() const
{
	return _date;
}


inline const Time& DateTime::getTime() const
{
	return _time;
}


inline void DateTime::setDate(const Date& val)
{
	_date = val;
}


inline void DateTime::setDate(Date&& val)
{
	_date = std::move(val);
}


inline void DateTime::setTime(const Time& val)
{
	_time = val;
}


inline void DateTime::setTime(Time&& val)
{
	_time = std::move(val);
}


} // ONVIF


#endif // ONVIF_DateTime_INCLUDED
