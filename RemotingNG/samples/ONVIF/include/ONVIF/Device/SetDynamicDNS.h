// 
// SetDynamicDNS.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetDynamicDNS_INCLUDED
#define ONVIF_Device_SetDynamicDNS_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetDynamicDNS"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetDynamicDNS
{
public:
	SetDynamicDNS();

	SetDynamicDNS(
		const std::string& type, 
		const Poco::Optional<std::string>& name, 
		const Poco::Optional<std::string>& tTL);

	virtual ~SetDynamicDNS();

	const Poco::Optional<std::string>& getName() const;

	const Poco::Optional<std::string>& getTTL() const;

	const std::string& getType() const;

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

};


inline const Poco::Optional<std::string>& SetDynamicDNS::getName() const
{
	return _name;
}


inline const Poco::Optional<std::string>& SetDynamicDNS::getTTL() const
{
	return _tTL;
}


inline const std::string& SetDynamicDNS::getType() const
{
	return _type;
}


inline void SetDynamicDNS::setName(const Poco::Optional<std::string>& val)
{
	_name = val;
}


inline void SetDynamicDNS::setName(Poco::Optional<std::string>&& val)
{
	_name = std::move(val);
}


inline void SetDynamicDNS::setTTL(const Poco::Optional<std::string>& val)
{
	_tTL = val;
}


inline void SetDynamicDNS::setTTL(Poco::Optional<std::string>&& val)
{
	_tTL = std::move(val);
}


inline void SetDynamicDNS::setType(const std::string& val)
{
	_type = val;
}


inline void SetDynamicDNS::setType(std::string&& val)
{
	_type = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetDynamicDNS_INCLUDED
