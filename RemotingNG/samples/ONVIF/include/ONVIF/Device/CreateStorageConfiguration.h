// 
// CreateStorageConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_CreateStorageConfiguration_INCLUDED
#define ONVIF_Device_CreateStorageConfiguration_INCLUDED


#include "ONVIF/Device/StorageConfigurationData.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#CreateStorageConfiguration"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API CreateStorageConfiguration
{
public:
	CreateStorageConfiguration();

	CreateStorageConfiguration(const StorageConfigurationData& storageConfiguration);

	virtual ~CreateStorageConfiguration();

	const StorageConfigurationData& getStorageConfiguration() const;

	void setStorageConfiguration(const StorageConfigurationData& val);

	void setStorageConfiguration(StorageConfigurationData&& val);

private:
	//@ name=StorageConfiguration
	//@ order=0
	StorageConfigurationData _storageConfiguration;

};


inline const StorageConfigurationData& CreateStorageConfiguration::getStorageConfiguration() const
{
	return _storageConfiguration;
}


inline void CreateStorageConfiguration::setStorageConfiguration(const StorageConfigurationData& val)
{
	_storageConfiguration = val;
}


inline void CreateStorageConfiguration::setStorageConfiguration(StorageConfigurationData&& val)
{
	_storageConfiguration = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_CreateStorageConfiguration_INCLUDED
