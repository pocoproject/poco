// 
// GetGeoLocationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetGeoLocationResponse_INCLUDED
#define ONVIF_Device_GetGeoLocationResponse_INCLUDED


#include "ONVIF/LocationEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetGeoLocationResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetGeoLocationResponse
{
public:
	GetGeoLocationResponse();

	GetGeoLocationResponse(const std::vector<ONVIF::LocationEntity>& location);

	virtual ~GetGeoLocationResponse();

	const std::vector<ONVIF::LocationEntity>& getLocation() const;

	std::vector<ONVIF::LocationEntity>& getLocation();

	void setLocation(const std::vector<ONVIF::LocationEntity>& val);

	void setLocation(std::vector<ONVIF::LocationEntity>&& val);

private:
	//@ mandatory=false
	//@ name=Location
	//@ order=0
	std::vector<ONVIF::LocationEntity> _location;

};


inline const std::vector<ONVIF::LocationEntity>& GetGeoLocationResponse::getLocation() const
{
	return _location;
}


inline std::vector<ONVIF::LocationEntity>& GetGeoLocationResponse::getLocation()
{
	return _location;
}


inline void GetGeoLocationResponse::setLocation(const std::vector<ONVIF::LocationEntity>& val)
{
	_location = val;
}


inline void GetGeoLocationResponse::setLocation(std::vector<ONVIF::LocationEntity>&& val)
{
	_location = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetGeoLocationResponse_INCLUDED
