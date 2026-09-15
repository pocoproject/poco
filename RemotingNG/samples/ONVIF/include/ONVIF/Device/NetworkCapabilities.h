// 
// NetworkCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_NetworkCapabilities_INCLUDED
#define ONVIF_Device_NetworkCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name=NetworkCapabilities
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
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
		Poco::Optional<bool> dot11Configuration, 
		Poco::Optional<int> dot1XConfigurations, 
		Poco::Optional<bool> hostnameFromDHCP, 
		Poco::Optional<int> nTP, 
		Poco::Optional<bool> dHCPv6);

	virtual ~NetworkCapabilities();

	Poco::Optional<bool> getDHCPv6() const;

	Poco::Optional<bool> getDot11Configuration() const;

	Poco::Optional<int> getDot1XConfigurations() const;

	Poco::Optional<bool> getDynDNS() const;

	Poco::Optional<bool> getHostnameFromDHCP() const;

	Poco::Optional<bool> getIPFilter() const;

	Poco::Optional<bool> getIPVersion6() const;

	Poco::Optional<int> getNTP() const;

	Poco::Optional<bool> getZeroConfiguration() const;

	void setDHCPv6(Poco::Optional<bool> val);

	void setDot11Configuration(Poco::Optional<bool> val);

	void setDot1XConfigurations(Poco::Optional<int> val);

	void setDynDNS(Poco::Optional<bool> val);

	void setHostnameFromDHCP(Poco::Optional<bool> val);

	void setIPFilter(Poco::Optional<bool> val);

	void setIPVersion6(Poco::Optional<bool> val);

	void setNTP(Poco::Optional<int> val);

	void setZeroConfiguration(Poco::Optional<bool> val);

private:
	//@ mandatory=false
	//@ name=IPFilter
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _iPFilter;

	//@ mandatory=false
	//@ name=ZeroConfiguration
	//@ order=1
	//@ type=attr
	Poco::Optional<bool> _zeroConfiguration;

	//@ mandatory=false
	//@ name=IPVersion6
	//@ order=2
	//@ type=attr
	Poco::Optional<bool> _iPVersion6;

	//@ mandatory=false
	//@ name=DynDNS
	//@ order=3
	//@ type=attr
	Poco::Optional<bool> _dynDNS;

	//@ mandatory=false
	//@ name=Dot11Configuration
	//@ order=4
	//@ type=attr
	Poco::Optional<bool> _dot11Configuration;

	//@ mandatory=false
	//@ name=Dot1XConfigurations
	//@ order=5
	//@ type=attr
	Poco::Optional<int> _dot1XConfigurations;

	//@ mandatory=false
	//@ name=HostnameFromDHCP
	//@ order=6
	//@ type=attr
	Poco::Optional<bool> _hostnameFromDHCP;

	//@ mandatory=false
	//@ name=NTP
	//@ order=7
	//@ type=attr
	Poco::Optional<int> _nTP;

	//@ mandatory=false
	//@ name=DHCPv6
	//@ order=8
	//@ type=attr
	Poco::Optional<bool> _dHCPv6;

};


inline Poco::Optional<bool> NetworkCapabilities::getDHCPv6() const
{
	return _dHCPv6;
}


inline Poco::Optional<bool> NetworkCapabilities::getDot11Configuration() const
{
	return _dot11Configuration;
}


inline Poco::Optional<int> NetworkCapabilities::getDot1XConfigurations() const
{
	return _dot1XConfigurations;
}


inline Poco::Optional<bool> NetworkCapabilities::getDynDNS() const
{
	return _dynDNS;
}


inline Poco::Optional<bool> NetworkCapabilities::getHostnameFromDHCP() const
{
	return _hostnameFromDHCP;
}


inline Poco::Optional<bool> NetworkCapabilities::getIPFilter() const
{
	return _iPFilter;
}


inline Poco::Optional<bool> NetworkCapabilities::getIPVersion6() const
{
	return _iPVersion6;
}


inline Poco::Optional<int> NetworkCapabilities::getNTP() const
{
	return _nTP;
}


inline Poco::Optional<bool> NetworkCapabilities::getZeroConfiguration() const
{
	return _zeroConfiguration;
}


inline void NetworkCapabilities::setDHCPv6(Poco::Optional<bool> val)
{
	_dHCPv6 = val;
}


inline void NetworkCapabilities::setDot11Configuration(Poco::Optional<bool> val)
{
	_dot11Configuration = val;
}


inline void NetworkCapabilities::setDot1XConfigurations(Poco::Optional<int> val)
{
	_dot1XConfigurations = val;
}


inline void NetworkCapabilities::setDynDNS(Poco::Optional<bool> val)
{
	_dynDNS = val;
}


inline void NetworkCapabilities::setHostnameFromDHCP(Poco::Optional<bool> val)
{
	_hostnameFromDHCP = val;
}


inline void NetworkCapabilities::setIPFilter(Poco::Optional<bool> val)
{
	_iPFilter = val;
}


inline void NetworkCapabilities::setIPVersion6(Poco::Optional<bool> val)
{
	_iPVersion6 = val;
}


inline void NetworkCapabilities::setNTP(Poco::Optional<int> val)
{
	_nTP = val;
}


inline void NetworkCapabilities::setZeroConfiguration(Poco::Optional<bool> val)
{
	_zeroConfiguration = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_NetworkCapabilities_INCLUDED
