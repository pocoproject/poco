// 
// CreateCertificateResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_CreateCertificateResponse_INCLUDED
#define ONVIF_Device_CreateCertificateResponse_INCLUDED


#include "ONVIF/Certificate.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#CreateCertificateResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API CreateCertificateResponse
{
public:
	CreateCertificateResponse();

	CreateCertificateResponse(const ONVIF::Certificate& nvtCertificate);

	virtual ~CreateCertificateResponse();

	const ONVIF::Certificate& getNvtCertificate() const;

	void setNvtCertificate(const ONVIF::Certificate& val);

	void setNvtCertificate(ONVIF::Certificate&& val);

private:
	//@ name=NvtCertificate
	//@ order=0
	ONVIF::Certificate _nvtCertificate;

};


inline const ONVIF::Certificate& CreateCertificateResponse::getNvtCertificate() const
{
	return _nvtCertificate;
}


inline void CreateCertificateResponse::setNvtCertificate(const ONVIF::Certificate& val)
{
	_nvtCertificate = val;
}


inline void CreateCertificateResponse::setNvtCertificate(ONVIF::Certificate&& val)
{
	_nvtCertificate = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_CreateCertificateResponse_INCLUDED
