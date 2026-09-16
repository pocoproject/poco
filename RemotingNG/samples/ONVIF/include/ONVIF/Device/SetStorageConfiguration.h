// 
// SetStorageConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetStorageConfiguration_INCLUDED
#define ONVIF_Device_SetStorageConfiguration_INCLUDED


#include "ONVIF/Device/StorageConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetStorageConfiguration"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetStorageConfiguration
{
public:
	SetStorageConfiguration();

	SetStorageConfiguration(const StorageConfiguration& storageConfiguration);

	virtual ~SetStorageConfiguration();

	const StorageConfiguration& getStorageConfiguration() const;

	void setStorageConfiguration(const StorageConfiguration& val);

	void setStorageConfiguration(StorageConfiguration&& val);

private:
	//@ name=StorageConfiguration
	//@ order=0
	StorageConfiguration _storageConfiguration;

};


inline const StorageConfiguration& SetStorageConfiguration::getStorageConfiguration() const
{
	return _storageConfiguration;
}


inline void SetStorageConfiguration::setStorageConfiguration(const StorageConfiguration& val)
{
	_storageConfiguration = val;
}


inline void SetStorageConfiguration::setStorageConfiguration(StorageConfiguration&& val)
{
	_storageConfiguration = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetStorageConfiguration_INCLUDED
