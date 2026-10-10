// 
// IPv6NetworkInterfaceSetConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IPv6NetworkInterfaceSetConfiguration_INCLUDED
#define ONVIF_IPv6NetworkInterfaceSetConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PrefixedIPv6Address.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=IPv6NetworkInterfaceSetConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IPv6NetworkInterfaceSetConfiguration
{
public:
	IPv6NetworkInterfaceSetConfiguration();

	IPv6NetworkInterfaceSetConfiguration(
		Poco::Optional<bool> enabled, 
		Poco::Optional<bool> acceptRouterAdvert, 
		const std::vector<PrefixedIPv6Address>& manual, 
		const Poco::Optional<std::string>& dHCP);

	virtual ~IPv6NetworkInterfaceSetConfiguration();

	Poco::Optional<bool> getAcceptRouterAdvert() const;

	const Poco::Optional<std::string>& getDHCP() const;

	Poco::Optional<bool> getEnabled() const;

	const std::vector<PrefixedIPv6Address>& getManual() const;

	std::vector<PrefixedIPv6Address>& getManual();

	void setAcceptRouterAdvert(Poco::Optional<bool> val);

	void setDHCP(const Poco::Optional<std::string>& val);

	void setDHCP(Poco::Optional<std::string>&& val);

	void setEnabled(Poco::Optional<bool> val);

	void setManual(const std::vector<PrefixedIPv6Address>& val);

	void setManual(std::vector<PrefixedIPv6Address>&& val);

private:
	//@ mandatory=false
	//@ name=Enabled
	//@ order=0
	Poco::Optional<bool> _enabled;

	//@ mandatory=false
	//@ name=AcceptRouterAdvert
	//@ order=1
	Poco::Optional<bool> _acceptRouterAdvert;

	//@ mandatory=false
	//@ name=Manual
	//@ order=2
	std::vector<PrefixedIPv6Address> _manual;

	//@ mandatory=false
	//@ name=DHCP
	//@ order=3
	Poco::Optional<std::string> _dHCP;

};


inline Poco::Optional<bool> IPv6NetworkInterfaceSetConfiguration::getAcceptRouterAdvert() const
{
	return _acceptRouterAdvert;
}


inline const Poco::Optional<std::string>& IPv6NetworkInterfaceSetConfiguration::getDHCP() const
{
	return _dHCP;
}


inline Poco::Optional<bool> IPv6NetworkInterfaceSetConfiguration::getEnabled() const
{
	return _enabled;
}


inline const std::vector<PrefixedIPv6Address>& IPv6NetworkInterfaceSetConfiguration::getManual() const
{
	return _manual;
}


inline std::vector<PrefixedIPv6Address>& IPv6NetworkInterfaceSetConfiguration::getManual()
{
	return _manual;
}


inline void IPv6NetworkInterfaceSetConfiguration::setAcceptRouterAdvert(Poco::Optional<bool> val)
{
	_acceptRouterAdvert = val;
}


inline void IPv6NetworkInterfaceSetConfiguration::setDHCP(const Poco::Optional<std::string>& val)
{
	_dHCP = val;
}


inline void IPv6NetworkInterfaceSetConfiguration::setDHCP(Poco::Optional<std::string>&& val)
{
	_dHCP = std::move(val);
}


inline void IPv6NetworkInterfaceSetConfiguration::setEnabled(Poco::Optional<bool> val)
{
	_enabled = val;
}


inline void IPv6NetworkInterfaceSetConfiguration::setManual(const std::vector<PrefixedIPv6Address>& val)
{
	_manual = val;
}


inline void IPv6NetworkInterfaceSetConfiguration::setManual(std::vector<PrefixedIPv6Address>&& val)
{
	_manual = std::move(val);
}


} // ONVIF


#endif // ONVIF_IPv6NetworkInterfaceSetConfiguration_INCLUDED
