// 
// IPAddressFilter.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IPAddressFilter_INCLUDED
#define ONVIF_IPAddressFilter_INCLUDED


#include "ONVIF/IPAddressFilter.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/PrefixedIPv4Address.h"
#include "ONVIF/PrefixedIPv6Address.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IPAddressFilterExtension;
} 


namespace ONVIF {


//@ name=IPAddressFilter
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IPAddressFilter
{
public:
	IPAddressFilter();

	IPAddressFilter(
		const Poco::SharedPtr<IPAddressFilter>& type, 
		const std::vector<PrefixedIPv4Address>& iPv4Address, 
		const std::vector<PrefixedIPv6Address>& iPv6Address, 
		const Poco::SharedPtr<IPAddressFilterExtension>& extension);

	virtual ~IPAddressFilter();

	const Poco::SharedPtr<IPAddressFilterExtension>& getExtension() const;

	const std::vector<PrefixedIPv4Address>& getIPv4Address() const;

	std::vector<PrefixedIPv4Address>& getIPv4Address();

	const std::vector<PrefixedIPv6Address>& getIPv6Address() const;

	std::vector<PrefixedIPv6Address>& getIPv6Address();

	const Poco::SharedPtr<IPAddressFilter>& getType() const;

	void setExtension(const Poco::SharedPtr<IPAddressFilterExtension>& val);

	void setExtension(Poco::SharedPtr<IPAddressFilterExtension>&& val);

	void setIPv4Address(const std::vector<PrefixedIPv4Address>& val);

	void setIPv4Address(std::vector<PrefixedIPv4Address>&& val);

	void setIPv6Address(const std::vector<PrefixedIPv6Address>& val);

	void setIPv6Address(std::vector<PrefixedIPv6Address>&& val);

	void setType(const Poco::SharedPtr<IPAddressFilter>& val);

	void setType(Poco::SharedPtr<IPAddressFilter>&& val);

private:
	//@ name=Type
	//@ order=0
	Poco::SharedPtr<IPAddressFilter> _type;

	//@ mandatory=false
	//@ name=IPv4Address
	//@ order=1
	std::vector<PrefixedIPv4Address> _iPv4Address;

	//@ mandatory=false
	//@ name=IPv6Address
	//@ order=2
	std::vector<PrefixedIPv6Address> _iPv6Address;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<IPAddressFilterExtension> _extension;

};


inline const Poco::SharedPtr<IPAddressFilterExtension>& IPAddressFilter::getExtension() const
{
	return _extension;
}


inline const std::vector<PrefixedIPv4Address>& IPAddressFilter::getIPv4Address() const
{
	return _iPv4Address;
}


inline std::vector<PrefixedIPv4Address>& IPAddressFilter::getIPv4Address()
{
	return _iPv4Address;
}


inline const std::vector<PrefixedIPv6Address>& IPAddressFilter::getIPv6Address() const
{
	return _iPv6Address;
}


inline std::vector<PrefixedIPv6Address>& IPAddressFilter::getIPv6Address()
{
	return _iPv6Address;
}


inline const Poco::SharedPtr<IPAddressFilter>& IPAddressFilter::getType() const
{
	return _type;
}


inline void IPAddressFilter::setExtension(const Poco::SharedPtr<IPAddressFilterExtension>& val)
{
	_extension = val;
}


inline void IPAddressFilter::setExtension(Poco::SharedPtr<IPAddressFilterExtension>&& val)
{
	_extension = std::move(val);
}


inline void IPAddressFilter::setIPv4Address(const std::vector<PrefixedIPv4Address>& val)
{
	_iPv4Address = val;
}


inline void IPAddressFilter::setIPv4Address(std::vector<PrefixedIPv4Address>&& val)
{
	_iPv4Address = std::move(val);
}


inline void IPAddressFilter::setIPv6Address(const std::vector<PrefixedIPv6Address>& val)
{
	_iPv6Address = val;
}


inline void IPAddressFilter::setIPv6Address(std::vector<PrefixedIPv6Address>&& val)
{
	_iPv6Address = std::move(val);
}


inline void IPAddressFilter::setType(const Poco::SharedPtr<IPAddressFilter>& val)
{
	_type = val;
}


inline void IPAddressFilter::setType(Poco::SharedPtr<IPAddressFilter>&& val)
{
	_type = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/IPAddressFilterExtension.h"


#endif // ONVIF_IPAddressFilter_INCLUDED
