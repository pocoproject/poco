// 
// TimeZone.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_TimeZone_INCLUDED
#define ONVIF_TimeZone_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=TimeZone
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API TimeZone
{
public:
	TimeZone();

	TimeZone(const std::string& tZ);

	virtual ~TimeZone();

	const std::string& getTZ() const;

	void setTZ(const std::string& val);

	void setTZ(std::string&& val);

private:
	//@ name=TZ
	//@ order=0
	std::string _tZ;

};


inline const std::string& TimeZone::getTZ() const
{
	return _tZ;
}


inline void TimeZone::setTZ(const std::string& val)
{
	_tZ = val;
}


inline void TimeZone::setTZ(std::string&& val)
{
	_tZ = std::move(val);
}


} // ONVIF


#endif // ONVIF_TimeZone_INCLUDED
