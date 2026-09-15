// 
// GetServiceCapabilitiesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetServiceCapabilitiesResponse_INCLUDED
#define ONVIF_Media_GetServiceCapabilitiesResponse_INCLUDED


#include "ONVIF/Media/Capabilities.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetServiceCapabilitiesResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetServiceCapabilitiesResponse
{
public:
	GetServiceCapabilitiesResponse();

	GetServiceCapabilitiesResponse(const Capabilities& capabilities);

	virtual ~GetServiceCapabilitiesResponse();

	const Capabilities& getCapabilities() const;

	void setCapabilities(const Capabilities& val);

	void setCapabilities(Capabilities&& val);

private:
	//@ name=Capabilities
	//@ order=0
	Capabilities _capabilities;

};


inline const Capabilities& GetServiceCapabilitiesResponse::getCapabilities() const
{
	return _capabilities;
}


inline void GetServiceCapabilitiesResponse::setCapabilities(const Capabilities& val)
{
	_capabilities = val;
}


inline void GetServiceCapabilitiesResponse::setCapabilities(Capabilities&& val)
{
	_capabilities = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetServiceCapabilitiesResponse_INCLUDED
