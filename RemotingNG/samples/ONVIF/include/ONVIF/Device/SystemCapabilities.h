// 
// SystemCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SystemCapabilities_INCLUDED
#define ONVIF_Device_SystemCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name=SystemCapabilities
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SystemCapabilities
{
public:
	SystemCapabilities();

	SystemCapabilities(
		Poco::Optional<bool> discoveryResolve, 
		Poco::Optional<bool> discoveryBye, 
		Poco::Optional<bool> remoteDiscovery, 
		Poco::Optional<bool> systemBackup, 
		Poco::Optional<bool> systemLogging, 
		Poco::Optional<bool> firmwareUpgrade, 
		Poco::Optional<bool> httpFirmwareUpgrade, 
		Poco::Optional<bool> httpSystemBackup, 
		Poco::Optional<bool> httpSystemLogging, 
		Poco::Optional<bool> httpSupportInformation, 
		Poco::Optional<bool> storageConfiguration, 
		Poco::Optional<int> maxStorageConfigurations, 
		Poco::Optional<int> geoLocationEntries, 
		const std::vector<std::vector<std::string>>& autoGeo, 
		const std::vector<std::vector<std::string>>& storageTypesSupported, 
		Poco::Optional<bool> discoveryNotSupported, 
		Poco::Optional<bool> networkConfigNotSupported, 
		Poco::Optional<bool> userConfigNotSupported, 
		const std::vector<std::vector<std::string>>& addons);

	virtual ~SystemCapabilities();

	const std::vector<std::vector<std::string>>& getAddons() const;

	std::vector<std::vector<std::string>>& getAddons();

	const std::vector<std::vector<std::string>>& getAutoGeo() const;

	std::vector<std::vector<std::string>>& getAutoGeo();

	Poco::Optional<bool> getDiscoveryBye() const;

	Poco::Optional<bool> getDiscoveryNotSupported() const;

	Poco::Optional<bool> getDiscoveryResolve() const;

	Poco::Optional<bool> getFirmwareUpgrade() const;

	Poco::Optional<int> getGeoLocationEntries() const;

	Poco::Optional<bool> getHttpFirmwareUpgrade() const;

	Poco::Optional<bool> getHttpSupportInformation() const;

	Poco::Optional<bool> getHttpSystemBackup() const;

	Poco::Optional<bool> getHttpSystemLogging() const;

	Poco::Optional<int> getMaxStorageConfigurations() const;

	Poco::Optional<bool> getNetworkConfigNotSupported() const;

	Poco::Optional<bool> getRemoteDiscovery() const;

	Poco::Optional<bool> getStorageConfiguration() const;

	const std::vector<std::vector<std::string>>& getStorageTypesSupported() const;

	std::vector<std::vector<std::string>>& getStorageTypesSupported();

	Poco::Optional<bool> getSystemBackup() const;

	Poco::Optional<bool> getSystemLogging() const;

	Poco::Optional<bool> getUserConfigNotSupported() const;

	void setAddons(const std::vector<std::vector<std::string>>& val);

	void setAddons(std::vector<std::vector<std::string>>&& val);

	void setAutoGeo(const std::vector<std::vector<std::string>>& val);

	void setAutoGeo(std::vector<std::vector<std::string>>&& val);

	void setDiscoveryBye(Poco::Optional<bool> val);

	void setDiscoveryNotSupported(Poco::Optional<bool> val);

	void setDiscoveryResolve(Poco::Optional<bool> val);

	void setFirmwareUpgrade(Poco::Optional<bool> val);

	void setGeoLocationEntries(Poco::Optional<int> val);

	void setHttpFirmwareUpgrade(Poco::Optional<bool> val);

	void setHttpSupportInformation(Poco::Optional<bool> val);

	void setHttpSystemBackup(Poco::Optional<bool> val);

	void setHttpSystemLogging(Poco::Optional<bool> val);

	void setMaxStorageConfigurations(Poco::Optional<int> val);

	void setNetworkConfigNotSupported(Poco::Optional<bool> val);

	void setRemoteDiscovery(Poco::Optional<bool> val);

	void setStorageConfiguration(Poco::Optional<bool> val);

	void setStorageTypesSupported(const std::vector<std::vector<std::string>>& val);

	void setStorageTypesSupported(std::vector<std::vector<std::string>>&& val);

	void setSystemBackup(Poco::Optional<bool> val);

	void setSystemLogging(Poco::Optional<bool> val);

	void setUserConfigNotSupported(Poco::Optional<bool> val);

private:
	//@ mandatory=false
	//@ name=DiscoveryResolve
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _discoveryResolve;

	//@ mandatory=false
	//@ name=DiscoveryBye
	//@ order=1
	//@ type=attr
	Poco::Optional<bool> _discoveryBye;

	//@ mandatory=false
	//@ name=RemoteDiscovery
	//@ order=2
	//@ type=attr
	Poco::Optional<bool> _remoteDiscovery;

	//@ mandatory=false
	//@ name=SystemBackup
	//@ order=3
	//@ type=attr
	Poco::Optional<bool> _systemBackup;

	//@ mandatory=false
	//@ name=SystemLogging
	//@ order=4
	//@ type=attr
	Poco::Optional<bool> _systemLogging;

	//@ mandatory=false
	//@ name=FirmwareUpgrade
	//@ order=5
	//@ type=attr
	Poco::Optional<bool> _firmwareUpgrade;

	//@ mandatory=false
	//@ name=HttpFirmwareUpgrade
	//@ order=6
	//@ type=attr
	Poco::Optional<bool> _httpFirmwareUpgrade;

	//@ mandatory=false
	//@ name=HttpSystemBackup
	//@ order=7
	//@ type=attr
	Poco::Optional<bool> _httpSystemBackup;

	//@ mandatory=false
	//@ name=HttpSystemLogging
	//@ order=8
	//@ type=attr
	Poco::Optional<bool> _httpSystemLogging;

	//@ mandatory=false
	//@ name=HttpSupportInformation
	//@ order=9
	//@ type=attr
	Poco::Optional<bool> _httpSupportInformation;

	//@ mandatory=false
	//@ name=StorageConfiguration
	//@ order=10
	//@ type=attr
	Poco::Optional<bool> _storageConfiguration;

	//@ mandatory=false
	//@ name=MaxStorageConfigurations
	//@ order=11
	//@ type=attr
	Poco::Optional<int> _maxStorageConfigurations;

	//@ mandatory=false
	//@ name=GeoLocationEntries
	//@ order=12
	//@ type=attr
	Poco::Optional<int> _geoLocationEntries;

	//@ mandatory=false
	//@ name=AutoGeo
	//@ order=13
	//@ type=attr
	std::vector<std::vector<std::string>> _autoGeo;

	//@ mandatory=false
	//@ name=StorageTypesSupported
	//@ order=14
	//@ type=attr
	std::vector<std::vector<std::string>> _storageTypesSupported;

	//@ mandatory=false
	//@ name=DiscoveryNotSupported
	//@ order=15
	//@ type=attr
	Poco::Optional<bool> _discoveryNotSupported;

	//@ mandatory=false
	//@ name=NetworkConfigNotSupported
	//@ order=16
	//@ type=attr
	Poco::Optional<bool> _networkConfigNotSupported;

	//@ mandatory=false
	//@ name=UserConfigNotSupported
	//@ order=17
	//@ type=attr
	Poco::Optional<bool> _userConfigNotSupported;

	//@ mandatory=false
	//@ name=Addons
	//@ order=18
	//@ type=attr
	std::vector<std::vector<std::string>> _addons;

};


