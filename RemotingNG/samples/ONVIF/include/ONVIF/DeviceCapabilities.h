// 
// DeviceCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceCapabilities_INCLUDED
#define ONVIF_DeviceCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
class DeviceCapabilitiesExtension;
class IOCapabilities;
class NetworkCapabilities;
class SecurityCapabilities;
class SystemCapabilities;
} 


namespace ONVIF {


//@ name=DeviceCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DeviceCapabilities
{
public:
	DeviceCapabilities();

	DeviceCapabilities(
		const Poco::URI& xAddr, 
		const Poco::SharedPtr<NetworkCapabilities>& network, 
		const Poco::SharedPtr<SystemCapabilities>& system, 
		const Poco::SharedPtr<IOCapabilities>& iO, 
		const Poco::SharedPtr<SecurityCapabilities>& security, 
		const Poco::SharedPtr<DeviceCapabilitiesExtension>& extension);

	virtual ~DeviceCapabilities();

	const Poco::SharedPtr<DeviceCapabilitiesExtension>& getExtension() const;

	const Poco::SharedPtr<IOCapabilities>& getIO() const;

	const Poco::SharedPtr<NetworkCapabilities>& getNetwork() const;

	const Poco::SharedPtr<SecurityCapabilities>& getSecurity() const;

	const Poco::SharedPtr<SystemCapabilities>& getSystem() const;

	const Poco::URI& getXAddr() const;

	void setExtension(const Poco::SharedPtr<DeviceCapabilitiesExtension>& val);

	void setExtension(Poco::SharedPtr<DeviceCapabilitiesExtension>&& val);

	void setIO(const Poco::SharedPtr<IOCapabilities>& val);

	void setIO(Poco::SharedPtr<IOCapabilities>&& val);

	void setNetwork(const Poco::SharedPtr<NetworkCapabilities>& val);

	void setNetwork(Poco::SharedPtr<NetworkCapabilities>&& val);

	void setSecurity(const Poco::SharedPtr<SecurityCapabilities>& val);

	void setSecurity(Poco::SharedPtr<SecurityCapabilities>&& val);

	void setSystem(const Poco::SharedPtr<SystemCapabilities>& val);

	void setSystem(Poco::SharedPtr<SystemCapabilities>&& val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

	//@ mandatory=false
	//@ name=Network
	//@ order=1
	Poco::SharedPtr<NetworkCapabilities> _network;

	//@ mandatory=false
	//@ name=System
	//@ order=2
	Poco::SharedPtr<SystemCapabilities> _system;

	//@ mandatory=false
	//@ name=IO
	//@ order=3
	Poco::SharedPtr<IOCapabilities> _iO;

	//@ mandatory=false
	//@ name=Security
	//@ order=4
	Poco::SharedPtr<SecurityCapabilities> _security;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<DeviceCapabilitiesExtension> _extension;

};


inline const Poco::SharedPtr<DeviceCapabilitiesExtension>& DeviceCapabilities::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<IOCapabilities>& DeviceCapabilities::getIO() const
{
	return _iO;
}


inline const Poco::SharedPtr<NetworkCapabilities>& DeviceCapabilities::getNetwork() const
{
	return _network;
}


inline const Poco::SharedPtr<SecurityCapabilities>& DeviceCapabilities::getSecurity() const
{
	return _security;
}


inline const Poco::SharedPtr<SystemCapabilities>& DeviceCapabilities::getSystem() const
{
	return _system;
}


inline const Poco::URI& DeviceCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void DeviceCapabilities::setExtension(const Poco::SharedPtr<DeviceCapabilitiesExtension>& val)
{
	_extension = val;
}


inline void DeviceCapabilities::setExtension(Poco::SharedPtr<DeviceCapabilitiesExtension>&& val)
{
	_extension = std::move(val);
}


inline void DeviceCapabilities::setIO(const Poco::SharedPtr<IOCapabilities>& val)
{
	_iO = val;
}


inline void DeviceCapabilities::setIO(Poco::SharedPtr<IOCapabilities>&& val)
{
	_iO = std::move(val);
}


inline void DeviceCapabilities::setNetwork(const Poco::SharedPtr<NetworkCapabilities>& val)
{
	_network = val;
}


inline void DeviceCapabilities::setNetwork(Poco::SharedPtr<NetworkCapabilities>&& val)
{
	_network = std::move(val);
}


inline void DeviceCapabilities::setSecurity(const Poco::SharedPtr<SecurityCapabilities>& val)
{
	_security = val;
}


inline void DeviceCapabilities::setSecurity(Poco::SharedPtr<SecurityCapabilities>&& val)
{
	_security = std::move(val);
}


inline void DeviceCapabilities::setSystem(const Poco::SharedPtr<SystemCapabilities>& val)
{
	_system = val;
}


inline void DeviceCapabilities::setSystem(Poco::SharedPtr<SystemCapabilities>&& val)
{
	_system = std::move(val);
}


inline void DeviceCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void DeviceCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/DeviceCapabilitiesExtension.h"
#include "ONVIF/IOCapabilities.h"
#include "ONVIF/NetworkCapabilities.h"
#include "ONVIF/SecurityCapabilities.h"
#include "ONVIF/SystemCapabilities.h"


#endif // ONVIF_DeviceCapabilities_INCLUDED
