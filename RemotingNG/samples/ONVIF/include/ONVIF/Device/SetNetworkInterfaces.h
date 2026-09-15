// 
// SetNetworkInterfaces.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetNetworkInterfaces_INCLUDED
#define ONVIF_Device_SetNetworkInterfaces_INCLUDED


#include "ONVIF/NetworkInterfaceSetConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetNetworkInterfaces"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetNetworkInterfaces
{
public:
	SetNetworkInterfaces();

	SetNetworkInterfaces(
		const std::string& interfaceToken, 
		const ONVIF::NetworkInterfaceSetConfiguration& networkInterface);

	virtual ~SetNetworkInterfaces();

	const std::string& getInterfaceToken() const;

	const ONVIF::NetworkInterfaceSetConfiguration& getNetworkInterface() const;

	void setInterfaceToken(const std::string& val);

	void setInterfaceToken(std::string&& val);

	void setNetworkInterface(const ONVIF::NetworkInterfaceSetConfiguration& val);

	void setNetworkInterface(ONVIF::NetworkInterfaceSetConfiguration&& val);

private:
	//@ name=InterfaceToken
	//@ order=0
	std::string _interfaceToken;

	//@ name=NetworkInterface
	//@ order=1
	ONVIF::NetworkInterfaceSetConfiguration _networkInterface;

};


inline const std::string& SetNetworkInterfaces::getInterfaceToken() const
{
	return _interfaceToken;
}


inline const ONVIF::NetworkInterfaceSetConfiguration& SetNetworkInterfaces::getNetworkInterface() const
{
	return _networkInterface;
}


inline void SetNetworkInterfaces::setInterfaceToken(const std::string& val)
{
	_interfaceToken = val;
}


inline void SetNetworkInterfaces::setInterfaceToken(std::string&& val)
{
	_interfaceToken = std::move(val);
}


inline void SetNetworkInterfaces::setNetworkInterface(const ONVIF::NetworkInterfaceSetConfiguration& val)
{
	_networkInterface = val;
}


inline void SetNetworkInterfaces::setNetworkInterface(ONVIF::NetworkInterfaceSetConfiguration&& val)
{
	_networkInterface = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetNetworkInterfaces_INCLUDED
