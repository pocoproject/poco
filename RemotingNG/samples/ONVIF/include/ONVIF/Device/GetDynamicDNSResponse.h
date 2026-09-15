// 
// GetDynamicDNSResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetDynamicDNSResponse_INCLUDED
#define ONVIF_Device_GetDynamicDNSResponse_INCLUDED


#include "ONVIF/DynamicDNSInformation.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetDynamicDNSResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetDynamicDNSResponse
{
public:
	GetDynamicDNSResponse();

	GetDynamicDNSResponse(const ONVIF::DynamicDNSInformation& dynamicDNSInformation);

	virtual ~GetDynamicDNSResponse();

	const ONVIF::DynamicDNSInformation& getDynamicDNSInformation() const;

	void setDynamicDNSInformation(const ONVIF::DynamicDNSInformation& val);

	void setDynamicDNSInformation(ONVIF::DynamicDNSInformation&& val);

private:
	//@ name=DynamicDNSInformation
	//@ order=0
	ONVIF::DynamicDNSInformation _dynamicDNSInformation;

};


inline const ONVIF::DynamicDNSInformation& GetDynamicDNSResponse::getDynamicDNSInformation() const
{
	return _dynamicDNSInformation;
}


inline void GetDynamicDNSResponse::setDynamicDNSInformation(const ONVIF::DynamicDNSInformation& val)
{
	_dynamicDNSInformation = val;
}


inline void GetDynamicDNSResponse::setDynamicDNSInformation(ONVIF::DynamicDNSInformation&& val)
{
	_dynamicDNSInformation = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetDynamicDNSResponse_INCLUDED
