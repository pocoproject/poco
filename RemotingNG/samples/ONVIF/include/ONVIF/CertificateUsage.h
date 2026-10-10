// 
// CertificateUsage.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_CertificateUsage_INCLUDED
#define ONVIF_CertificateUsage_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=CertificateUsage
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API CertificateUsage
{
public:
	CertificateUsage();

	CertificateUsage(const Poco::Optional<std::string>& value);

	virtual ~CertificateUsage();

	const Poco::Optional<std::string>& getValue() const;

	void setValue(const Poco::Optional<std::string>& val);

	void setValue(Poco::Optional<std::string>&& val);

private:
	//@ inline=true
	//@ mandatory=false
	//@ name=value
	//@ order=0
	Poco::Optional<std::string> _value;

};


inline const Poco::Optional<std::string>& CertificateUsage::getValue() const
{
	return _value;
}


inline void CertificateUsage::setValue(const Poco::Optional<std::string>& val)
{
	_value = val;
}


inline void CertificateUsage::setValue(Poco::Optional<std::string>&& val)
{
	_value = std::move(val);
}


} // ONVIF


#endif // ONVIF_CertificateUsage_INCLUDED
