// 
// SystemDateTime.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SystemDateTime_INCLUDED
#define ONVIF_SystemDateTime_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class DateTime;
class SystemDateTimeExtension;
class TimeZone;
} 


namespace ONVIF {


//@ name=SystemDateTime
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SystemDateTime
{
public:
	SystemDateTime();

	SystemDateTime(
		const std::string& dateTimeType, 
		bool daylightSavings, 
		const Poco::SharedPtr<TimeZone>& timeZone, 
		const Poco::SharedPtr<DateTime>& uTCDateTime, 
		const Poco::SharedPtr<DateTime>& localDateTime, 
		const Poco::SharedPtr<SystemDateTimeExtension>& extension);

	virtual ~SystemDateTime();

	const std::string& getDateTimeType() const;

	bool getDaylightSavings() const;

	const Poco::SharedPtr<SystemDateTimeExtension>& getExtension() const;

	const Poco::SharedPtr<DateTime>& getLocalDateTime() const;

	const Poco::SharedPtr<TimeZone>& getTimeZone() const;

	const Poco::SharedPtr<DateTime>& getUTCDateTime() const;

	void setDateTimeType(const std::string& val);

	void setDateTimeType(std::string&& val);

	void setDaylightSavings(bool val);

	void setExtension(const Poco::SharedPtr<SystemDateTimeExtension>& val);

	void setExtension(Poco::SharedPtr<SystemDateTimeExtension>&& val);

	void setLocalDateTime(const Poco::SharedPtr<DateTime>& val);

	void setLocalDateTime(Poco::SharedPtr<DateTime>&& val);

	void setTimeZone(const Poco::SharedPtr<TimeZone>& val);

	void setTimeZone(Poco::SharedPtr<TimeZone>&& val);

	void setUTCDateTime(const Poco::SharedPtr<DateTime>& val);

	void setUTCDateTime(Poco::SharedPtr<DateTime>&& val);

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
	Poco::SharedPtr<TimeZone> _timeZone;

	//@ mandatory=false
	//@ name=UTCDateTime
	//@ order=3
	Poco::SharedPtr<DateTime> _uTCDateTime;

	//@ mandatory=false
	//@ name=LocalDateTime
	//@ order=4
	Poco::SharedPtr<DateTime> _localDateTime;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<SystemDateTimeExtension> _extension;

};


inline const std::string& SystemDateTime::getDateTimeType() const
{
	return _dateTimeType;
}


inline bool SystemDateTime::getDaylightSavings() const
{
	return _daylightSavings;
}


inline const Poco::SharedPtr<SystemDateTimeExtension>& SystemDateTime::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<DateTime>& SystemDateTime::getLocalDateTime() const
{
	return _localDateTime;
}


inline const Poco::SharedPtr<TimeZone>& SystemDateTime::getTimeZone() const
{
	return _timeZone;
}


inline const Poco::SharedPtr<DateTime>& SystemDateTime::getUTCDateTime() const
{
	return _uTCDateTime;
}


inline void SystemDateTime::setDateTimeType(const std::string& val)
{
	_dateTimeType = val;
}


inline void SystemDateTime::setDateTimeType(std::string&& val)
{
	_dateTimeType = std::move(val);
}


inline void SystemDateTime::setDaylightSavings(bool val)
{
	_daylightSavings = val;
}


inline void SystemDateTime::setExtension(const Poco::SharedPtr<SystemDateTimeExtension>& val)
{
	_extension = val;
}


inline void SystemDateTime::setExtension(Poco::SharedPtr<SystemDateTimeExtension>&& val)
{
	_extension = std::move(val);
}


inline void SystemDateTime::setLocalDateTime(const Poco::SharedPtr<DateTime>& val)
{
	_localDateTime = val;
}


inline void SystemDateTime::setLocalDateTime(Poco::SharedPtr<DateTime>&& val)
{
	_localDateTime = std::move(val);
}


inline void SystemDateTime::setTimeZone(const Poco::SharedPtr<TimeZone>& val)
{
	_timeZone = val;
}


inline void SystemDateTime::setTimeZone(Poco::SharedPtr<TimeZone>&& val)
{
	_timeZone = std::move(val);
}


inline void SystemDateTime::setUTCDateTime(const Poco::SharedPtr<DateTime>& val)
{
	_uTCDateTime = val;
}


inline void SystemDateTime::setUTCDateTime(Poco::SharedPtr<DateTime>&& val)
{
	_uTCDateTime = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/DateTime.h"
#include "ONVIF/SystemDateTimeExtension.h"
#include "ONVIF/TimeZone.h"


#endif // ONVIF_SystemDateTime_INCLUDED
