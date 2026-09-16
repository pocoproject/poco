// 
// GetDNSResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetDNSResponse_INCLUDED
#define ONVIF_Device_GetDNSResponse_INCLUDED


#include "ONVIF/DNSInformation.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetDNSResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetDNSResponse
{
public:
	GetDNSResponse();

	GetDNSResponse(const ONVIF::DNSInformation& dNSInformation);

	virtual ~GetDNSResponse();

	const ONVIF::DNSInformation& getDNSInformation() const;

	void setDNSInformation(const ONVIF::DNSInformation& val);

	void setDNSInformation(ONVIF::DNSInformation&& val);

private:
	//@ name=DNSInformation
	//@ order=0
	ONVIF::DNSInformation _dNSInformation;

};


inline const ONVIF::DNSInformation& GetDNSResponse::getDNSInformation() const
{
	return _dNSInformation;
}


inline void GetDNSResponse::setDNSInformation(const ONVIF::DNSInformation& val)
{
	_dNSInformation = val;
}


inline void GetDNSResponse::setDNSInformation(ONVIF::DNSInformation&& val)
{
	_dNSInformation = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetDNSResponse_INCLUDED
