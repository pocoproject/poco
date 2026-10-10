// 
// NetworkInterfaceLink.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkInterfaceLink_INCLUDED
#define ONVIF_NetworkInterfaceLink_INCLUDED


#include "ONVIF/NetworkInterfaceConnectionSetting.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=NetworkInterfaceLink
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkInterfaceLink
{
public:
	NetworkInterfaceLink();

	NetworkInterfaceLink(
		const NetworkInterfaceConnectionSetting& adminSettings, 
		const NetworkInterfaceConnectionSetting& operSettings, 
		const int& interfaceType);

	virtual ~NetworkInterfaceLink();

	const NetworkInterfaceConnectionSetting& getAdminSettings() const;

	const int& getInterfaceType() const;

	const NetworkInterfaceConnectionSetting& getOperSettings() const;

	void setAdminSettings(const NetworkInterfaceConnectionSetting& val);

	void setAdminSettings(NetworkInterfaceConnectionSetting&& val);

	void setInterfaceType(const int& val);

	void setInterfaceType(int&& val);

	void setOperSettings(const NetworkInterfaceConnectionSetting& val);

	void setOperSettings(NetworkInterfaceConnectionSetting&& val);

private:
	//@ name=AdminSettings
	//@ order=0
	NetworkInterfaceConnectionSetting _adminSettings;

	//@ name=OperSettings
	//@ order=1
	NetworkInterfaceConnectionSetting _operSettings;

	//@ name=InterfaceType
	//@ order=2
	int _interfaceType;

};


inline const NetworkInterfaceConnectionSetting& NetworkInterfaceLink::getAdminSettings() const
{
	return _adminSettings;
}


inline const int& NetworkInterfaceLink::getInterfaceType() const
{
	return _interfaceType;
}


inline const NetworkInterfaceConnectionSetting& NetworkInterfaceLink::getOperSettings() const
{
	return _operSettings;
}


inline void NetworkInterfaceLink::setAdminSettings(const NetworkInterfaceConnectionSetting& val)
{
	_adminSettings = val;
}


inline void NetworkInterfaceLink::setAdminSettings(NetworkInterfaceConnectionSetting&& val)
{
	_adminSettings = std::move(val);
}


inline void NetworkInterfaceLink::setInterfaceType(const int& val)
{
	_interfaceType = val;
}


inline void NetworkInterfaceLink::setInterfaceType(int&& val)
{
	_interfaceType = std::move(val);
}


inline void NetworkInterfaceLink::setOperSettings(const NetworkInterfaceConnectionSetting& val)
{
	_operSettings = val;
}


inline void NetworkInterfaceLink::setOperSettings(NetworkInterfaceConnectionSetting&& val)
{
	_operSettings = std::move(val);
}


} // ONVIF


#endif // ONVIF_NetworkInterfaceLink_INCLUDED
