// 
// IPv6Configuration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IPv6Configuration_INCLUDED
#define ONVIF_IPv6Configuration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PrefixedIPv6Address.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IPv6ConfigurationExtension;
} 


namespace ONVIF {


//@ name=IPv6Configuration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IPv6Configuration
{
public:
	IPv6Configuration();

	IPv6Configuration(
		Poco::Optional<bool> acceptRouterAdvert, 
		const std::string& dHCP, 
		const std::vector<PrefixedIPv6Address>& manual, 
		const std::vector<PrefixedIPv6Address>& linkLocal, 
		const std::vector<PrefixedIPv6Address>& fromDHCP, 
		const std::vector<PrefixedIPv6Address>& fromRA, 
		const Poco::SharedPtr<IPv6ConfigurationExtension>& extension);

	virtual ~IPv6Configuration();

	Poco::Optional<bool> getAcceptRouterAdvert() const;

	const std::string& getDHCP() const;

	const Poco::SharedPtr<IPv6ConfigurationExtension>& getExtension() const;

	const std::vector<PrefixedIPv6Address>& getFromDHCP() const;

	std::vector<PrefixedIPv6Address>& getFromDHCP();

	const std::vector<PrefixedIPv6Address>& getFromRA() const;

	std::vector<PrefixedIPv6Address>& getFromRA();

	const std::vector<PrefixedIPv6Address>& getLinkLocal() const;

	std::vector<PrefixedIPv6Address>& getLinkLocal();

	const std::vector<PrefixedIPv6Address>& getManual() const;

	std::vector<PrefixedIPv6Address>& getManual();

	void setAcceptRouterAdvert(Poco::Optional<bool> val);

	void setDHCP(const std::string& val);

	void setDHCP(std::string&& val);

	void setExtension(const Poco::SharedPtr<IPv6ConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<IPv6ConfigurationExtension>&& val);

	void setFromDHCP(const std::vector<PrefixedIPv6Address>& val);

	void setFromDHCP(std::vector<PrefixedIPv6Address>&& val);

	void setFromRA(const std::vector<PrefixedIPv6Address>& val);

	void setFromRA(std::vector<PrefixedIPv6Address>&& val);

	void setLinkLocal(const std::vector<PrefixedIPv6Address>& val);

	void setLinkLocal(std::vector<PrefixedIPv6Address>&& val);

	void setManual(const std::vector<PrefixedIPv6Address>& val);

	void setManual(std::vector<PrefixedIPv6Address>&& val);

private:
	//@ mandatory=false
	//@ name=AcceptRouterAdvert
	//@ order=0
	Poco::Optional<bool> _acceptRouterAdvert;

	//@ name=DHCP
	//@ order=1
	std::string _dHCP;

	//@ mandatory=false
	//@ name=Manual
	//@ order=2
	std::vector<PrefixedIPv6Address> _manual;

	//@ mandatory=false
	//@ name=LinkLocal
	//@ order=3
	std::vector<PrefixedIPv6Address> _linkLocal;

	//@ mandatory=false
	//@ name=FromDHCP
	//@ order=4
	std::vector<PrefixedIPv6Address> _fromDHCP;

	//@ mandatory=false
	//@ name=FromRA
	//@ order=5
	std::vector<PrefixedIPv6Address> _fromRA;

	//@ mandatory=false
	//@ name=Extension
	//@ order=6
	Poco::SharedPtr<IPv6ConfigurationExtension> _extension;

};


inline Poco::Optional<bool> IPv6Configuration::getAcceptRouterAdvert() const
{
	return _acceptRouterAdvert;
}


inline const std::string& IPv6Configuration::getDHCP() const
{
	return _dHCP;
}


inline const Poco::SharedPtr<IPv6ConfigurationExtension>& IPv6Configuration::getExtension() const
{
	return _extension;
}


inline const std::vector<PrefixedIPv6Address>& IPv6Configuration::getFromDHCP() const
{
	return _fromDHCP;
}


inline std::vector<PrefixedIPv6Address>& IPv6Configuration::getFromDHCP()
{
	return _fromDHCP;
}


inline const std::vector<PrefixedIPv6Address>& IPv6Configuration::getFromRA() const
{
	return _fromRA;
}


inline std::vector<PrefixedIPv6Address>& IPv6Configuration::getFromRA()
{
	return _fromRA;
}


inline const std::vector<PrefixedIPv6Address>& IPv6Configuration::getLinkLocal() const
{
	return _linkLocal;
}


inline std::vector<PrefixedIPv6Address>& IPv6Configuration::getLinkLocal()
{
	return _linkLocal;
}


inline const std::vector<PrefixedIPv6Address>& IPv6Configuration::getManual() const
{
	return _manual;
}


inline std::vector<PrefixedIPv6Address>& IPv6Configuration::getManual()
{
	return _manual;
}


inline void IPv6Configuration::setAcceptRouterAdvert(Poco::Optional<bool> val)
{
	_acceptRouterAdvert = val;
}


inline void IPv6Configuration::setDHCP(const std::string& val)
{
	_dHCP = val;
}


inline void IPv6Configuration::setDHCP(std::string&& val)
{
	_dHCP = std::move(val);
}


inline void IPv6Configuration::setExtension(const Poco::SharedPtr<IPv6ConfigurationExtension>& val)
{
	_extension = val;
}


inline void IPv6Configuration::setExtension(Poco::SharedPtr<IPv6ConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void IPv6Configuration::setFromDHCP(const std::vector<PrefixedIPv6Address>& val)
{
	_fromDHCP = val;
}


inline void IPv6Configuration::setFromDHCP(std::vector<PrefixedIPv6Address>&& val)
{
	_fromDHCP = std::move(val);
}


inline void IPv6Configuration::setFromRA(const std::vector<PrefixedIPv6Address>& val)
{
	_fromRA = val;
}


inline void IPv6Configuration::setFromRA(std::vector<PrefixedIPv6Address>&& val)
{
	_fromRA = std::move(val);
}


inline void IPv6Configuration::setLinkLocal(const std::vector<PrefixedIPv6Address>& val)
{
	_linkLocal = val;
}


inline void IPv6Configuration::setLinkLocal(std::vector<PrefixedIPv6Address>&& val)
{
	_linkLocal = std::move(val);
}


inline void IPv6Configuration::setManual(const std::vector<PrefixedIPv6Address>& val)
{
	_manual = val;
}


inline void IPv6Configuration::setManual(std::vector<PrefixedIPv6Address>&& val)
{
	_manual = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/IPv6ConfigurationExtension.h"


#endif // ONVIF_IPv6Configuration_INCLUDED
