// 
// GetCertificatesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetCertificatesResponse_INCLUDED
#define ONVIF_Device_GetCertificatesResponse_INCLUDED


#include "ONVIF/Certificate.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetCertificatesResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetCertificatesResponse
{
public:
	GetCertificatesResponse();

	GetCertificatesResponse(const std::vector<ONVIF::Certificate>& nvtCertificate);

	virtual ~GetCertificatesResponse();

	const std::vector<ONVIF::Certificate>& getNvtCertificate() const;

	std::vector<ONVIF::Certificate>& getNvtCertificate();

	void setNvtCertificate(const std::vector<ONVIF::Certificate>& val);

	void setNvtCertificate(std::vector<ONVIF::Certificate>&& val);

private:
	//@ mandatory=false
	//@ name=NvtCertificate
	//@ order=0
	std::vector<ONVIF::Certificate> _nvtCertificate;

};


inline const std::vector<ONVIF::Certificate>& GetCertificatesResponse::getNvtCertificate() const
{
	return _nvtCertificate;
}


inline std::vector<ONVIF::Certificate>& GetCertificatesResponse::getNvtCertificate()
{
	return _nvtCertificate;
}


inline void GetCertificatesResponse::setNvtCertificate(const std::vector<ONVIF::Certificate>& val)
{
	_nvtCertificate = val;
}


inline void GetCertificatesResponse::setNvtCertificate(std::vector<ONVIF::Certificate>&& val)
{
	_nvtCertificate = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetCertificatesResponse_INCLUDED
