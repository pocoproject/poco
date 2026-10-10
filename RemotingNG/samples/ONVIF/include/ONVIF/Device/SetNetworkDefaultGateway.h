// 
// SetNetworkDefaultGateway.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetNetworkDefaultGateway_INCLUDED
#define ONVIF_Device_SetNetworkDefaultGateway_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetNetworkDefaultGateway"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetNetworkDefaultGateway
{
public:
	SetNetworkDefaultGateway();

	SetNetworkDefaultGateway(
		const std::vector<std::string>& iPv4Address, 
		const std::vector<std::string>& iPv6Address);

	virtual ~SetNetworkDefaultGateway();

	const std::vector<std::string>& getIPv4Address() const;

	std::vector<std::string>& getIPv4Address();

	const std::vector<std::string>& getIPv6Address() const;

	std::vector<std::string>& getIPv6Address();

	void setIPv4Address(const std::vector<std::string>& val);

	void setIPv4Address(std::vector<std::string>&& val);

	void setIPv6Address(const std::vector<std::string>& val);

	void setIPv6Address(std::vector<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=IPv4Address
	//@ order=0
	std::vector<std::string> _iPv4Address;

	//@ mandatory=false
	//@ name=IPv6Address
	//@ order=1
	std::vector<std::string> _iPv6Address;

};


inline const std::vector<std::string>& SetNetworkDefaultGateway::getIPv4Address() const
{
	return _iPv4Address;
}


inline std::vector<std::string>& SetNetworkDefaultGateway::getIPv4Address()
{
	return _iPv4Address;
}


inline const std::vector<std::string>& SetNetworkDefaultGateway::getIPv6Address() const
{
	return _iPv6Address;
}


inline std::vector<std::string>& SetNetworkDefaultGateway::getIPv6Address()
{
	return _iPv6Address;
}


inline void SetNetworkDefaultGateway::setIPv4Address(const std::vector<std::string>& val)
{
	_iPv4Address = val;
}


inline void SetNetworkDefaultGateway::setIPv4Address(std::vector<std::string>&& val)
{
	_iPv4Address = std::move(val);
}


inline void SetNetworkDefaultGateway::setIPv6Address(const std::vector<std::string>& val)
{
	_iPv6Address = val;
}


inline void SetNetworkDefaultGateway::setIPv6Address(std::vector<std::string>&& val)
{
	_iPv6Address = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetNetworkDefaultGateway_INCLUDED
