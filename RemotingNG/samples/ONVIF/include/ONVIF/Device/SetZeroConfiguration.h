// 
// SetZeroConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetZeroConfiguration_INCLUDED
#define ONVIF_Device_SetZeroConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetZeroConfiguration"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetZeroConfiguration
{
public:
	SetZeroConfiguration();

	SetZeroConfiguration(
		const std::string& interfaceToken, 
		bool enabled);

	virtual ~SetZeroConfiguration();

	bool getEnabled() const;

	const std::string& getInterfaceToken() const;

	void setEnabled(bool val);

	void setInterfaceToken(const std::string& val);

	void setInterfaceToken(std::string&& val);

private:
	//@ name=InterfaceToken
	//@ order=0
	std::string _interfaceToken;

	//@ name=Enabled
	//@ order=1
	bool _enabled;

};


inline bool SetZeroConfiguration::getEnabled() const
{
	return _enabled;
}


inline const std::string& SetZeroConfiguration::getInterfaceToken() const
{
	return _interfaceToken;
}


inline void SetZeroConfiguration::setEnabled(bool val)
{
	_enabled = val;
}


inline void SetZeroConfiguration::setInterfaceToken(const std::string& val)
{
	_interfaceToken = val;
}


inline void SetZeroConfiguration::setInterfaceToken(std::string&& val)
{
	_interfaceToken = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetZeroConfiguration_INCLUDED
