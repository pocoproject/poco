// 
// GetDot11CapabilitiesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetDot11CapabilitiesResponse_INCLUDED
#define ONVIF_Device_GetDot11CapabilitiesResponse_INCLUDED


#include "ONVIF/Dot11Capabilities.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetDot11CapabilitiesResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetDot11CapabilitiesResponse
{
public:
	GetDot11CapabilitiesResponse();

	GetDot11CapabilitiesResponse(const ONVIF::Dot11Capabilities& capabilities);

	virtual ~GetDot11CapabilitiesResponse();

	const ONVIF::Dot11Capabilities& getCapabilities() const;

	void setCapabilities(const ONVIF::Dot11Capabilities& val);

	void setCapabilities(ONVIF::Dot11Capabilities&& val);

private:
	//@ name=Capabilities
	//@ order=0
	ONVIF::Dot11Capabilities _capabilities;

};


inline const ONVIF::Dot11Capabilities& GetDot11CapabilitiesResponse::getCapabilities() const
{
	return _capabilities;
}


inline void GetDot11CapabilitiesResponse::setCapabilities(const ONVIF::Dot11Capabilities& val)
{
	_capabilities = val;
}


inline void GetDot11CapabilitiesResponse::setCapabilities(ONVIF::Dot11Capabilities&& val)
{
	_capabilities = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetDot11CapabilitiesResponse_INCLUDED
