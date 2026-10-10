// 
// Service.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_Service_INCLUDED
#define ONVIF_Device_Service_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/OnvifVersion.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {
class Capabilities;
} } 


namespace ONVIF {
namespace Device {


//@ name=Service
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API Service
{
public:
	Service();

	Service(
		const Poco::URI& namespace_, 
		const Poco::URI& xAddr, 
		const Poco::SharedPtr<Capabilities>& capabilities, 
		const ONVIF::OnvifVersion& version);

	virtual ~Service();

	const Poco::SharedPtr<Capabilities>& getCapabilities() const;

	const Poco::URI& getNamespace() const;

	const ONVIF::OnvifVersion& getVersion() const;

	const Poco::URI& getXAddr() const;

	void setCapabilities(const Poco::SharedPtr<Capabilities>& val);

	void setCapabilities(Poco::SharedPtr<Capabilities>&& val);

	void setNamespace(const Poco::URI& val);

	void setNamespace(Poco::URI&& val);

	void setVersion(const ONVIF::OnvifVersion& val);

	void setVersion(ONVIF::OnvifVersion&& val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=Namespace
	//@ order=0
	Poco::URI _namespace;

	//@ name=XAddr
	//@ order=1
	Poco::URI _xAddr;

	//@ mandatory=false
	//@ name=Capabilities
	//@ order=2
	Poco::SharedPtr<Capabilities> _capabilities;

	//@ name=Version
	//@ order=3
	ONVIF::OnvifVersion _version;

};


inline const Poco::SharedPtr<Capabilities>& Service::getCapabilities() const
{
	return _capabilities;
}


inline const Poco::URI& Service::getNamespace() const
{
	return _namespace;
}


inline const ONVIF::OnvifVersion& Service::getVersion() const
{
	return _version;
}


inline const Poco::URI& Service::getXAddr() const
{
	return _xAddr;
}


inline void Service::setCapabilities(const Poco::SharedPtr<Capabilities>& val)
{
	_capabilities = val;
}


inline void Service::setCapabilities(Poco::SharedPtr<Capabilities>&& val)
{
	_capabilities = std::move(val);
}


inline void Service::setNamespace(const Poco::URI& val)
{
	_namespace = val;
}


inline void Service::setNamespace(Poco::URI&& val)
{
	_namespace = std::move(val);
}


inline void Service::setVersion(const ONVIF::OnvifVersion& val)
{
	_version = val;
}


inline void Service::setVersion(ONVIF::OnvifVersion&& val)
{
	_version = std::move(val);
}


inline void Service::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void Service::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} } // ONVIF::Device


// The following headers are required for template instantiation.
#include "ONVIF/Device/Capabilities.h"


#endif // ONVIF_Device_Service_INCLUDED
