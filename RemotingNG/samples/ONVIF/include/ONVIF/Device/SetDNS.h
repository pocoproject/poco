// 
// SetDNS.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetDNS_INCLUDED
#define ONVIF_Device_SetDNS_INCLUDED


#include "ONVIF/IPAddress.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetDNS"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetDNS
{
public:
	SetDNS();

	SetDNS(
		bool fromDHCP, 
		const std::vector<std::string>& searchDomain, 
		const std::vector<ONVIF::IPAddress>& dNSManual);

	virtual ~SetDNS();

	const std::vector<ONVIF::IPAddress>& getDNSManual() const;

	std::vector<ONVIF::IPAddress>& getDNSManual();

	bool getFromDHCP() const;

	const std::vector<std::string>& getSearchDomain() const;

	std::vector<std::string>& getSearchDomain();

	void setDNSManual(const std::vector<ONVIF::IPAddress>& val);

	void setDNSManual(std::vector<ONVIF::IPAddress>&& val);

	void setFromDHCP(bool val);

	void setSearchDomain(const std::vector<std::string>& val);

	void setSearchDomain(std::vector<std::string>&& val);

private:
	//@ name=FromDHCP
	//@ order=0
	bool _fromDHCP;

	//@ mandatory=false
	//@ name=SearchDomain
	//@ order=1
	std::vector<std::string> _searchDomain;

	//@ mandatory=false
	//@ name=DNSManual
	//@ order=2
	std::vector<ONVIF::IPAddress> _dNSManual;

};


inline const std::vector<ONVIF::IPAddress>& SetDNS::getDNSManual() const
{
	return _dNSManual;
}


inline std::vector<ONVIF::IPAddress>& SetDNS::getDNSManual()
{
	return _dNSManual;
}


inline bool SetDNS::getFromDHCP() const
{
	return _fromDHCP;
}


inline const std::vector<std::string>& SetDNS::getSearchDomain() const
{
	return _searchDomain;
}


inline std::vector<std::string>& SetDNS::getSearchDomain()
{
	return _searchDomain;
}


inline void SetDNS::setDNSManual(const std::vector<ONVIF::IPAddress>& val)
{
	_dNSManual = val;
}


inline void SetDNS::setDNSManual(std::vector<ONVIF::IPAddress>&& val)
{
	_dNSManual = std::move(val);
}


inline void SetDNS::setFromDHCP(bool val)
{
	_fromDHCP = val;
}


inline void SetDNS::setSearchDomain(const std::vector<std::string>& val)
{
	_searchDomain = val;
}


inline void SetDNS::setSearchDomain(std::vector<std::string>&& val)
{
	_searchDomain = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetDNS_INCLUDED
