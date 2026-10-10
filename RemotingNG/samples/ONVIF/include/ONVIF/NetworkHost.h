// 
// NetworkHost.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkHost_INCLUDED
#define ONVIF_NetworkHost_INCLUDED


#include "ONVIF/NetworkHost.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class NetworkHostExtension;
} 


namespace ONVIF {


//@ name=NetworkHost
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkHost
{
public:
	NetworkHost();

	NetworkHost(
		const Poco::SharedPtr<NetworkHost>& type, 
		const Poco::Optional<std::string>& iPv4Address, 
		const Poco::Optional<std::string>& iPv6Address, 
		const Poco::Optional<std::string>& dNSname, 
		const Poco::SharedPtr<NetworkHostExtension>& extension);

	virtual ~NetworkHost();

	const Poco::Optional<std::string>& getDNSname() const;

	const Poco::SharedPtr<NetworkHostExtension>& getExtension() const;

	const Poco::Optional<std::string>& getIPv4Address() const;

	const Poco::Optional<std::string>& getIPv6Address() const;

	const Poco::SharedPtr<NetworkHost>& getType() const;

	void setDNSname(const Poco::Optional<std::string>& val);

	void setDNSname(Poco::Optional<std::string>&& val);

	void setExtension(const Poco::SharedPtr<NetworkHostExtension>& val);

	void setExtension(Poco::SharedPtr<NetworkHostExtension>&& val);

	void setIPv4Address(const Poco::Optional<std::string>& val);

	void setIPv4Address(Poco::Optional<std::string>&& val);

	void setIPv6Address(const Poco::Optional<std::string>& val);

	void setIPv6Address(Poco::Optional<std::string>&& val);

	void setType(const Poco::SharedPtr<NetworkHost>& val);

	void setType(Poco::SharedPtr<NetworkHost>&& val);

private:
	//@ name=Type
	//@ order=0
	Poco::SharedPtr<NetworkHost> _type;

	//@ mandatory=false
	//@ name=IPv4Address
	//@ order=1
	Poco::Optional<std::string> _iPv4Address;

	//@ mandatory=false
	//@ name=IPv6Address
	//@ order=2
	Poco::Optional<std::string> _iPv6Address;

	//@ mandatory=false
	//@ name=DNSname
	//@ order=3
	Poco::Optional<std::string> _dNSname;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<NetworkHostExtension> _extension;

};


inline const Poco::Optional<std::string>& NetworkHost::getDNSname() const
{
	return _dNSname;
}


inline const Poco::SharedPtr<NetworkHostExtension>& NetworkHost::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& NetworkHost::getIPv4Address() const
{
	return _iPv4Address;
}


inline const Poco::Optional<std::string>& NetworkHost::getIPv6Address() const
{
	return _iPv6Address;
}


inline const Poco::SharedPtr<NetworkHost>& NetworkHost::getType() const
{
	return _type;
}


inline void NetworkHost::setDNSname(const Poco::Optional<std::string>& val)
{
	_dNSname = val;
}


inline void NetworkHost::setDNSname(Poco::Optional<std::string>&& val)
{
	_dNSname = std::move(val);
}


inline void NetworkHost::setExtension(const Poco::SharedPtr<NetworkHostExtension>& val)
{
	_extension = val;
}


inline void NetworkHost::setExtension(Poco::SharedPtr<NetworkHostExtension>&& val)
{
	_extension = std::move(val);
}


inline void NetworkHost::setIPv4Address(const Poco::Optional<std::string>& val)
{
	_iPv4Address = val;
}


inline void NetworkHost::setIPv4Address(Poco::Optional<std::string>&& val)
{
	_iPv4Address = std::move(val);
}


inline void NetworkHost::setIPv6Address(const Poco::Optional<std::string>& val)
{
	_iPv6Address = val;
}


inline void NetworkHost::setIPv6Address(Poco::Optional<std::string>&& val)
{
	_iPv6Address = std::move(val);
}


inline void NetworkHost::setType(const Poco::SharedPtr<NetworkHost>& val)
{
	_type = val;
}


inline void NetworkHost::setType(Poco::SharedPtr<NetworkHost>&& val)
{
	_type = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/NetworkHostExtension.h"


#endif // ONVIF_NetworkHost_INCLUDED
