// 
// Certificate.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Certificate_INCLUDED
#define ONVIF_Certificate_INCLUDED


#include "ONVIF/BinaryData.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Certificate
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Certificate
{
public:
	Certificate();

	Certificate(
		const std::string& certificateID, 
		const BinaryData& certificate);

	virtual ~Certificate();

	const BinaryData& getCertificate() const;

	const std::string& getCertificateID() const;

	void setCertificate(const BinaryData& val);

	void setCertificate(BinaryData&& val);

	void setCertificateID(const std::string& val);

	void setCertificateID(std::string&& val);

private:
	//@ name=CertificateID
	//@ order=0
	std::string _certificateID;

	//@ name=Certificate
	//@ order=1
	BinaryData _certificate;

};


inline const BinaryData& Certificate::getCertificate() const
{
	return _certificate;
}


inline const std::string& Certificate::getCertificateID() const
{
	return _certificateID;
}


inline void Certificate::setCertificate(const BinaryData& val)
{
	_certificate = val;
}


inline void Certificate::setCertificate(BinaryData&& val)
{
	_certificate = std::move(val);
}


inline void Certificate::setCertificateID(const std::string& val)
{
	_certificateID = val;
}


inline void Certificate::setCertificateID(std::string&& val)
{
	_certificateID = std::move(val);
}


} // ONVIF


#endif // ONVIF_Certificate_INCLUDED
