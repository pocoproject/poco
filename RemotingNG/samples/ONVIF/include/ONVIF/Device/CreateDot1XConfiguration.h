// 
// CreateDot1XConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_CreateDot1XConfiguration_INCLUDED
#define ONVIF_Device_CreateDot1XConfiguration_INCLUDED


#include "ONVIF/Dot1XConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#CreateDot1XConfiguration"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API CreateDot1XConfiguration
{
public:
	CreateDot1XConfiguration();

	CreateDot1XConfiguration(const ONVIF::Dot1XConfiguration& dot1XConfiguration);

	virtual ~CreateDot1XConfiguration();

	const ONVIF::Dot1XConfiguration& getDot1XConfiguration() const;

	void setDot1XConfiguration(const ONVIF::Dot1XConfiguration& val);

	void setDot1XConfiguration(ONVIF::Dot1XConfiguration&& val);

private:
	//@ name=Dot1XConfiguration
	//@ order=0
	ONVIF::Dot1XConfiguration _dot1XConfiguration;

};


inline const ONVIF::Dot1XConfiguration& CreateDot1XConfiguration::getDot1XConfiguration() const
{
	return _dot1XConfiguration;
}


inline void CreateDot1XConfiguration::setDot1XConfiguration(const ONVIF::Dot1XConfiguration& val)
{
	_dot1XConfiguration = val;
}


inline void CreateDot1XConfiguration::setDot1XConfiguration(ONVIF::Dot1XConfiguration&& val)
{
	_dot1XConfiguration = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_CreateDot1XConfiguration_INCLUDED
