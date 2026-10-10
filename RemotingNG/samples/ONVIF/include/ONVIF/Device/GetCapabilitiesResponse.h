// 
// GetCapabilitiesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetCapabilitiesResponse_INCLUDED
#define ONVIF_Device_GetCapabilitiesResponse_INCLUDED


#include "ONVIF/Capabilities.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetCapabilitiesResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetCapabilitiesResponse
{
public:
	GetCapabilitiesResponse();

	GetCapabilitiesResponse(const ONVIF::Capabilities& capabilities);

	virtual ~GetCapabilitiesResponse();

	const ONVIF::Capabilities& getCapabilities() const;

	void setCapabilities(const ONVIF::Capabilities& val);

	void setCapabilities(ONVIF::Capabilities&& val);

private:
	//@ name=Capabilities
	//@ order=0
	ONVIF::Capabilities _capabilities;

};


inline const ONVIF::Capabilities& GetCapabilitiesResponse::getCapabilities() const
{
	return _capabilities;
}


inline void GetCapabilitiesResponse::setCapabilities(const ONVIF::Capabilities& val)
{
	_capabilities = val;
}


inline void GetCapabilitiesResponse::setCapabilities(ONVIF::Capabilities&& val)
{
	_capabilities = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetCapabilitiesResponse_INCLUDED
