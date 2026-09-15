// 
// CertificateWithPrivateKey.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_CertificateWithPrivateKey_INCLUDED
#define ONVIF_CertificateWithPrivateKey_INCLUDED


#include "ONVIF/BinaryData.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=CertificateWithPrivateKey
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API CertificateWithPrivateKey
{
public:
	CertificateWithPrivateKey();

	CertificateWithPrivateKey(
		const Poco::Optional<std::string>& certificateID, 
		const BinaryData& certificate, 
		const BinaryData& privateKey);

	virtual ~CertificateWithPrivateKey();

	const BinaryData& getCertificate() const;

	const Poco::Optional<std::string>& getCertificateID() const;

	const BinaryData& getPrivateKey() const;

	void setCertificate(const BinaryData& val);

	void setCertificate(BinaryData&& val);

	void setCertificateID(const Poco::Optional<std::string>& val);

	void setCertificateID(Poco::Optional<std::string>&& val);

	void setPrivateKey(const BinaryData& val);

	void setPrivateKey(BinaryData&& val);

private:
	//@ mandatory=false
	//@ name=CertificateID
	//@ order=0
	Poco::Optional<std::string> _certificateID;

	//@ name=Certificate
	//@ order=1
	BinaryData _certificate;

	//@ name=PrivateKey
	//@ order=2
	BinaryData _privateKey;

};


inline const BinaryData& CertificateWithPrivateKey::getCertificate() const
{
	return _certificate;
}


inline const Poco::Optional<std::string>& CertificateWithPrivateKey::getCertificateID() const
{
	return _certificateID;
}


inline const BinaryData& CertificateWithPrivateKey::getPrivateKey() const
{
	return _privateKey;
}


inline void CertificateWithPrivateKey::setCertificate(const BinaryData& val)
{
	_certificate = val;
}


inline void CertificateWithPrivateKey::setCertificate(BinaryData&& val)
{
	_certificate = std::move(val);
}


inline void CertificateWithPrivateKey::setCertificateID(const Poco::Optional<std::string>& val)
{
	_certificateID = val;
}


inline void CertificateWithPrivateKey::setCertificateID(Poco::Optional<std::string>&& val)
{
	_certificateID = std::move(val);
}


inline void CertificateWithPrivateKey::setPrivateKey(const BinaryData& val)
{
	_privateKey = val;
}


inline void CertificateWithPrivateKey::setPrivateKey(BinaryData&& val)
{
	_privateKey = std::move(val);
}


} // ONVIF


#endif // ONVIF_CertificateWithPrivateKey_INCLUDED
