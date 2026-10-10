// 
// NetworkInterface.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkInterface_INCLUDED
#define ONVIF_NetworkInterface_INCLUDED


#include "ONVIF/DeviceEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IPv4NetworkInterface;
class IPv6NetworkInterface;
class NetworkInterfaceExtension;
class NetworkInterfaceInfo;
class NetworkInterfaceLink;
} 


namespace ONVIF {


//@ name=NetworkInterface
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkInterface: public DeviceEntity
{
public:
	NetworkInterface();

	NetworkInterface(
		const std::string& token, 
		bool enabled, 
		const Poco::SharedPtr<NetworkInterfaceInfo>& info, 
		const Poco::SharedPtr<NetworkInterfaceLink>& link, 
		const Poco::SharedPtr<IPv4NetworkInterface>& iPv4, 
		const Poco::SharedPtr<IPv6NetworkInterface>& iPv6, 
		const Poco::SharedPtr<NetworkInterfaceExtension>& extension);

	virtual ~NetworkInterface();

	bool getEnabled() const;

	const Poco::SharedPtr<NetworkInterfaceExtension>& getExtension() const;

	const Poco::SharedPtr<IPv4NetworkInterface>& getIPv4() const;

	const Poco::SharedPtr<IPv6NetworkInterface>& getIPv6() const;

	const Poco::SharedPtr<NetworkInterfaceInfo>& getInfo() const;

	const Poco::SharedPtr<NetworkInterfaceLink>& getLink() const;

	void setEnabled(bool val);

	void setExtension(const Poco::SharedPtr<NetworkInterfaceExtension>& val);

	void setExtension(Poco::SharedPtr<NetworkInterfaceExtension>&& val);

	void setIPv4(const Poco::SharedPtr<IPv4NetworkInterface>& val);

	void setIPv4(Poco::SharedPtr<IPv4NetworkInterface>&& val);

	void setIPv6(const Poco::SharedPtr<IPv6NetworkInterface>& val);

	void setIPv6(Poco::SharedPtr<IPv6NetworkInterface>&& val);

	void setInfo(const Poco::SharedPtr<NetworkInterfaceInfo>& val);

	void setInfo(Poco::SharedPtr<NetworkInterfaceInfo>&& val);

	void setLink(const Poco::SharedPtr<NetworkInterfaceLink>& val);

	void setLink(Poco::SharedPtr<NetworkInterfaceLink>&& val);

private:
	//@ name=Enabled
	//@ order=0
	bool _enabled;

	//@ mandatory=false
	//@ name=Info
	//@ order=1
	Poco::SharedPtr<NetworkInterfaceInfo> _info;

	//@ mandatory=false
	//@ name=Link
	//@ order=2
	Poco::SharedPtr<NetworkInterfaceLink> _link;

	//@ mandatory=false
	//@ name=IPv4
	//@ order=3
	Poco::SharedPtr<IPv4NetworkInterface> _iPv4;

	//@ mandatory=false
	//@ name=IPv6
	//@ order=4
	Poco::SharedPtr<IPv6NetworkInterface> _iPv6;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<NetworkInterfaceExtension> _extension;

};


inline bool NetworkInterface::getEnabled() const
{
	return _enabled;
}


inline const Poco::SharedPtr<NetworkInterfaceExtension>& NetworkInterface::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<IPv4NetworkInterface>& NetworkInterface::getIPv4() const
{
	return _iPv4;
}


inline const Poco::SharedPtr<IPv6NetworkInterface>& NetworkInterface::getIPv6() const
{
	return _iPv6;
}


inline const Poco::SharedPtr<NetworkInterfaceInfo>& NetworkInterface::getInfo() const
{
	return _info;
}


inline const Poco::SharedPtr<NetworkInterfaceLink>& NetworkInterface::getLink() const
{
	return _link;
}


inline void NetworkInterface::setEnabled(bool val)
{
	_enabled = val;
}


inline void NetworkInterface::setExtension(const Poco::SharedPtr<NetworkInterfaceExtension>& val)
{
	_extension = val;
}


inline void NetworkInterface::setExtension(Poco::SharedPtr<NetworkInterfaceExtension>&& val)
{
	_extension = std::move(val);
}


inline void NetworkInterface::setIPv4(const Poco::SharedPtr<IPv4NetworkInterface>& val)
{
	_iPv4 = val;
}


inline void NetworkInterface::setIPv4(Poco::SharedPtr<IPv4NetworkInterface>&& val)
{
	_iPv4 = std::move(val);
}


inline void NetworkInterface::setIPv6(const Poco::SharedPtr<IPv6NetworkInterface>& val)
{
	_iPv6 = val;
}


inline void NetworkInterface::setIPv6(Poco::SharedPtr<IPv6NetworkInterface>&& val)
{
	_iPv6 = std::move(val);
}


inline void NetworkInterface::setInfo(const Poco::SharedPtr<NetworkInterfaceInfo>& val)
{
	_info = val;
}


inline void NetworkInterface::setInfo(Poco::SharedPtr<NetworkInterfaceInfo>&& val)
{
	_info = std::move(val);
}


inline void NetworkInterface::setLink(const Poco::SharedPtr<NetworkInterfaceLink>& val)
{
	_link = val;
}


inline void NetworkInterface::setLink(Poco::SharedPtr<NetworkInterfaceLink>&& val)
{
	_link = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/IPv4NetworkInterface.h"
#include "ONVIF/IPv6NetworkInterface.h"
#include "ONVIF/NetworkInterfaceExtension.h"
#include "ONVIF/NetworkInterfaceInfo.h"
#include "ONVIF/NetworkInterfaceLink.h"


#endif // ONVIF_NetworkInterface_INCLUDED
