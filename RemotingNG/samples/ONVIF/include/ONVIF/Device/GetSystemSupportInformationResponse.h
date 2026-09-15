// 
// GetSystemSupportInformationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetSystemSupportInformationResponse_INCLUDED
#define ONVIF_Device_GetSystemSupportInformationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/SupportInformation.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetSystemSupportInformationResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetSystemSupportInformationResponse
{
public:
	GetSystemSupportInformationResponse();

	GetSystemSupportInformationResponse(const ONVIF::SupportInformation& supportInformation);

	virtual ~GetSystemSupportInformationResponse();

	const ONVIF::SupportInformation& getSupportInformation() const;

	void setSupportInformation(const ONVIF::SupportInformation& val);

	void setSupportInformation(ONVIF::SupportInformation&& val);

private:
	//@ name=SupportInformation
	//@ order=0
	ONVIF::SupportInformation _supportInformation;

};


inline const ONVIF::SupportInformation& GetSystemSupportInformationResponse::getSupportInformation() const
{
	return _supportInformation;
}


inline void GetSystemSupportInformationResponse::setSupportInformation(const ONVIF::SupportInformation& val)
{
	_supportInformation = val;
}


inline void GetSystemSupportInformationResponse::setSupportInformation(ONVIF::SupportInformation&& val)
{
	_supportInformation = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetSystemSupportInformationResponse_INCLUDED
