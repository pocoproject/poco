// 
// Date.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Date_INCLUDED
#define ONVIF_Date_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Date
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Date
{
public:
	Date();

	Date(
		int year, 
		int month, 
		int day);

	virtual ~Date();

	int getDay() const;

	int getMonth() const;

	int getYear() const;

	void setDay(int val);

	void setMonth(int val);

	void setYear(int val);

private:
	//@ name=Year
	//@ order=0
	int _year;

	//@ name=Month
	//@ order=1
	int _month;

	//@ name=Day
	//@ order=2
	int _day;

};


inline int Date::getDay() const
{
	return _day;
}


inline int Date::getMonth() const
{
	return _month;
}


inline int Date::getYear() const
{
	return _year;
}


inline void Date::setDay(int val)
{
	_day = val;
}


inline void Date::setMonth(int val)
{
	_month = val;
}


inline void Date::setYear(int val)
{
	_year = val;
}


} // ONVIF


#endif // ONVIF_Date_INCLUDED
