// 
// IPv4Configuration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IPv4Configuration_INCLUDED
#define ONVIF_IPv4Configuration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PrefixedIPv4Address.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PrefixedIPv4Address;
} 


namespace ONVIF {


//@ name=IPv4Configuration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IPv4Configuration
{
public:
	IPv4Configuration();

	IPv4Configuration(
		const std::vector<PrefixedIPv4Address>& manual, 
		const Poco::SharedPtr<PrefixedIPv4Address>& linkLocal, 
		const Poco::SharedPtr<PrefixedIPv4Address>& fromDHCP, 
		bool dHCP);

	virtual ~IPv4Configuration();

	bool getDHCP() const;

	const Poco::SharedPtr<PrefixedIPv4Address>& getFromDHCP() const;

	const Poco::SharedPtr<PrefixedIPv4Address>& getLinkLocal() const;

	const std::vector<PrefixedIPv4Address>& getManual() const;

	std::vector<PrefixedIPv4Address>& getManual();

	void setDHCP(bool val);

	void setFromDHCP(const Poco::SharedPtr<PrefixedIPv4Address>& val);

	void setFromDHCP(Poco::SharedPtr<PrefixedIPv4Address>&& val);

	void setLinkLocal(const Poco::SharedPtr<PrefixedIPv4Address>& val);

	void setLinkLocal(Poco::SharedPtr<PrefixedIPv4Address>&& val);

	void setManual(const std::vector<PrefixedIPv4Address>& val);

	void setManual(std::vector<PrefixedIPv4Address>&& val);

private:
	//@ mandatory=false
	//@ name=Manual
	//@ order=0
	std::vector<PrefixedIPv4Address> _manual;

	//@ mandatory=false
	//@ name=LinkLocal
	//@ order=1
	Poco::SharedPtr<PrefixedIPv4Address> _linkLocal;

	//@ mandatory=false
	//@ name=FromDHCP
	//@ order=2
	Poco::SharedPtr<PrefixedIPv4Address> _fromDHCP;

	//@ name=DHCP
	//@ order=3
	bool _dHCP;

};


inline bool IPv4Configuration::getDHCP() const
{
	return _dHCP;
}


inline const Poco::SharedPtr<PrefixedIPv4Address>& IPv4Configuration::getFromDHCP() const
{
	return _fromDHCP;
}


inline const Poco::SharedPtr<PrefixedIPv4Address>& IPv4Configuration::getLinkLocal() const
{
	return _linkLocal;
}


inline const std::vector<PrefixedIPv4Address>& IPv4Configuration::getManual() const
{
	return _manual;
}


inline std::vector<PrefixedIPv4Address>& IPv4Configuration::getManual()
{
	return _manual;
}


inline void IPv4Configuration::setDHCP(bool val)
{
	_dHCP = val;
}


inline void IPv4Configuration::setFromDHCP(const Poco::SharedPtr<PrefixedIPv4Address>& val)
{
	_fromDHCP = val;
}


inline void IPv4Configuration::setFromDHCP(Poco::SharedPtr<PrefixedIPv4Address>&& val)
{
	_fromDHCP = std::move(val);
}


inline void IPv4Configuration::setLinkLocal(const Poco::SharedPtr<PrefixedIPv4Address>& val)
{
	_linkLocal = val;
}


inline void IPv4Configuration::setLinkLocal(Poco::SharedPtr<PrefixedIPv4Address>&& val)
{
	_linkLocal = std::move(val);
}


inline void IPv4Configuration::setManual(const std::vector<PrefixedIPv4Address>& val)
{
	_manual = val;
}


inline void IPv4Configuration::setManual(std::vector<PrefixedIPv4Address>&& val)
{
	_manual = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PrefixedIPv4Address.h"


#endif // ONVIF_IPv4Configuration_INCLUDED