inline const std::vector<std::vector<std::string>>& SystemCapabilities::getAddons() const
{
	return _addons;
}


inline std::vector<std::vector<std::string>>& SystemCapabilities::getAddons()
{
	return _addons;
}


inline const std::vector<std::vector<std::string>>& SystemCapabilities::getAutoGeo() const
{
	return _autoGeo;
}


inline std::vector<std::vector<std::string>>& SystemCapabilities::getAutoGeo()
{
	return _autoGeo;
}


inline Poco::Optional<bool> SystemCapabilities::getDiscoveryBye() const
{
	return _discoveryBye;
}


inline Poco::Optional<bool> SystemCapabilities::getDiscoveryNotSupported() const
{
	return _discoveryNotSupported;
}


inline Poco::Optional<bool> SystemCapabilities::getDiscoveryResolve() const
{
	return _discoveryResolve;
}


inline Poco::Optional<bool> SystemCapabilities::getFirmwareUpgrade() const
{
	return _firmwareUpgrade;
}


inline Poco::Optional<int> SystemCapabilities::getGeoLocationEntries() const
{
	return _geoLocationEntries;
}


inline Poco::Optional<bool> SystemCapabilities::getHttpFirmwareUpgrade() const
{
	return _httpFirmwareUpgrade;
}


inline Poco::Optional<bool> SystemCapabilities::getHttpSupportInformation() const
{
	return _httpSupportInformation;
}


