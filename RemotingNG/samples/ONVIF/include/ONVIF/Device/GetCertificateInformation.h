// 
// GetCertificateInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetCertificateInformation_INCLUDED
#define ONVIF_Device_GetCertificateInformation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetCertificateInformation"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetCertificateInformation
{
public:
	GetCertificateInformation();

	GetCertificateInformation(const std::string& certificateID);

	virtual ~GetCertificateInformation();

	const std::string& getCertificateID() const;

	void setCertificateID(const std::string& val);

	void setCertificateID(std::string&& val);

private:
	//@ name=CertificateID
	//@ order=0
	std::string _certificateID;

};


inline const std::string& GetCertificateInformation::getCertificateID() const
{
	return _certificateID;
}


inline void GetCertificateInformation::setCertificateID(const std::string& val)
{
	_certificateID = val;
}


inline void GetCertificateInformation::setCertificateID(std::string&& val)
{
	_certificateID = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetCertificateInformation_INCLUDED
