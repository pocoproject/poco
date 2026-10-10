// 
// DeviceServiceCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_DeviceServiceCapabilities_INCLUDED
#define ONVIF_Device_DeviceServiceCapabilities_INCLUDED


#include "ONVIF/Device/NetworkCapabilities.h"
#include "ONVIF/Device/SecurityCapabilities.h"
#include "ONVIF/Device/SystemCapabilities.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {
class MiscCapabilities;
} } 


namespace ONVIF {
namespace Device {


//@ name=DeviceServiceCapabilities
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API DeviceServiceCapabilities
{
public:
	DeviceServiceCapabilities();

	DeviceServiceCapabilities(
		const NetworkCapabilities& network, 
		const SecurityCapabilities& security, 
		const SystemCapabilities& system, 
		const Poco::SharedPtr<MiscCapabilities>& misc);

	virtual ~DeviceServiceCapabilities();

	const Poco::SharedPtr<MiscCapabilities>& getMisc() const;

	const NetworkCapabilities& getNetwork() const;

	const SecurityCapabilities& getSecurity() const;

	const SystemCapabilities& getSystem() const;

	void setMisc(const Poco::SharedPtr<MiscCapabilities>& val);

	void setMisc(Poco::SharedPtr<MiscCapabilities>&& val);

	void setNetwork(const NetworkCapabilities& val);

	void setNetwork(NetworkCapabilities&& val);

	void setSecurity(const SecurityCapabilities& val);

	void setSecurity(SecurityCapabilities&& val);

	void setSystem(const SystemCapabilities& val);

	void setSystem(SystemCapabilities&& val);

private:
	//@ name=Network
	//@ order=0
	NetworkCapabilities _network;

	//@ name=Security
	//@ order=1
	SecurityCapabilities _security;

	//@ name=System
	//@ order=2
	SystemCapabilities _system;

	//@ mandatory=false
	//@ name=Misc
	//@ order=3
	Poco::SharedPtr<MiscCapabilities> _misc;

};


inline const Poco::SharedPtr<MiscCapabilities>& DeviceServiceCapabilities::getMisc() const
{
	return _misc;
}


inline const NetworkCapabilities& DeviceServiceCapabilities::getNetwork() const
{
	return _network;
}


inline const SecurityCapabilities& DeviceServiceCapabilities::getSecurity() const
{
	return _security;
}


inline const SystemCapabilities& DeviceServiceCapabilities::getSystem() const
{
	return _system;
}


inline void DeviceServiceCapabilities::setMisc(const Poco::SharedPtr<MiscCapabilities>& val)
{
	_misc = val;
}


inline void DeviceServiceCapabilities::setMisc(Poco::SharedPtr<MiscCapabilities>&& val)
{
	_misc = std::move(val);
}


inline void DeviceServiceCapabilities::setNetwork(const NetworkCapabilities& val)
{
	_network = val;
}


inline void DeviceServiceCapabilities::setNetwork(NetworkCapabilities&& val)
{
	_network = std::move(val);
}


inline void DeviceServiceCapabilities::setSecurity(const SecurityCapabilities& val)
{
	_security = val;
}


inline void DeviceServiceCapabilities::setSecurity(SecurityCapabilities&& val)
{
	_security = std::move(val);
}


inline void DeviceServiceCapabilities::setSystem(const SystemCapabilities& val)
{
	_system = val;
}


inline void DeviceServiceCapabilities::setSystem(SystemCapabilities&& val)
{
	_system = std::move(val);
}


} } // ONVIF::Device


// The following headers are required for template instantiation.
#include "ONVIF/Device/MiscCapabilities.h"


#endif // ONVIF_Device_DeviceServiceCapabilities_INCLUDED
