// 
// SetSystemDateAndTime.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetSystemDateAndTime_INCLUDED
#define ONVIF_Device_SetSystemDateAndTime_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class DateTime;
class TimeZone;
} 


namespace ONVIF {
namespace Device {


//@ name="#SetSystemDateAndTime"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetSystemDateAndTime
{
public:
	SetSystemDateAndTime();

	SetSystemDateAndTime(
		const std::string& dateTimeType, 
		bool daylightSavings, 
		const Poco::SharedPtr<ONVIF::TimeZone>& timeZone, 
		const Poco::SharedPtr<ONVIF::DateTime>& uTCDateTime);

	virtual ~SetSystemDateAndTime();

	const std::string& getDateTimeType() const;

	bool getDaylightSavings() const;

	const Poco::SharedPtr<ONVIF::TimeZone>& getTimeZone() const;

	const Poco::SharedPtr<ONVIF::DateTime>& getUTCDateTime() const;

	void setDateTimeType(const std::string& val);

	void setDateTimeType(std::string&& val);

	void setDaylightSavings(bool val);

	void setTimeZone(const Poco::SharedPtr<ONVIF::TimeZone>& val);

	void setTimeZone(Poco::SharedPtr<ONVIF::TimeZone>&& val);

	void setUTCDateTime(const Poco::SharedPtr<ONVIF::DateTime>& val);

	void setUTCDateTime(Poco::SharedPtr<ONVIF::DateTime>&& val);

private:
	//@ name=DateTimeType
	//@ order=0
	std::string _dateTimeType;

	//@ name=DaylightSavings
	//@ order=1
	bool _daylightSavings;

	//@ mandatory=false
	//@ name=TimeZone
	//@ order=2
	Poco::SharedPtr<ONVIF::TimeZone> _timeZone;

	//@ mandatory=false
	//@ name=UTCDateTime
	//@ order=3
	Poco::SharedPtr<ONVIF::DateTime> _uTCDateTime;

};


inline const std::string& SetSystemDateAndTime::getDateTimeType() const
{
	return _dateTimeType;
}


inline bool SetSystemDateAndTime::getDaylightSavings() const
{
	return _daylightSavings;
}


inline const Poco::SharedPtr<ONVIF::TimeZone>& SetSystemDateAndTime::getTimeZone() const
{
	return _timeZone;
}


inline const Poco::SharedPtr<ONVIF::DateTime>& SetSystemDateAndTime::getUTCDateTime() const
{
	return _uTCDateTime;
}


inline void SetSystemDateAndTime::setDateTimeType(const std::string& val)
{
	_dateTimeType = val;
}


inline void SetSystemDateAndTime::setDateTimeType(std::string&& val)
{
	_dateTimeType = std::move(val);
}


inline void SetSystemDateAndTime::setDaylightSavings(bool val)
{
	_daylightSavings = val;
}


inline void SetSystemDateAndTime::setTimeZone(const Poco::SharedPtr<ONVIF::TimeZone>& val)
{
	_timeZone = val;
}


inline void SetSystemDateAndTime::setTimeZone(Poco::SharedPtr<ONVIF::TimeZone>&& val)
{
	_timeZone = std::move(val);
}


inline void SetSystemDateAndTime::setUTCDateTime(const Poco::SharedPtr<ONVIF::DateTime>& val)
{
	_uTCDateTime = val;
}


inline void SetSystemDateAndTime::setUTCDateTime(Poco::SharedPtr<ONVIF::DateTime>&& val)
{
	_uTCDateTime = std::move(val);
}


} } // ONVIF::Device


// The following headers are required for template instantiation.
#include "ONVIF/DateTime.h"
#include "ONVIF/TimeZone.h"


#endif // ONVIF_Device_SetSystemDateAndTime_INCLUDED
