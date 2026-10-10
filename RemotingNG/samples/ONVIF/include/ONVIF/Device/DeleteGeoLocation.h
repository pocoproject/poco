// 
// DeleteGeoLocation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_DeleteGeoLocation_INCLUDED
#define ONVIF_Device_DeleteGeoLocation_INCLUDED


#include "ONVIF/LocationEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#DeleteGeoLocation"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API DeleteGeoLocation
{
public:
	DeleteGeoLocation();

	DeleteGeoLocation(const std::vector<ONVIF::LocationEntity>& location);

	virtual ~DeleteGeoLocation();

	const std::vector<ONVIF::LocationEntity>& getLocation() const;

	std::vector<ONVIF::LocationEntity>& getLocation();

	void setLocation(const std::vector<ONVIF::LocationEntity>& val);

	void setLocation(std::vector<ONVIF::LocationEntity>&& val);

private:
	//@ name=Location
	//@ order=0
	std::vector<ONVIF::LocationEntity> _location;

};


inline const std::vector<ONVIF::LocationEntity>& DeleteGeoLocation::getLocation() const
{
	return _location;
}


inline std::vector<ONVIF::LocationEntity>& DeleteGeoLocation::getLocation()
{
	return _location;
}


inline void DeleteGeoLocation::setLocation(const std::vector<ONVIF::LocationEntity>& val)
{
	_location = val;
}


inline void DeleteGeoLocation::setLocation(std::vector<ONVIF::LocationEntity>&& val)
{
	_location = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_DeleteGeoLocation_INCLUDED
