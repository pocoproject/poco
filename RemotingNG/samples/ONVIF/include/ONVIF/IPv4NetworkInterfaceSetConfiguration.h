// 
// IPv4NetworkInterfaceSetConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IPv4NetworkInterfaceSetConfiguration_INCLUDED
#define ONVIF_IPv4NetworkInterfaceSetConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PrefixedIPv4Address.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=IPv4NetworkInterfaceSetConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IPv4NetworkInterfaceSetConfiguration
{
public:
	IPv4NetworkInterfaceSetConfiguration();

	IPv4NetworkInterfaceSetConfiguration(
		Poco::Optional<bool> enabled, 
		const std::vector<PrefixedIPv4Address>& manual, 
		Poco::Optional<bool> dHCP);

	virtual ~IPv4NetworkInterfaceSetConfiguration();

	Poco::Optional<bool> getDHCP() const;

	Poco::Optional<bool> getEnabled() const;

	const std::vector<PrefixedIPv4Address>& getManual() const;

	std::vector<PrefixedIPv4Address>& getManual();

	void setDHCP(Poco::Optional<bool> val);

	void setEnabled(Poco::Optional<bool> val);

	void setManual(const std::vector<PrefixedIPv4Address>& val);

	void setManual(std::vector<PrefixedIPv4Address>&& val);

private:
	//@ mandatory=false
	//@ name=Enabled
	//@ order=0
	Poco::Optional<bool> _enabled;

	//@ mandatory=false
	//@ name=Manual
	//@ order=1
	std::vector<PrefixedIPv4Address> _manual;

	//@ mandatory=false
	//@ name=DHCP
	//@ order=2
	Poco::Optional<bool> _dHCP;

};


inline Poco::Optional<bool> IPv4NetworkInterfaceSetConfiguration::getDHCP() const
{
	return _dHCP;
}


inline Poco::Optional<bool> IPv4NetworkInterfaceSetConfiguration::getEnabled() const
{
	return _enabled;
}


inline const std::vector<PrefixedIPv4Address>& IPv4NetworkInterfaceSetConfiguration::getManual() const
{
	return _manual;
}


inline std::vector<PrefixedIPv4Address>& IPv4NetworkInterfaceSetConfiguration::getManual()
{
	return _manual;
}


inline void IPv4NetworkInterfaceSetConfiguration::setDHCP(Poco::Optional<bool> val)
{
	_dHCP = val;
}


inline void IPv4NetworkInterfaceSetConfiguration::setEnabled(Poco::Optional<bool> val)
{
	_enabled = val;
}


inline void IPv4NetworkInterfaceSetConfiguration::setManual(const std::vector<PrefixedIPv4Address>& val)
{
	_manual = val;
}


inline void IPv4NetworkInterfaceSetConfiguration::setManual(std::vector<PrefixedIPv4Address>&& val)
{
	_manual = std::move(val);
}


} // ONVIF


#endif // ONVIF_IPv4NetworkInterfaceSetConfiguration_INCLUDED
