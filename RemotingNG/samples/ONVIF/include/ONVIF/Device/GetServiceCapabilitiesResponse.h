// 
// GetServiceCapabilitiesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetServiceCapabilitiesResponse_INCLUDED
#define ONVIF_Device_GetServiceCapabilitiesResponse_INCLUDED


#include "ONVIF/Device/DeviceServiceCapabilities.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetServiceCapabilitiesResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetServiceCapabilitiesResponse
{
public:
	GetServiceCapabilitiesResponse();

	GetServiceCapabilitiesResponse(const DeviceServiceCapabilities& capabilities);

	virtual ~GetServiceCapabilitiesResponse();

	const DeviceServiceCapabilities& getCapabilities() const;

	void setCapabilities(const DeviceServiceCapabilities& val);

	void setCapabilities(DeviceServiceCapabilities&& val);

private:
	//@ name=Capabilities
	//@ order=0
	DeviceServiceCapabilities _capabilities;

};


inline const DeviceServiceCapabilities& GetServiceCapabilitiesResponse::getCapabilities() const
{
	return _capabilities;
}


inline void GetServiceCapabilitiesResponse::setCapabilities(const DeviceServiceCapabilities& val)
{
	_capabilities = val;
}


inline void GetServiceCapabilitiesResponse::setCapabilities(DeviceServiceCapabilities&& val)
{
	_capabilities = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetServiceCapabilitiesResponse_INCLUDED
