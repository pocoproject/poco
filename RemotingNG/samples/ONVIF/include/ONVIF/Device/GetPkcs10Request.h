// 
// GetPkcs10Request.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetPkcs10Request_INCLUDED
#define ONVIF_Device_GetPkcs10Request_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class BinaryData;
} 


namespace ONVIF {
namespace Device {


//@ name="#GetPkcs10Request"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetPkcs10Request
{
public:
	GetPkcs10Request();

	GetPkcs10Request(
		const std::string& certificateID, 
		const Poco::Optional<std::string>& subject, 
		const Poco::SharedPtr<ONVIF::BinaryData>& attributes);

	virtual ~GetPkcs10Request();

	const Poco::SharedPtr<ONVIF::BinaryData>& getAttributes() const;

	const std::string& getCertificateID() const;

	const Poco::Optional<std::string>& getSubject() const;

	void setAttributes(const Poco::SharedPtr<ONVIF::BinaryData>& val);

	void setAttributes(Poco::SharedPtr<ONVIF::BinaryData>&& val);

	void setCertificateID(const std::string& val);

	void setCertificateID(std::string&& val);

	void setSubject(const Poco::Optional<std::string>& val);

	void setSubject(Poco::Optional<std::string>&& val);

private:
	//@ name=CertificateID
	//@ order=0
	std::string _certificateID;

	//@ mandatory=false
	//@ name=Subject
	//@ order=1
	Poco::Optional<std::string> _subject;

	//@ mandatory=false
	//@ name=Attributes
	//@ order=2
	Poco::SharedPtr<ONVIF::BinaryData> _attributes;

};


inline const Poco::SharedPtr<ONVIF::BinaryData>& GetPkcs10Request::getAttributes() const
{
	return _attributes;
}


inline const std::string& GetPkcs10Request::getCertificateID() const
{
	return _certificateID;
}


inline const Poco::Optional<std::string>& GetPkcs10Request::getSubject() const
{
	return _subject;
}


inline void GetPkcs10Request::setAttributes(const Poco::SharedPtr<ONVIF::BinaryData>& val)
{
	_attributes = val;
}


inline void GetPkcs10Request::setAttributes(Poco::SharedPtr<ONVIF::BinaryData>&& val)
{
	_attributes = std::move(val);
}


inline void GetPkcs10Request::setCertificateID(const std::string& val)
{
	_certificateID = val;
}


inline void GetPkcs10Request::setCertificateID(std::string&& val)
{
	_certificateID = std::move(val);
}


inline void GetPkcs10Request::setSubject(const Poco::Optional<std::string>& val)
{
	_subject = val;
}


inline void GetPkcs10Request::setSubject(Poco::Optional<std::string>&& val)
{
	_subject = std::move(val);
}


} } // ONVIF::Device


// The following headers are required for template instantiation.
#include "ONVIF/BinaryData.h"


#endif // ONVIF_Device_GetPkcs10Request_INCLUDED
