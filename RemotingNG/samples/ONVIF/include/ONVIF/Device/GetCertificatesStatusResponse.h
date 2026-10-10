// 
// GetCertificatesStatusResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetCertificatesStatusResponse_INCLUDED
#define ONVIF_Device_GetCertificatesStatusResponse_INCLUDED


#include "ONVIF/CertificateStatus.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetCertificatesStatusResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetCertificatesStatusResponse
{
public:
	GetCertificatesStatusResponse();

	GetCertificatesStatusResponse(const std::vector<ONVIF::CertificateStatus>& certificateStatus);

	virtual ~GetCertificatesStatusResponse();

	const std::vector<ONVIF::CertificateStatus>& getCertificateStatus() const;

	std::vector<ONVIF::CertificateStatus>& getCertificateStatus();

	void setCertificateStatus(const std::vector<ONVIF::CertificateStatus>& val);

	void setCertificateStatus(std::vector<ONVIF::CertificateStatus>&& val);

private:
	//@ mandatory=false
	//@ name=CertificateStatus
	//@ order=0
	std::vector<ONVIF::CertificateStatus> _certificateStatus;

};


inline const std::vector<ONVIF::CertificateStatus>& GetCertificatesStatusResponse::getCertificateStatus() const
{
	return _certificateStatus;
}


inline std::vector<ONVIF::CertificateStatus>& GetCertificatesStatusResponse::getCertificateStatus()
{
	return _certificateStatus;
}


inline void GetCertificatesStatusResponse::setCertificateStatus(const std::vector<ONVIF::CertificateStatus>& val)
{
	_certificateStatus = val;
}


inline void GetCertificatesStatusResponse::setCertificateStatus(std::vector<ONVIF::CertificateStatus>&& val)
{
	_certificateStatus = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetCertificatesStatusResponse_INCLUDED
