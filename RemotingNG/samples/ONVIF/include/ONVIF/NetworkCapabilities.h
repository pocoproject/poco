// 
// NetworkCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkCapabilities_INCLUDED
#define ONVIF_NetworkCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class NetworkCapabilitiesExtension;
} 


namespace ONVIF {


//@ name=NetworkCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkCapabilities
{
public:
	NetworkCapabilities();

	NetworkCapabilities(
		Poco::Optional<bool> iPFilter, 
		Poco::Optional<bool> zeroConfiguration, 
		Poco::Optional<bool> iPVersion6, 
		Poco::Optional<bool> dynDNS, 
		const Poco::SharedPtr<NetworkCapabilitiesExtension>& extension);

	virtual ~NetworkCapabilities();

	Poco::Optional<bool> getDynDNS() const;

	const Poco::SharedPtr<NetworkCapabilitiesExtension>& getExtension() const;

	Poco::Optional<bool> getIPFilter() const;

	Poco::Optional<bool> getIPVersion6() const;

	Poco::Optional<bool> getZeroConfiguration() const;

	void setDynDNS(Poco::Optional<bool> val);

	void setExtension(const Poco::SharedPtr<NetworkCapabilitiesExtension>& val);

	void setExtension(Poco::SharedPtr<NetworkCapabilitiesExtension>&& val);

	void setIPFilter(Poco::Optional<bool> val);

	void setIPVersion6(Poco::Optional<bool> val);

	void setZeroConfiguration(Poco::Optional<bool> val);

private:
	//@ mandatory=false
	//@ name=IPFilter
	//@ order=0
	Poco::Optional<bool> _iPFilter;

	//@ mandatory=false
	//@ name=ZeroConfiguration
	//@ order=1
	Poco::Optional<bool> _zeroConfiguration;

	//@ mandatory=false
	//@ name=IPVersion6
	//@ order=2
	Poco::Optional<bool> _iPVersion6;

	//@ mandatory=false
	//@ name=DynDNS
	//@ order=3
	Poco::Optional<bool> _dynDNS;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<NetworkCapabilitiesExtension> _extension;

};


inline Poco::Optional<bool> NetworkCapabilities::getDynDNS() const
{
	return _dynDNS;
}


inline const Poco::SharedPtr<NetworkCapabilitiesExtension>& NetworkCapabilities::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> NetworkCapabilities::getIPFilter() const
{
	return _iPFilter;
}


inline Poco::Optional<bool> NetworkCapabilities::getIPVersion6() const
{
	return _iPVersion6;
}


inline Poco::Optional<bool> NetworkCapabilities::getZeroConfiguration() const
{
	return _zeroConfiguration;
}


inline void NetworkCapabilities::setDynDNS(Poco::Optional<bool> val)
{
	_dynDNS = val;
}


inline void NetworkCapabilities::setExtension(const Poco::SharedPtr<NetworkCapabilitiesExtension>& val)
{
	_extension = val;
}


inline void NetworkCapabilities::setExtension(Poco::SharedPtr<NetworkCapabilitiesExtension>&& val)
{
	_extension = std::move(val);
}


inline void NetworkCapabilities::setIPFilter(Poco::Optional<bool> val)
{
	_iPFilter = val;
}


inline void NetworkCapabilities::setIPVersion6(Poco::Optional<bool> val)
{
	_iPVersion6 = val;
}


inline void NetworkCapabilities::setZeroConfiguration(Poco::Optional<bool> val)
{
	_zeroConfiguration = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/NetworkCapabilitiesExtension.h"


#endif // ONVIF_NetworkCapabilities_INCLUDED
