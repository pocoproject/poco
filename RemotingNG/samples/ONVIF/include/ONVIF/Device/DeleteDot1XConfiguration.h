// 
// DeleteDot1XConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_DeleteDot1XConfiguration_INCLUDED
#define ONVIF_Device_DeleteDot1XConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#DeleteDot1XConfiguration"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API DeleteDot1XConfiguration
{
public:
	DeleteDot1XConfiguration();

	DeleteDot1XConfiguration(const std::vector<std::string>& dot1XConfigurationToken);

	virtual ~DeleteDot1XConfiguration();

	const std::vector<std::string>& getDot1XConfigurationToken() const;

	std::vector<std::string>& getDot1XConfigurationToken();

	void setDot1XConfigurationToken(const std::vector<std::string>& val);

	void setDot1XConfigurationToken(std::vector<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Dot1XConfigurationToken
	//@ order=0
	std::vector<std::string> _dot1XConfigurationToken;

};


inline const std::vector<std::string>& DeleteDot1XConfiguration::getDot1XConfigurationToken() const
{
	return _dot1XConfigurationToken;
}


inline std::vector<std::string>& DeleteDot1XConfiguration::getDot1XConfigurationToken()
{
	return _dot1XConfigurationToken;
}


inline void DeleteDot1XConfiguration::setDot1XConfigurationToken(const std::vector<std::string>& val)
{
	_dot1XConfigurationToken = val;
}


inline void DeleteDot1XConfiguration::setDot1XConfigurationToken(std::vector<std::string>&& val)
{
	_dot1XConfigurationToken = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_DeleteDot1XConfiguration_INCLUDED
