// 
// GetStorageConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetStorageConfigurationsResponse_INCLUDED
#define ONVIF_Device_GetStorageConfigurationsResponse_INCLUDED


#include "ONVIF/Device/StorageConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetStorageConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetStorageConfigurationsResponse
{
public:
	GetStorageConfigurationsResponse();

	GetStorageConfigurationsResponse(const std::vector<StorageConfiguration>& storageConfigurations);

	virtual ~GetStorageConfigurationsResponse();

	const std::vector<StorageConfiguration>& getStorageConfigurations() const;

	std::vector<StorageConfiguration>& getStorageConfigurations();

	void setStorageConfigurations(const std::vector<StorageConfiguration>& val);

	void setStorageConfigurations(std::vector<StorageConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=StorageConfigurations
	//@ order=0
	std::vector<StorageConfiguration> _storageConfigurations;

};


inline const std::vector<StorageConfiguration>& GetStorageConfigurationsResponse::getStorageConfigurations() const
{
	return _storageConfigurations;
}


inline std::vector<StorageConfiguration>& GetStorageConfigurationsResponse::getStorageConfigurations()
{
	return _storageConfigurations;
}


inline void GetStorageConfigurationsResponse::setStorageConfigurations(const std::vector<StorageConfiguration>& val)
{
	_storageConfigurations = val;
}


inline void GetStorageConfigurationsResponse::setStorageConfigurations(std::vector<StorageConfiguration>&& val)
{
	_storageConfigurations = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetStorageConfigurationsResponse_INCLUDED
