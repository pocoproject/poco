// 
// DynamicDNSInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DynamicDNSInformation_INCLUDED
#define ONVIF_DynamicDNSInformation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class DynamicDNSInformationExtension;
} 


namespace ONVIF {


//@ name=DynamicDNSInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DynamicDNSInformation
{
public:
	DynamicDNSInformation();

	DynamicDNSInformation(
		const std::string& type, 
		const Poco::Optional<std::string>& name, 
		const Poco::Optional<std::string>& tTL, 
		const Poco::SharedPtr<DynamicDNSInformationExtension>& extension);

	virtual ~DynamicDNSInformation();

	const Poco::SharedPtr<DynamicDNSInformationExtension>& getExtension() const;

	const Poco::Optional<std::string>& getName() const;

	const Poco::Optional<std::string>& getTTL() const;

	const std::string& getType() const;

	void setExtension(const Poco::SharedPtr<DynamicDNSInformationExtension>& val);

	void setExtension(Poco::SharedPtr<DynamicDNSInformationExtension>&& val);

	void setName(const Poco::Optional<std::string>& val);

	void setName(Poco::Optional<std::string>&& val);

	void setTTL(const Poco::Optional<std::string>& val);

	void setTTL(Poco::Optional<std::string>&& val);

	void setType(const std::string& val);

	void setType(std::string&& val);

private:
	//@ name=Type
	//@ order=0
	std::string _type;

	//@ mandatory=false
	//@ name=Name
	//@ order=1
	Poco::Optional<std::string> _name;

	//@ mandatory=false
	//@ name=TTL
	//@ order=2
	Poco::Optional<std::string> _tTL;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<DynamicDNSInformationExtension> _extension;

};


inline const Poco::SharedPtr<DynamicDNSInformationExtension>& DynamicDNSInformation::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& DynamicDNSInformation::getName() const
{
	return _name;
}


inline const Poco::Optional<std::string>& DynamicDNSInformation::getTTL() const
{
	return _tTL;
}


inline const std::string& DynamicDNSInformation::getType() const
{
	return _type;
}


inline void DynamicDNSInformation::setExtension(const Poco::SharedPtr<DynamicDNSInformationExtension>& val)
{
	_extension = val;
}


inline void DynamicDNSInformation::setExtension(Poco::SharedPtr<DynamicDNSInformationExtension>&& val)
{
	_extension = std::move(val);
}


inline void DynamicDNSInformation::setName(const Poco::Optional<std::string>& val)
{
	_name = val;
}


inline void DynamicDNSInformation::setName(Poco::Optional<std::string>&& val)
{
	_name = std::move(val);
}


inline void DynamicDNSInformation::setTTL(const Poco::Optional<std::string>& val)
{
	_tTL = val;
}


inline void DynamicDNSInformation::setTTL(Poco::Optional<std::string>&& val)
{
	_tTL = std::move(val);
}


inline void DynamicDNSInformation::setType(const std::string& val)
{
	_type = val;
}


inline void DynamicDNSInformation::setType(std::string&& val)
{
	_type = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/DynamicDNSInformationExtension.h"


#endif // ONVIF_DynamicDNSInformation_INCLUDED
