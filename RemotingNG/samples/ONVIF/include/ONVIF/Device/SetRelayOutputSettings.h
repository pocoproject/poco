// 
// SetRelayOutputSettings.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetRelayOutputSettings_INCLUDED
#define ONVIF_Device_SetRelayOutputSettings_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RelayOutputSettings.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetRelayOutputSettings"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetRelayOutputSettings
{
public:
	SetRelayOutputSettings();

	SetRelayOutputSettings(
		const std::string& relayOutputToken, 
		const ONVIF::RelayOutputSettings& properties);

	virtual ~SetRelayOutputSettings();

	const ONVIF::RelayOutputSettings& getProperties() const;

	const std::string& getRelayOutputToken() const;

	void setProperties(const ONVIF::RelayOutputSettings& val);

	void setProperties(ONVIF::RelayOutputSettings&& val);

	void setRelayOutputToken(const std::string& val);

	void setRelayOutputToken(std::string&& val);

private:
	//@ name=RelayOutputToken
	//@ order=0
	std::string _relayOutputToken;

	//@ name=Properties
	//@ order=1
	ONVIF::RelayOutputSettings _properties;

};


inline const ONVIF::RelayOutputSettings& SetRelayOutputSettings::getProperties() const
{
	return _properties;
}


inline const std::string& SetRelayOutputSettings::getRelayOutputToken() const
{
	return _relayOutputToken;
}


inline void SetRelayOutputSettings::setProperties(const ONVIF::RelayOutputSettings& val)
{
	_properties = val;
}


inline void SetRelayOutputSettings::setProperties(ONVIF::RelayOutputSettings&& val)
{
	_properties = std::move(val);
}


inline void SetRelayOutputSettings::setRelayOutputToken(const std::string& val)
{
	_relayOutputToken = val;
}


inline void SetRelayOutputSettings::setRelayOutputToken(std::string&& val)
{
	_relayOutputToken = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetRelayOutputSettings_INCLUDED
