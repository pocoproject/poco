// 
// IPAddress.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IPAddress_INCLUDED
#define ONVIF_IPAddress_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=IPAddress
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IPAddress
{
public:
	IPAddress();

	IPAddress(
		const std::string& type, 
		const Poco::Optional<std::string>& iPv4Address, 
		const Poco::Optional<std::string>& iPv6Address);

	virtual ~IPAddress();

	const Poco::Optional<std::string>& getIPv4Address() const;

	const Poco::Optional<std::string>& getIPv6Address() const;

	const std::string& getType() const;

	void setIPv4Address(const Poco::Optional<std::string>& val);

	void setIPv4Address(Poco::Optional<std::string>&& val);

	void setIPv6Address(const Poco::Optional<std::string>& val);

	void setIPv6Address(Poco::Optional<std::string>&& val);

	void setType(const std::string& val);

	void setType(std::string&& val);

private:
	//@ name=Type
	//@ order=0
	std::string _type;

	//@ mandatory=false
	//@ name=IPv4Address
	//@ order=1
	Poco::Optional<std::string> _iPv4Address;

	//@ mandatory=false
	//@ name=IPv6Address
	//@ order=2
	Poco::Optional<std::string> _iPv6Address;

};


inline const Poco::Optional<std::string>& IPAddress::getIPv4Address() const
{
	return _iPv4Address;
}


inline const Poco::Optional<std::string>& IPAddress::getIPv6Address() const
{
	return _iPv6Address;
}


inline const std::string& IPAddress::getType() const
{
	return _type;
}


inline void IPAddress::setIPv4Address(const Poco::Optional<std::string>& val)
{
	_iPv4Address = val;
}


inline void IPAddress::setIPv4Address(Poco::Optional<std::string>&& val)
{
	_iPv4Address = std::move(val);
}


inline void IPAddress::setIPv6Address(const Poco::Optional<std::string>& val)
{
	_iPv6Address = val;
}


inline void IPAddress::setIPv6Address(Poco::Optional<std::string>&& val)
{
	_iPv6Address = std::move(val);
}


inline void IPAddress::setType(const std::string& val)
{
	_type = val;
}


inline void IPAddress::setType(std::string&& val)
{
	_type = std::move(val);
}


} // ONVIF


#endif // ONVIF_IPAddress_INCLUDED
