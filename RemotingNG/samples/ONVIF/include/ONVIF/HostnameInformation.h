// 
// HostnameInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_HostnameInformation_INCLUDED
#define ONVIF_HostnameInformation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class HostnameInformationExtension;
} 


namespace ONVIF {


//@ name=HostnameInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API HostnameInformation
{
public:
	HostnameInformation();

	HostnameInformation(
		bool fromDHCP, 
		const Poco::Optional<std::string>& name, 
		const Poco::SharedPtr<HostnameInformationExtension>& extension);

	virtual ~HostnameInformation();

	const Poco::SharedPtr<HostnameInformationExtension>& getExtension() const;

	bool getFromDHCP() const;

	const Poco::Optional<std::string>& getName() const;

	void setExtension(const Poco::SharedPtr<HostnameInformationExtension>& val);

	void setExtension(Poco::SharedPtr<HostnameInformationExtension>&& val);

	void setFromDHCP(bool val);

	void setName(const Poco::Optional<std::string>& val);

	void setName(Poco::Optional<std::string>&& val);

private:
	//@ name=FromDHCP
	//@ order=0
	bool _fromDHCP;

	//@ mandatory=false
	//@ name=Name
	//@ order=1
	Poco::Optional<std::string> _name;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<HostnameInformationExtension> _extension;

};


inline const Poco::SharedPtr<HostnameInformationExtension>& HostnameInformation::getExtension() const
{
	return _extension;
}


inline bool HostnameInformation::getFromDHCP() const
{
	return _fromDHCP;
}


inline const Poco::Optional<std::string>& HostnameInformation::getName() const
{
	return _name;
}


inline void HostnameInformation::setExtension(const Poco::SharedPtr<HostnameInformationExtension>& val)
{
	_extension = val;
}


inline void HostnameInformation::setExtension(Poco::SharedPtr<HostnameInformationExtension>&& val)
{
	_extension = std::move(val);
}


inline void HostnameInformation::setFromDHCP(bool val)
{
	_fromDHCP = val;
}


inline void HostnameInformation::setName(const Poco::Optional<std::string>& val)
{
	_name = val;
}


inline void HostnameInformation::setName(Poco::Optional<std::string>&& val)
{
	_name = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/HostnameInformationExtension.h"


#endif // ONVIF_HostnameInformation_INCLUDED
