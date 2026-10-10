// 
// DateTimeRange.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DateTimeRange_INCLUDED
#define ONVIF_DateTimeRange_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=DateTimeRange
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DateTimeRange
{
public:
	DateTimeRange();

	DateTimeRange(
		const Poco::DateTime& from, 
		const Poco::DateTime& until);

	virtual ~DateTimeRange();

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getFrom() const;

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getUntil() const;

	//@ $val={xsdType=dateTime}
	void setFrom(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setFrom(Poco::DateTime&& val);

	//@ $val={xsdType=dateTime}
	void setUntil(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setUntil(Poco::DateTime&& val);

private:
	//@ name=From
	//@ order=0
	//@ xsdType=dateTime
	Poco::DateTime _from;

	//@ name=Until
	//@ order=1
	//@ xsdType=dateTime
	Poco::DateTime _until;

};


inline const Poco::DateTime& DateTimeRange::getFrom() const
{
	return _from;
}


inline const Poco::DateTime& DateTimeRange::getUntil() const
{
	return _until;
}


inline void DateTimeRange::setFrom(const Poco::DateTime& val)
{
	_from = val;
}


inline void DateTimeRange::setFrom(Poco::DateTime&& val)
{
	_from = std::move(val);
}


inline void DateTimeRange::setUntil(const Poco::DateTime& val)
{
	_until = val;
}


inline void DateTimeRange::setUntil(Poco::DateTime&& val)
{
	_until = std::move(val);
}


} // ONVIF


#endif // ONVIF_DateTimeRange_INCLUDED
