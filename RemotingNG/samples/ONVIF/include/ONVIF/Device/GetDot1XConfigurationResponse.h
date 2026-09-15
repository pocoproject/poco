// 
// GetDot1XConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetDot1XConfigurationResponse_INCLUDED
#define ONVIF_Device_GetDot1XConfigurationResponse_INCLUDED


#include "ONVIF/Dot1XConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetDot1XConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetDot1XConfigurationResponse
{
public:
	GetDot1XConfigurationResponse();

	GetDot1XConfigurationResponse(const ONVIF::Dot1XConfiguration& dot1XConfiguration);

	virtual ~GetDot1XConfigurationResponse();

	const ONVIF::Dot1XConfiguration& getDot1XConfiguration() const;

	void setDot1XConfiguration(const ONVIF::Dot1XConfiguration& val);

	void setDot1XConfiguration(ONVIF::Dot1XConfiguration&& val);

private:
	//@ name=Dot1XConfiguration
	//@ order=0
	ONVIF::Dot1XConfiguration _dot1XConfiguration;

};


inline const ONVIF::Dot1XConfiguration& GetDot1XConfigurationResponse::getDot1XConfiguration() const
{
	return _dot1XConfiguration;
}


inline void GetDot1XConfigurationResponse::setDot1XConfiguration(const ONVIF::Dot1XConfiguration& val)
{
	_dot1XConfiguration = val;
}


inline void GetDot1XConfigurationResponse::setDot1XConfiguration(ONVIF::Dot1XConfiguration&& val)
{
	_dot1XConfiguration = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetDot1XConfigurationResponse_INCLUDED
