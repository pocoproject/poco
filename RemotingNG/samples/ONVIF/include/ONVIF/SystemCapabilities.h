// 
// SystemCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SystemCapabilities_INCLUDED
#define ONVIF_SystemCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/OnvifVersion.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class SystemCapabilitiesExtension;
} 


namespace ONVIF {


//@ name=SystemCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SystemCapabilities
{
public:
	SystemCapabilities();

	SystemCapabilities(
		bool discoveryResolve, 
		bool discoveryBye, 
		bool remoteDiscovery, 
		bool systemBackup, 
		bool systemLogging, 
		bool firmwareUpgrade, 
		const std::vector<OnvifVersion>& supportedVersions, 
		const Poco::SharedPtr<SystemCapabilitiesExtension>& extension);

	virtual ~SystemCapabilities();

	bool getDiscoveryBye() const;

	bool getDiscoveryResolve() const;

	const Poco::SharedPtr<SystemCapabilitiesExtension>& getExtension() const;

	bool getFirmwareUpgrade() const;

	bool getRemoteDiscovery() const;

	const std::vector<OnvifVersion>& getSupportedVersions() const;

	std::vector<OnvifVersion>& getSupportedVersions();

	bool getSystemBackup() const;

	bool getSystemLogging() const;

	void setDiscoveryBye(bool val);

	void setDiscoveryResolve(bool val);

	void setExtension(const Poco::SharedPtr<SystemCapabilitiesExtension>& val);

	void setExtension(Poco::SharedPtr<SystemCapabilitiesExtension>&& val);

	void setFirmwareUpgrade(bool val);

	void setRemoteDiscovery(bool val);

	void setSupportedVersions(const std::vector<OnvifVersion>& val);

	void setSupportedVersions(std::vector<OnvifVersion>&& val);

	void setSystemBackup(bool val);

	void setSystemLogging(bool val);

private:
	//@ name=DiscoveryResolve
	//@ order=0
	bool _discoveryResolve;

	//@ name=DiscoveryBye
	//@ order=1
	bool _discoveryBye;

	//@ name=RemoteDiscovery
	//@ order=2
	bool _remoteDiscovery;

	//@ name=SystemBackup
	//@ order=3
	bool _systemBackup;

	//@ name=SystemLogging
	//@ order=4
	bool _systemLogging;

	//@ name=FirmwareUpgrade
	//@ order=5
	bool _firmwareUpgrade;

	//@ name=SupportedVersions
	//@ order=6
	std::vector<OnvifVersion> _supportedVersions;

	//@ mandatory=false
	//@ name=Extension
	//@ order=7
	Poco::SharedPtr<SystemCapabilitiesExtension> _extension;

};


inline bool SystemCapabilities::getDiscoveryBye() const
{
	return _discoveryBye;
}


inline bool SystemCapabilities::getDiscoveryResolve() const
{
	return _discoveryResolve;
}


inline const Poco::SharedPtr<SystemCapabilitiesExtension>& SystemCapabilities::getExtension() const
{
	return _extension;
}


inline bool SystemCapabilities::getFirmwareUpgrade() const
{
	return _firmwareUpgrade;
}


inline bool SystemCapabilities::getRemoteDiscovery() const
{
	return _remoteDiscovery;
}


inline const std::vector<OnvifVersion>& SystemCapabilities::getSupportedVersions() const
{
	return _supportedVersions;
}


inline std::vector<OnvifVersion>& SystemCapabilities::getSupportedVersions()
{
	return _supportedVersions;
}


inline bool SystemCapabilities::getSystemBackup() const
{
	return _systemBackup;
}


inline bool SystemCapabilities::getSystemLogging() const
{
	return _systemLogging;
}


inline void SystemCapabilities::setDiscoveryBye(bool val)
{
	_discoveryBye = val;
}


inline void SystemCapabilities::setDiscoveryResolve(bool val)
{
	_discoveryResolve = val;
}


inline void SystemCapabilities::setExtension(const Poco::SharedPtr<SystemCapabilitiesExtension>& val)
{
	_extension = val;
}


inline void SystemCapabilities::setExtension(Poco::SharedPtr<SystemCapabilitiesExtension>&& val)
{
	_extension = std::move(val);
}


inline void SystemCapabilities::setFirmwareUpgrade(bool val)
{
	_firmwareUpgrade = val;
}


inline void SystemCapabilities::setRemoteDiscovery(bool val)
{
	_remoteDiscovery = val;
}


inline void SystemCapabilities::setSupportedVersions(const std::vector<OnvifVersion>& val)
{
	_supportedVersions = val;
}


inline void SystemCapabilities::setSupportedVersions(std::vector<OnvifVersion>&& val)
{
	_supportedVersions = std::move(val);
}


inline void SystemCapabilities::setSystemBackup(bool val)
{
	_systemBackup = val;
}


inline void SystemCapabilities::setSystemLogging(bool val)
{
	_systemLogging = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/SystemCapabilitiesExtension.h"


#endif // ONVIF_SystemCapabilities_INCLUDED
