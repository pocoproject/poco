// 
// CertificateGenerationParameters.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_CertificateGenerationParameters_INCLUDED
#define ONVIF_CertificateGenerationParameters_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class CertificateGenerationParametersExtension;
} 


namespace ONVIF {


//@ name=CertificateGenerationParameters
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API CertificateGenerationParameters
{
public:
	CertificateGenerationParameters();

	CertificateGenerationParameters(
		const Poco::Optional<std::string>& certificateID, 
		const Poco::Optional<std::string>& subject, 
		const Poco::Optional<std::string>& validNotBefore, 
		const Poco::Optional<std::string>& validNotAfter, 
		const Poco::SharedPtr<CertificateGenerationParametersExtension>& extension);

	virtual ~CertificateGenerationParameters();

	const Poco::Optional<std::string>& getCertificateID() const;

	const Poco::SharedPtr<CertificateGenerationParametersExtension>& getExtension() const;

	const Poco::Optional<std::string>& getSubject() const;

	const Poco::Optional<std::string>& getValidNotAfter() const;

	const Poco::Optional<std::string>& getValidNotBefore() const;

	void setCertificateID(const Poco::Optional<std::string>& val);

	void setCertificateID(Poco::Optional<std::string>&& val);

	void setExtension(const Poco::SharedPtr<CertificateGenerationParametersExtension>& val);

	void setExtension(Poco::SharedPtr<CertificateGenerationParametersExtension>&& val);

	void setSubject(const Poco::Optional<std::string>& val);

	void setSubject(Poco::Optional<std::string>&& val);

	void setValidNotAfter(const Poco::Optional<std::string>& val);

	void setValidNotAfter(Poco::Optional<std::string>&& val);

	void setValidNotBefore(const Poco::Optional<std::string>& val);

	void setValidNotBefore(Poco::Optional<std::string>&& val);

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
	Poco::Optional<std::string> _validNotBefore;

	//@ mandatory=false
	//@ name=ValidNotAfter
	//@ order=3
	Poco::Optional<std::string> _validNotAfter;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<CertificateGenerationParametersExtension> _extension;

};


inline const Poco::Optional<std::string>& CertificateGenerationParameters::getCertificateID() const
{
	return _certificateID;
}


inline const Poco::SharedPtr<CertificateGenerationParametersExtension>& CertificateGenerationParameters::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& CertificateGenerationParameters::getSubject() const
{
	return _subject;
}


inline const Poco::Optional<std::string>& CertificateGenerationParameters::getValidNotAfter() const
{
	return _validNotAfter;
}


inline const Poco::Optional<std::string>& CertificateGenerationParameters::getValidNotBefore() const
{
	return _validNotBefore;
}


inline void CertificateGenerationParameters::setCertificateID(const Poco::Optional<std::string>& val)
{
	_certificateID = val;
}


inline void CertificateGenerationParameters::setCertificateID(Poco::Optional<std::string>&& val)
{
	_certificateID = std::move(val);
}


inline void CertificateGenerationParameters::setExtension(const Poco::SharedPtr<CertificateGenerationParametersExtension>& val)
{
	_extension = val;
}


inline void CertificateGenerationParameters::setExtension(Poco::SharedPtr<CertificateGenerationParametersExtension>&& val)
{
	_extension = std::move(val);
}


inline void CertificateGenerationParameters::setSubject(const Poco::Optional<std::string>& val)
{
	_subject = val;
}


inline void CertificateGenerationParameters::setSubject(Poco::Optional<std::string>&& val)
{
	_subject = std::move(val);
}


inline void CertificateGenerationParameters::setValidNotAfter(const Poco::Optional<std::string>& val)
{
	_validNotAfter = val;
}


inline void CertificateGenerationParameters::setValidNotAfter(Poco::Optional<std::string>&& val)
{
	_validNotAfter = std::move(val);
}


inline void CertificateGenerationParameters::setValidNotBefore(const Poco::Optional<std::string>& val)
{
	_validNotBefore = val;
}


inline void CertificateGenerationParameters::setValidNotBefore(Poco::Optional<std::string>&& val)
{
	_validNotBefore = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/CertificateGenerationParametersExtension.h"


#endif // ONVIF_CertificateGenerationParameters_INCLUDED
