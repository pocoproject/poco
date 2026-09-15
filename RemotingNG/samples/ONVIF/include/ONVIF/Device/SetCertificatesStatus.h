// 
// SetCertificatesStatus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetCertificatesStatus_INCLUDED
#define ONVIF_Device_SetCertificatesStatus_INCLUDED


#include "ONVIF/CertificateStatus.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetCertificatesStatus"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetCertificatesStatus
{
public:
	SetCertificatesStatus();

	SetCertificatesStatus(const std::vector<ONVIF::CertificateStatus>& certificateStatus);

	virtual ~SetCertificatesStatus();

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


inline const std::vector<ONVIF::CertificateStatus>& SetCertificatesStatus::getCertificateStatus() const
{
	return _certificateStatus;
}


inline std::vector<ONVIF::CertificateStatus>& SetCertificatesStatus::getCertificateStatus()
{
	return _certificateStatus;
}


inline void SetCertificatesStatus::setCertificateStatus(const std::vector<ONVIF::CertificateStatus>& val)
{
	_certificateStatus = val;
}


inline void SetCertificatesStatus::setCertificateStatus(std::vector<ONVIF::CertificateStatus>&& val)
{
	_certificateStatus = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetCertificatesStatus_INCLUDED
