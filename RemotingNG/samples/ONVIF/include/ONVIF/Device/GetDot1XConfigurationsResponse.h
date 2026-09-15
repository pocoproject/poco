// 
// GetDot1XConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetDot1XConfigurationsResponse_INCLUDED
#define ONVIF_Device_GetDot1XConfigurationsResponse_INCLUDED


#include "ONVIF/Dot1XConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetDot1XConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetDot1XConfigurationsResponse
{
public:
	GetDot1XConfigurationsResponse();

	GetDot1XConfigurationsResponse(const std::vector<ONVIF::Dot1XConfiguration>& dot1XConfiguration);

	virtual ~GetDot1XConfigurationsResponse();

	const std::vector<ONVIF::Dot1XConfiguration>& getDot1XConfiguration() const;

	std::vector<ONVIF::Dot1XConfiguration>& getDot1XConfiguration();

	void setDot1XConfiguration(const std::vector<ONVIF::Dot1XConfiguration>& val);

	void setDot1XConfiguration(std::vector<ONVIF::Dot1XConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=Dot1XConfiguration
	//@ order=0
	std::vector<ONVIF::Dot1XConfiguration> _dot1XConfiguration;

};


inline const std::vector<ONVIF::Dot1XConfiguration>& GetDot1XConfigurationsResponse::getDot1XConfiguration() const
{
	return _dot1XConfiguration;
}


inline std::vector<ONVIF::Dot1XConfiguration>& GetDot1XConfigurationsResponse::getDot1XConfiguration()
{
	return _dot1XConfiguration;
}


inline void GetDot1XConfigurationsResponse::setDot1XConfiguration(const std::vector<ONVIF::Dot1XConfiguration>& val)
{
	_dot1XConfiguration = val;
}


inline void GetDot1XConfigurationsResponse::setDot1XConfiguration(std::vector<ONVIF::Dot1XConfiguration>&& val)
{
	_dot1XConfiguration = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetDot1XConfigurationsResponse_INCLUDED
