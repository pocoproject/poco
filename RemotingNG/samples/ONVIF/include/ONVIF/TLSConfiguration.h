// 
// TLSConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_TLSConfiguration_INCLUDED
#define ONVIF_TLSConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=TLSConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API TLSConfiguration
{
public:
	TLSConfiguration();

	TLSConfiguration(const std::string& certificateID);

	virtual ~TLSConfiguration();

	const std::string& getCertificateID() const;

	void setCertificateID(const std::string& val);

	void setCertificateID(std::string&& val);

private:
	//@ name=CertificateID
	//@ order=0
	std::string _certificateID;

};


inline const std::string& TLSConfiguration::getCertificateID() const
{
	return _certificateID;
}


inline void TLSConfiguration::setCertificateID(const std::string& val)
{
	_certificateID = val;
}


inline void TLSConfiguration::setCertificateID(std::string&& val)
{
	_certificateID = std::move(val);
}


} // ONVIF


#endif // ONVIF_TLSConfiguration_INCLUDED
