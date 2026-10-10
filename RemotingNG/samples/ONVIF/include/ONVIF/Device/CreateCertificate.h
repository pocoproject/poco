// 
// CreateCertificate.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_CreateCertificate_INCLUDED
#define ONVIF_Device_CreateCertificate_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#CreateCertificate"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API CreateCertificate
{
public:
	CreateCertificate();

	CreateCertificate(
		const Poco::Optional<std::string>& certificateID, 
		const Poco::Optional<std::string>& subject, 
		const Poco::Optional<Poco::DateTime>& validNotBefore, 
		const Poco::Optional<Poco::DateTime>& validNotAfter);

	virtual ~CreateCertificate();

	const Poco::Optional<std::string>& getCertificateID() const;

	const Poco::Optional<std::string>& getSubject() const;

	//@ return={xsdType=dateTime}
	const Poco::Optional<Poco::DateTime>& getValidNotAfter() const;

	//@ return={xsdType=dateTime}
	const Poco::Optional<Poco::DateTime>& getValidNotBefore() const;

	void setCertificateID(const Poco::Optional<std::string>& val);

	void setCertificateID(Poco::Optional<std::string>&& val);

	void setSubject(const Poco::Optional<std::string>& val);

	void setSubject(Poco::Optional<std::string>&& val);

	//@ $val={xsdType=dateTime}
	void setValidNotAfter(const Poco::Optional<Poco::DateTime>& val);

	//@ $val={xsdType=dateTime}
	void setValidNotAfter(Poco::Optional<Poco::DateTime>&& val);

	//@ $val={xsdType=dateTime}
	void setValidNotBefore(const Poco::Optional<Poco::DateTime>& val);

	//@ $val={xsdType=dateTime}
	void setValidNotBefore(Poco::Optional<Poco::DateTime>&& val);

private:
	//@ mandatory=false
	//@ name=CertificateID
	//@ order=0
	Poco::Optional<std::string> _certificateID;

	//@ mandatory=false
	//@ name=Subject
	//@ order=1
	Poco::Optional<std::string> _subject;

	//@ mandatory=false
	//@ name=ValidNotBefore
	//@ order=2
	//@ xsdType=dateTime
	Poco::Optional<Poco::DateTime> _validNotBefore;

	//@ mandatory=false
	//@ name=ValidNotAfter
	//@ order=3
	//@ xsdType=dateTime
	Poco::Optional<Poco::DateTime> _validNotAfter;

};


inline const Poco::Optional<std::string>& CreateCertificate::getCertificateID() const
{
	return _certificateID;
}


inline const Poco::Optional<std::string>& CreateCertificate::getSubject() const
{
	return _subject;
}


inline const Poco::Optional<Poco::DateTime>& CreateCertificate::getValidNotAfter() const
{
	return _validNotAfter;
}


inline const Poco::Optional<Poco::DateTime>& CreateCertificate::getValidNotBefore() const
{
	return _validNotBefore;
}


inline void CreateCertificate::setCertificateID(const Poco::Optional<std::string>& val)
{
	_certificateID = val;
}


inline void CreateCertificate::setCertificateID(Poco::Optional<std::string>&& val)
{
	_certificateID = std::move(val);
}


inline void CreateCertificate::setSubject(const Poco::Optional<std::string>& val)
{
	_subject = val;
}


inline void CreateCertificate::setSubject(Poco::Optional<std::string>&& val)
{
	_subject = std::move(val);
}


inline void CreateCertificate::setValidNotAfter(const Poco::Optional<Poco::DateTime>& val)
{
	_validNotAfter = val;
}


inline void CreateCertificate::setValidNotAfter(Poco::Optional<Poco::DateTime>&& val)
{
	_validNotAfter = std::move(val);
}


inline void CreateCertificate::setValidNotBefore(const Poco::Optional<Poco::DateTime>& val)
{
	_validNotBefore = val;
}


inline void CreateCertificate::setValidNotBefore(Poco::Optional<Poco::DateTime>&& val)
{
	_validNotBefore = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_CreateCertificate_INCLUDED
