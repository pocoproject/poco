// 
// GetDot1XConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetDot1XConfiguration_INCLUDED
#define ONVIF_Device_GetDot1XConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetDot1XConfiguration"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetDot1XConfiguration
{
public:
	GetDot1XConfiguration();

	GetDot1XConfiguration(const std::string& dot1XConfigurationToken);

	virtual ~GetDot1XConfiguration();

	const std::string& getDot1XConfigurationToken() const;

	void setDot1XConfigurationToken(const std::string& val);

	void setDot1XConfigurationToken(std::string&& val);

private:
	//@ name=Dot1XConfigurationToken
	//@ order=0
	std::string _dot1XConfigurationToken;

};


inline const std::string& GetDot1XConfiguration::getDot1XConfigurationToken() const
{
	return _dot1XConfigurationToken;
}


inline void GetDot1XConfiguration::setDot1XConfigurationToken(const std::string& val)
{
	_dot1XConfigurationToken = val;
}


inline void GetDot1XConfiguration::setDot1XConfigurationToken(std::string&& val)
{
	_dot1XConfigurationToken = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetDot1XConfiguration_INCLUDED
