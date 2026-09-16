// 
// CertificateInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_CertificateInformation_INCLUDED
#define ONVIF_CertificateInformation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class CertificateInformationExtension;
class CertificateUsage;
class DateTimeRange;
} 


namespace ONVIF {


//@ name=CertificateInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API CertificateInformation
{
public:
	CertificateInformation();

	CertificateInformation(
		const std::string& certificateID, 
		const Poco::Optional<std::string>& issuerDN, 
		const Poco::Optional<std::string>& subjectDN, 
		const Poco::SharedPtr<CertificateUsage>& keyUsage, 
		const Poco::SharedPtr<CertificateUsage>& extendedKeyUsage, 
		Poco::Optional<int> keyLength, 
		const Poco::Optional<std::string>& version, 
		const Poco::Optional<std::string>& serialNum, 
		const Poco::Optional<std::string>& signatureAlgorithm, 
		const Poco::SharedPtr<DateTimeRange>& validity, 
		const Poco::SharedPtr<CertificateInformationExtension>& extension);

	virtual ~CertificateInformation();

	const std::string& getCertificateID() const;

	const Poco::SharedPtr<CertificateUsage>& getExtendedKeyUsage() const;

	const Poco::SharedPtr<CertificateInformationExtension>& getExtension() const;

	const Poco::Optional<std::string>& getIssuerDN() const;

	Poco::Optional<int> getKeyLength() const;

	const Poco::SharedPtr<CertificateUsage>& getKeyUsage() const;

	const Poco::Optional<std::string>& getSerialNum() const;

	const Poco::Optional<std::string>& getSignatureAlgorithm() const;

	const Poco::Optional<std::string>& getSubjectDN() const;

	const Poco::SharedPtr<DateTimeRange>& getValidity() const;

	const Poco::Optional<std::string>& getVersion() const;

	void setCertificateID(const std::string& val);

	void setCertificateID(std::string&& val);

	void setExtendedKeyUsage(const Poco::SharedPtr<CertificateUsage>& val);

	void setExtendedKeyUsage(Poco::SharedPtr<CertificateUsage>&& val);

	void setExtension(const Poco::SharedPtr<CertificateInformationExtension>& val);

	void setExtension(Poco::SharedPtr<CertificateInformationExtension>&& val);

	void setIssuerDN(const Poco::Optional<std::string>& val);

	void setIssuerDN(Poco::Optional<std::string>&& val);

	void setKeyLength(Poco::Optional<int> val);

	void setKeyUsage(const Poco::SharedPtr<CertificateUsage>& val);

	void setKeyUsage(Poco::SharedPtr<CertificateUsage>&& val);

	void setSerialNum(const Poco::Optional<std::string>& val);

	void setSerialNum(Poco::Optional<std::string>&& val);

	void setSignatureAlgorithm(const Poco::Optional<std::string>& val);

	void setSignatureAlgorithm(Poco::Optional<std::string>&& val);

	void setSubjectDN(const Poco::Optional<std::string>& val);

	void setSubjectDN(Poco::Optional<std::string>&& val);

	void setValidity(const Poco::SharedPtr<DateTimeRange>& val);

	void setValidity(Poco::SharedPtr<DateTimeRange>&& val);

	void setVersion(const Poco::Optional<std::string>& val);

	void setVersion(Poco::Optional<std::string>&& val);

private:
	//@ name=CertificateID
	//@ order=0
	std::string _certificateID;

	//@ mandatory=false
	//@ name=IssuerDN
	//@ order=1
	Poco::Optional<std::string> _issuerDN;

	//@ mandatory=false
	//@ name=SubjectDN
	//@ order=2
	Poco::Optional<std::string> _subjectDN;

	//@ mandatory=false
	//@ name=KeyUsage
	//@ order=3
	Poco::SharedPtr<CertificateUsage> _keyUsage;

	//@ mandatory=false
	//@ name=ExtendedKeyUsage
	//@ order=4
	Poco::SharedPtr<CertificateUsage> _extendedKeyUsage;

	//@ mandatory=false
	//@ name=KeyLength
	//@ order=5
	Poco::Optional<int> _keyLength;

	//@ mandatory=false
	//@ name=Version
	//@ order=6
	Poco::Optional<std::string> _version;

	//@ mandatory=false
	//@ name=SerialNum
	//@ order=7
	Poco::Optional<std::string> _serialNum;

	//@ mandatory=false
	//@ name=SignatureAlgorithm
	//@ order=8
	Poco::Optional<std::string> _signatureAlgorithm;

	//@ mandatory=false
	//@ name=Validity
	//@ order=9
	Poco::SharedPtr<DateTimeRange> _validity;

	//@ mandatory=false
	//@ name=Extension
	//@ order=10
	Poco::SharedPtr<CertificateInformationExtension> _extension;

};


inline const std::string& CertificateInformation::getCertificateID() const
{
	return _certificateID;
}


inline const Poco::SharedPtr<CertificateUsage>& CertificateInformation::getExtendedKeyUsage() const
{
	return _extendedKeyUsage;
}


inline const Poco::SharedPtr<CertificateInformationExtension>& CertificateInformation::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& CertificateInformation::getIssuerDN() const
{
	return _issuerDN;
}


inline Poco::Optional<int> CertificateInformation::getKeyLength() const
{
	return _keyLength;
}


inline const Poco::SharedPtr<CertificateUsage>& CertificateInformation::getKeyUsage() const
{
	return _keyUsage;
}


inline const Poco::Optional<std::string>& CertificateInformation::getSerialNum() const
{
	return _serialNum;
}


inline const Poco::Optional<std::string>& CertificateInformation::getSignatureAlgorithm() const
{
	return _signatureAlgorithm;
}


inline const Poco::Optional<std::string>& CertificateInformation::getSubjectDN() const
{
	return _subjectDN;
}


inline const Poco::SharedPtr<DateTimeRange>& CertificateInformation::getValidity() const
{
	return _validity;
}


inline const Poco::Optional<std::string>& CertificateInformation::getVersion() const
{
	return _version;
}


inline void CertificateInformation::setCertificateID(const std::string& val)
{
	_certificateID = val;
}


inline void CertificateInformation::setCertificateID(std::string&& val)
{
	_certificateID = std::move(val);
}


inline void CertificateInformation::setExtendedKeyUsage(const Poco::SharedPtr<CertificateUsage>& val)
{
	_extendedKeyUsage = val;
}


inline void CertificateInformation::setExtendedKeyUsage(Poco::SharedPtr<CertificateUsage>&& val)
{
	_extendedKeyUsage = std::move(val);
}


inline void CertificateInformation::setExtension(const Poco::SharedPtr<CertificateInformationExtension>& val)
{
	_extension = val;
}


inline void CertificateInformation::setExtension(Poco::SharedPtr<CertificateInformationExtension>&& val)
{
	_extension = std::move(val);
}


inline void CertificateInformation::setIssuerDN(const Poco::Optional<std::string>& val)
{
	_issuerDN = val;
}


inline void CertificateInformation::setIssuerDN(Poco::Optional<std::string>&& val)
{
	_issuerDN = std::move(val);
}


inline void CertificateInformation::setKeyLength(Poco::Optional<int> val)
{
	_keyLength = val;
}


inline void CertificateInformation::setKeyUsage(const Poco::SharedPtr<CertificateUsage>& val)
{
	_keyUsage = val;
}


inline void CertificateInformation::setKeyUsage(Poco::SharedPtr<CertificateUsage>&& val)
{
	_keyUsage = std::move(val);
}


inline void CertificateInformation::setSerialNum(const Poco::Optional<std::string>& val)
{
	_serialNum = val;
}


inline void CertificateInformation::setSerialNum(Poco::Optional<std::string>&& val)
{
	_serialNum = std::move(val);
}


inline void CertificateInformation::setSignatureAlgorithm(const Poco::Optional<std::string>& val)
{
	_signatureAlgorithm = val;
}


inline void CertificateInformation::setSignatureAlgorithm(Poco::Optional<std::string>&& val)
{
	_signatureAlgorithm = std::move(val);
}


inline void CertificateInformation::setSubjectDN(const Poco::Optional<std::string>& val)
{
	_subjectDN = val;
}


inline void CertificateInformation::setSubjectDN(Poco::Optional<std::string>&& val)
{
	_subjectDN = std::move(val);
}


inline void CertificateInformation::setValidity(const Poco::SharedPtr<DateTimeRange>& val)
{
	_validity = val;
}


inline void CertificateInformation::setValidity(Poco::SharedPtr<DateTimeRange>&& val)
{
	_validity = std::move(val);
}


inline void CertificateInformation::setVersion(const Poco::Optional<std::string>& val)
{
	_version = val;
}


inline void CertificateInformation::setVersion(Poco::Optional<std::string>&& val)
{
	_version = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/CertificateInformationExtension.h"
#include "ONVIF/CertificateUsage.h"
#include "ONVIF/DateTimeRange.h"


#endif // ONVIF_CertificateInformation_INCLUDED
