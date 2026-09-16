// 
// GetCertificateInformationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetCertificateInformationResponse_INCLUDED
#define ONVIF_Device_GetCertificateInformationResponse_INCLUDED


#include "ONVIF/CertificateInformation.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetCertificateInformationResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetCertificateInformationResponse
{
public:
	GetCertificateInformationResponse();

	GetCertificateInformationResponse(const ONVIF::CertificateInformation& certificateInformation);

	virtual ~GetCertificateInformationResponse();

	const ONVIF::CertificateInformation& getCertificateInformation() const;

	void setCertificateInformation(const ONVIF::CertificateInformation& val);

	void setCertificateInformation(ONVIF::CertificateInformation&& val);

private:
	//@ name=CertificateInformation
	//@ order=0
	ONVIF::CertificateInformation _certificateInformation;

};


inline const ONVIF::CertificateInformation& GetCertificateInformationResponse::getCertificateInformation() const
{
	return _certificateInformation;
}


inline void GetCertificateInformationResponse::setCertificateInformation(const ONVIF::CertificateInformation& val)
{
	_certificateInformation = val;
}


inline void GetCertificateInformationResponse::setCertificateInformation(ONVIF::CertificateInformation&& val)
{
	_certificateInformation = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetCertificateInformationResponse_INCLUDED
