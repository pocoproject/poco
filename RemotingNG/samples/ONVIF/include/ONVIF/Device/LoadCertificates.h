// 
// LoadCertificates.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_LoadCertificates_INCLUDED
#define ONVIF_Device_LoadCertificates_INCLUDED


#include "ONVIF/Certificate.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#LoadCertificates"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API LoadCertificates
{
public:
	LoadCertificates();

	LoadCertificates(const std::vector<ONVIF::Certificate>& nVTCertificate);

	virtual ~LoadCertificates();

	const std::vector<ONVIF::Certificate>& getNVTCertificate() const;

	std::vector<ONVIF::Certificate>& getNVTCertificate();

	void setNVTCertificate(const std::vector<ONVIF::Certificate>& val);

	void setNVTCertificate(std::vector<ONVIF::Certificate>&& val);

private:
	//@ name=NVTCertificate
	//@ order=0
	std::vector<ONVIF::Certificate> _nVTCertificate;

};


inline const std::vector<ONVIF::Certificate>& LoadCertificates::getNVTCertificate() const
{
	return _nVTCertificate;
}


inline std::vector<ONVIF::Certificate>& LoadCertificates::getNVTCertificate()
{
	return _nVTCertificate;
}


inline void LoadCertificates::setNVTCertificate(const std::vector<ONVIF::Certificate>& val)
{
	_nVTCertificate = val;
}


inline void LoadCertificates::setNVTCertificate(std::vector<ONVIF::Certificate>&& val)
{
	_nVTCertificate = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_LoadCertificates_INCLUDED
