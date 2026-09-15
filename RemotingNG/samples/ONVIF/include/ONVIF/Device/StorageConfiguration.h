// 
// StorageConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_StorageConfiguration_INCLUDED
#define ONVIF_Device_StorageConfiguration_INCLUDED


#include "ONVIF/Device/StorageConfigurationData.h"
#include "ONVIF/DeviceEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name=StorageConfiguration
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API StorageConfiguration: public ONVIF::DeviceEntity
{
public:
	StorageConfiguration();

	StorageConfiguration(
		const std::string& token, 
		const StorageConfigurationData& data);

	virtual ~StorageConfiguration();

	const StorageConfigurationData& getData() const;

	void setData(const StorageConfigurationData& val);

	void setData(StorageConfigurationData&& val);

private:
	//@ name=Data
	//@ order=0
	StorageConfigurationData _data;

};


inline const StorageConfigurationData& StorageConfiguration::getData() const
{
	return _data;
}


inline void StorageConfiguration::setData(const StorageConfigurationData& val)
{
	_data = val;
}


inline void StorageConfiguration::setData(StorageConfigurationData&& val)
{
	_data = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_StorageConfiguration_INCLUDED
