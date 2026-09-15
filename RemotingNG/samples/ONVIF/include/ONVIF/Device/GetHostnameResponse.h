// 
// GetHostnameResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetHostnameResponse_INCLUDED
#define ONVIF_Device_GetHostnameResponse_INCLUDED


#include "ONVIF/HostnameInformation.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetHostnameResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetHostnameResponse
{
public:
	GetHostnameResponse();

	GetHostnameResponse(const ONVIF::HostnameInformation& hostnameInformation);

	virtual ~GetHostnameResponse();

	const ONVIF::HostnameInformation& getHostnameInformation() const;

	void setHostnameInformation(const ONVIF::HostnameInformation& val);

	void setHostnameInformation(ONVIF::HostnameInformation&& val);

private:
	//@ name=HostnameInformation
	//@ order=0
	ONVIF::HostnameInformation _hostnameInformation;

};


inline const ONVIF::HostnameInformation& GetHostnameResponse::getHostnameInformation() const
{
	return _hostnameInformation;
}


inline void GetHostnameResponse::setHostnameInformation(const ONVIF::HostnameInformation& val)
{
	_hostnameInformation = val;
}


inline void GetHostnameResponse::setHostnameInformation(ONVIF::HostnameInformation&& val)
{
	_hostnameInformation = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetHostnameResponse_INCLUDED
