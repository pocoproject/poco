// 
// CertificateStatus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_CertificateStatus_INCLUDED
#define ONVIF_CertificateStatus_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=CertificateStatus
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API CertificateStatus
{
public:
	CertificateStatus();

	CertificateStatus(
		const std::string& certificateID, 
		bool status);

	virtual ~CertificateStatus();

	const std::string& getCertificateID() const;

	bool getStatus() const;

	void setCertificateID(const std::string& val);

	void setCertificateID(std::string&& val);

	void setStatus(bool val);

private:
	//@ name=CertificateID
	//@ order=0
	std::string _certificateID;

	//@ name=Status
	//@ order=1
	bool _status;

};


inline const std::string& CertificateStatus::getCertificateID() const
{
	return _certificateID;
}


inline bool CertificateStatus::getStatus() const
{
	return _status;
}


inline void CertificateStatus::setCertificateID(const std::string& val)
{
	_certificateID = val;
}


inline void CertificateStatus::setCertificateID(std::string&& val)
{
	_certificateID = std::move(val);
}


inline void CertificateStatus::setStatus(bool val)
{
	_status = val;
}


} // ONVIF


#endif // ONVIF_CertificateStatus_INCLUDED
