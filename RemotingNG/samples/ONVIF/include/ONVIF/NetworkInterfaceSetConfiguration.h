// 
// NetworkInterfaceSetConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkInterfaceSetConfiguration_INCLUDED
#define ONVIF_NetworkInterfaceSetConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IPv4NetworkInterfaceSetConfiguration;
class IPv6NetworkInterfaceSetConfiguration;
class NetworkInterfaceConnectionSetting;
class NetworkInterfaceSetConfigurationExtension;
} 


namespace ONVIF {


//@ name=NetworkInterfaceSetConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkInterfaceSetConfiguration
{
public:
	NetworkInterfaceSetConfiguration();

	NetworkInterfaceSetConfiguration(
		Poco::Optional<bool> enabled, 
		const Poco::SharedPtr<NetworkInterfaceConnectionSetting>& link, 
		Poco::Optional<int> mTU, 
		const Poco::SharedPtr<IPv4NetworkInterfaceSetConfiguration>& iPv4, 
		const Poco::SharedPtr<IPv6NetworkInterfaceSetConfiguration>& iPv6, 
		const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension>& extension);

	virtual ~NetworkInterfaceSetConfiguration();

	Poco::Optional<bool> getEnabled() const;

	const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension>& getExtension() const;

	const Poco::SharedPtr<IPv4NetworkInterfaceSetConfiguration>& getIPv4() const;

	const Poco::SharedPtr<IPv6NetworkInterfaceSetConfiguration>& getIPv6() const;

	const Poco::SharedPtr<NetworkInterfaceConnectionSetting>& getLink() const;

	Poco::Optional<int> getMTU() const;

	void setEnabled(Poco::Optional<bool> val);

	void setExtension(const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension>&& val);

	void setIPv4(const Poco::SharedPtr<IPv4NetworkInterfaceSetConfiguration>& val);

	void setIPv4(Poco::SharedPtr<IPv4NetworkInterfaceSetConfiguration>&& val);

	void setIPv6(const Poco::SharedPtr<IPv6NetworkInterfaceSetConfiguration>& val);

	void setIPv6(Poco::SharedPtr<IPv6NetworkInterfaceSetConfiguration>&& val);

	void setLink(const Poco::SharedPtr<NetworkInterfaceConnectionSetting>& val);

	void setLink(Poco::SharedPtr<NetworkInterfaceConnectionSetting>&& val);

	void setMTU(Poco::Optional<int> val);

private:
	//@ mandatory=false
	//@ name=Enabled
	//@ order=0
	Poco::Optional<bool> _enabled;

	//@ mandatory=false
	//@ name=Link
	//@ order=1
	Poco::SharedPtr<NetworkInterfaceConnectionSetting> _link;

	//@ mandatory=false
	//@ name=MTU
	//@ order=2
	Poco::Optional<int> _mTU;

	//@ mandatory=false
	//@ name=IPv4
	//@ order=3
	Poco::SharedPtr<IPv4NetworkInterfaceSetConfiguration> _iPv4;

	//@ mandatory=false
	//@ name=IPv6
	//@ order=4
	Poco::SharedPtr<IPv6NetworkInterfaceSetConfiguration> _iPv6;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension> _extension;

};


inline Poco::Optional<bool> NetworkInterfaceSetConfiguration::getEnabled() const
{
	return _enabled;
}


inline const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension>& NetworkInterfaceSetConfiguration::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<IPv4NetworkInterfaceSetConfiguration>& NetworkInterfaceSetConfiguration::getIPv4() const
{
	return _iPv4;
}


inline const Poco::SharedPtr<IPv6NetworkInterfaceSetConfiguration>& NetworkInterfaceSetConfiguration::getIPv6() const
{
	return _iPv6;
}


inline const Poco::SharedPtr<NetworkInterfaceConnectionSetting>& NetworkInterfaceSetConfiguration::getLink() const
{
	return _link;
}


inline Poco::Optional<int> NetworkInterfaceSetConfiguration::getMTU() const
{
	return _mTU;
}


inline void NetworkInterfaceSetConfiguration::setEnabled(Poco::Optional<bool> val)
{
	_enabled = val;
}


inline void NetworkInterfaceSetConfiguration::setExtension(const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension>& val)
{
	_extension = val;
}


inline void NetworkInterfaceSetConfiguration::setExtension(Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void NetworkInterfaceSetConfiguration::setIPv4(const Poco::SharedPtr<IPv4NetworkInterfaceSetConfiguration>& val)
{
	_iPv4 = val;
}


inline void NetworkInterfaceSetConfiguration::setIPv4(Poco::SharedPtr<IPv4NetworkInterfaceSetConfiguration>&& val)
{
	_iPv4 = std::move(val);
}


inline void NetworkInterfaceSetConfiguration::setIPv6(const Poco::SharedPtr<IPv6NetworkInterfaceSetConfiguration>& val)
{
	_iPv6 = val;
}


inline void NetworkInterfaceSetConfiguration::setIPv6(Poco::SharedPtr<IPv6NetworkInterfaceSetConfiguration>&& val)
{
	_iPv6 = std::move(val);
}


inline void NetworkInterfaceSetConfiguration::setLink(const Poco::SharedPtr<NetworkInterfaceConnectionSetting>& val)
{
	_link = val;
}


inline void NetworkInterfaceSetConfiguration::setLink(Poco::SharedPtr<NetworkInterfaceConnectionSetting>&& val)
{
	_link = std::move(val);
}


inline void NetworkInterfaceSetConfiguration::setMTU(Poco::Optional<int> val)
{
	_mTU = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/IPv4NetworkInterfaceSetConfiguration.h"
#include "ONVIF/IPv6NetworkInterfaceSetConfiguration.h"
#include "ONVIF/NetworkInterfaceConnectionSetting.h"
#include "ONVIF/NetworkInterfaceSetConfigurationExtension.h"


#endif // ONVIF_NetworkInterfaceSetConfiguration_INCLUDED