inline Poco::Optional<bool> SystemCapabilities::getHttpSystemBackup() const
{
	return _httpSystemBackup;
}


inline Poco::Optional<bool> SystemCapabilities::getHttpSystemLogging() const
{
	return _httpSystemLogging;
}


inline Poco::Optional<int> SystemCapabilities::getMaxStorageConfigurations() const
{
	return _maxStorageConfigurations;
}


inline Poco::Optional<bool> SystemCapabilities::getNetworkConfigNotSupported() const
{
	return _networkConfigNotSupported;
}


inline Poco::Optional<bool> SystemCapabilities::getRemoteDiscovery() const
{
	return _remoteDiscovery;
}


inline Poco::Optional<bool> SystemCapabilities::getStorageConfiguration() const
{
	return _storageConfiguration;
}


inline const std::vector<std::vector<std::string>>& SystemCapabilities::getStorageTypesSupported() const
{
	return _storageTypesSupported;
}


inline std::vector<std::vector<std::string>>& SystemCapabilities::getStorageTypesSupported()
{
	return _storageTypesSupported;
}


inline Poco::Optional<bool> SystemCapabilities::getSystemBackup() const
{
	return _systemBackup;
}


inline Poco::Optional<bool> SystemCapabilities::getSystemLogging() const
{
	return _systemLogging;
}


inline Poco::Optional<bool> SystemCapabilities::getUserConfigNotSupported() const
{
	return _userConfigNotSupported;
}


inline void SystemCapabilities::setAddons(const std::vector<std::vector<std::string>>& val)
{
	_addons = val;
}


inline void SystemCapabilities::setAddons(std::vector<std::vector<std::string>>&& val)
{
	_addons = std::move(val);
}


inline void SystemCapabilities::setAutoGeo(const std::vector<std::vector<std::string>>& val)
{
	_autoGeo = val;
}


inline void SystemCapabilities::setAutoGeo(std::vector<std::vector<std::string>>&& val)
{
	_autoGeo = std::move(val);
}


inline void SystemCapabilities::setDiscoveryBye(Poco::Optional<bool> val)
{
	_discoveryBye = val;
}


inline void SystemCapabilities::setDiscoveryNotSupported(Poco::Optional<bool> val)
{
	_discoveryNotSupported = val;
}


inline void SystemCapabilities::setDiscoveryResolve(Poco::Optional<bool> val)
{
	_discoveryResolve = val;
}


inline void SystemCapabilities::setFirmwareUpgrade(Poco::Optional<bool> val)
{
	_firmwareUpgrade = val;
}


inline void SystemCapabilities::setGeoLocationEntries(Poco::Optional<int> val)
{
	_geoLocationEntries = val;
}


inline void SystemCapabilities::setHttpFirmwareUpgrade(Poco::Optional<bool> val)
{
	_httpFirmwareUpgrade = val;
}


inline void SystemCapabilities::setHttpSupportInformation(Poco::Optional<bool> val)
{
	_httpSupportInformation = val;
}


inline void SystemCapabilities::setHttpSystemBackup(Poco::Optional<bool> val)
{
	_httpSystemBackup = val;
}


inline void SystemCapabilities::setHttpSystemLogging(Poco::Optional<bool> val)
{
	_httpSystemLogging = val;
}


inline void SystemCapabilities::setMaxStorageConfigurations(Poco::Optional<int> val)
{
	_maxStorageConfigurations = val;
}


inline void SystemCapabilities::setNetworkConfigNotSupported(Poco::Optional<bool> val)
{
	_networkConfigNotSupported = val;
}


inline void SystemCapabilities::setRemoteDiscovery(Poco::Optional<bool> val)
{
	_remoteDiscovery = val;
}


inline void SystemCapabilities::setStorageConfiguration(Poco::Optional<bool> val)
{
	_storageConfiguration = val;
}


inline void SystemCapabilities::setStorageTypesSupported(const std::vector<std::vector<std::string>>& val)
{
	_storageTypesSupported = val;
}


inline void SystemCapabilities::setStorageTypesSupported(std::vector<std::vector<std::string>>&& val)
{
	_storageTypesSupported = std::move(val);
}


inline void SystemCapabilities::setSystemBackup(Poco::Optional<bool> val)
{
	_systemBackup = val;
}


inline void SystemCapabilities::setSystemLogging(Poco::Optional<bool> val)
{
	_systemLogging = val;
}


inline void SystemCapabilities::setUserConfigNotSupported(Poco::Optional<bool> val)
{
	_userConfigNotSupported = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_SystemCapabilities_INCLUDED
