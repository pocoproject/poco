// 
// DeleteCertificates.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_DeleteCertificates_INCLUDED
#define ONVIF_Device_DeleteCertificates_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#DeleteCertificates"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API DeleteCertificates
{
public:
	DeleteCertificates();

	DeleteCertificates(const std::vector<std::string>& certificateID);

	virtual ~DeleteCertificates();

	const std::vector<std::string>& getCertificateID() const;

	std::vector<std::string>& getCertificateID();

	void setCertificateID(const std::vector<std::string>& val);

	void setCertificateID(std::vector<std::string>&& val);

private:
	//@ name=CertificateID
	//@ order=0
	std::vector<std::string> _certificateID;

};


inline const std::vector<std::string>& DeleteCertificates::getCertificateID() const
{
	return _certificateID;
}


inline std::vector<std::string>& DeleteCertificates::getCertificateID()
{
	return _certificateID;
}


inline void DeleteCertificates::setCertificateID(const std::vector<std::string>& val)
{
	_certificateID = val;
}


inline void DeleteCertificates::setCertificateID(std::vector<std::string>&& val)
{
	_certificateID = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_DeleteCertificates_INCLUDED
