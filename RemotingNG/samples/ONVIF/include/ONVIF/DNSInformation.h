// 
// DNSInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DNSInformation_INCLUDED
#define ONVIF_DNSInformation_INCLUDED


#include "ONVIF/IPAddress.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class DNSInformationExtension;
} 


namespace ONVIF {


//@ name=DNSInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DNSInformation
{
public:
	DNSInformation();

	DNSInformation(
		bool fromDHCP, 
		const std::vector<std::string>& searchDomain, 
		const std::vector<IPAddress>& dNSFromDHCP, 
		const std::vector<IPAddress>& dNSManual, 
		const Poco::SharedPtr<DNSInformationExtension>& extension);

	virtual ~DNSInformation();

	const std::vector<IPAddress>& getDNSFromDHCP() const;

	std::vector<IPAddress>& getDNSFromDHCP();

	const std::vector<IPAddress>& getDNSManual() const;

	std::vector<IPAddress>& getDNSManual();

	const Poco::SharedPtr<DNSInformationExtension>& getExtension() const;

	bool getFromDHCP() const;

	const std::vector<std::string>& getSearchDomain() const;

	std::vector<std::string>& getSearchDomain();

	void setDNSFromDHCP(const std::vector<IPAddress>& val);

	void setDNSFromDHCP(std::vector<IPAddress>&& val);

	void setDNSManual(const std::vector<IPAddress>& val);

	void setDNSManual(std::vector<IPAddress>&& val);

	void setExtension(const Poco::SharedPtr<DNSInformationExtension>& val);

	void setExtension(Poco::SharedPtr<DNSInformationExtension>&& val);

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
	//@ name=DNSFromDHCP
	//@ order=2
	std::vector<IPAddress> _dNSFromDHCP;

	//@ mandatory=false
	//@ name=DNSManual
	//@ order=3
	std::vector<IPAddress> _dNSManual;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<DNSInformationExtension> _extension;

};


inline const std::vector<IPAddress>& DNSInformation::getDNSFromDHCP() const
{
	return _dNSFromDHCP;
}


inline std::vector<IPAddress>& DNSInformation::getDNSFromDHCP()
{
	return _dNSFromDHCP;
}


inline const std::vector<IPAddress>& DNSInformation::getDNSManual() const
{
	return _dNSManual;
}


inline std::vector<IPAddress>& DNSInformation::getDNSManual()
{
	return _dNSManual;
}


inline const Poco::SharedPtr<DNSInformationExtension>& DNSInformation::getExtension() const
{
	return _extension;
}


inline bool DNSInformation::getFromDHCP() const
{
	return _fromDHCP;
}


inline const std::vector<std::string>& DNSInformation::getSearchDomain() const
{
	return _searchDomain;
}


inline std::vector<std::string>& DNSInformation::getSearchDomain()
{
	return _searchDomain;
}


inline void DNSInformation::setDNSFromDHCP(const std::vector<IPAddress>& val)
{
	_dNSFromDHCP = val;
}


inline void DNSInformation::setDNSFromDHCP(std::vector<IPAddress>&& val)
{
	_dNSFromDHCP = std::move(val);
}


inline void DNSInformation::setDNSManual(const std::vector<IPAddress>& val)
{
	_dNSManual = val;
}


inline void DNSInformation::setDNSManual(std::vector<IPAddress>&& val)
{
	_dNSManual = std::move(val);
}


inline void DNSInformation::setExtension(const Poco::SharedPtr<DNSInformationExtension>& val)
{
	_extension = val;
}


inline void DNSInformation::setExtension(Poco::SharedPtr<DNSInformationExtension>&& val)
{
	_extension = std::move(val);
}


inline void DNSInformation::setFromDHCP(bool val)
{
	_fromDHCP = val;
}


inline void DNSInformation::setSearchDomain(const std::vector<std::string>& val)
{
	_searchDomain = val;
}


inline void DNSInformation::setSearchDomain(std::vector<std::string>&& val)
{
	_searchDomain = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/DNSInformationExtension.h"


#endif // ONVIF_DNSInformation_INCLUDED
