// 
// LoadCertificateWithPrivateKey.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_LoadCertificateWithPrivateKey_INCLUDED
#define ONVIF_Device_LoadCertificateWithPrivateKey_INCLUDED


#include "ONVIF/CertificateWithPrivateKey.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#LoadCertificateWithPrivateKey"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API LoadCertificateWithPrivateKey
{
public:
	LoadCertificateWithPrivateKey();

	LoadCertificateWithPrivateKey(const std::vector<ONVIF::CertificateWithPrivateKey>& certificateWithPrivateKey);

	virtual ~LoadCertificateWithPrivateKey();

	const std::vector<ONVIF::CertificateWithPrivateKey>& getCertificateWithPrivateKey() const;

	std::vector<ONVIF::CertificateWithPrivateKey>& getCertificateWithPrivateKey();

	void setCertificateWithPrivateKey(const std::vector<ONVIF::CertificateWithPrivateKey>& val);

	void setCertificateWithPrivateKey(std::vector<ONVIF::CertificateWithPrivateKey>&& val);

private:
	//@ name=CertificateWithPrivateKey
	//@ order=0
	std::vector<ONVIF::CertificateWithPrivateKey> _certificateWithPrivateKey;

};


inline const std::vector<ONVIF::CertificateWithPrivateKey>& LoadCertificateWithPrivateKey::getCertificateWithPrivateKey() const
{
	return _certificateWithPrivateKey;
}


inline std::vector<ONVIF::CertificateWithPrivateKey>& LoadCertificateWithPrivateKey::getCertificateWithPrivateKey()
{
	return _certificateWithPrivateKey;
}


inline void LoadCertificateWithPrivateKey::setCertificateWithPrivateKey(const std::vector<ONVIF::CertificateWithPrivateKey>& val)
{
	_certificateWithPrivateKey = val;
}


inline void LoadCertificateWithPrivateKey::setCertificateWithPrivateKey(std::vector<ONVIF::CertificateWithPrivateKey>&& val)
{
	_certificateWithPrivateKey = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_LoadCertificateWithPrivateKey_INCLUDED
