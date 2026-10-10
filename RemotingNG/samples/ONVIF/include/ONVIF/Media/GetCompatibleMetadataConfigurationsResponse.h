// 
// GetCompatibleMetadataConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetCompatibleMetadataConfigurationsResponse_INCLUDED
#define ONVIF_Media_GetCompatibleMetadataConfigurationsResponse_INCLUDED


#include "ONVIF/MetadataConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetCompatibleMetadataConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetCompatibleMetadataConfigurationsResponse
{
public:
	GetCompatibleMetadataConfigurationsResponse();

	GetCompatibleMetadataConfigurationsResponse(const std::vector<ONVIF::MetadataConfiguration>& configurations);

	virtual ~GetCompatibleMetadataConfigurationsResponse();

	const std::vector<ONVIF::MetadataConfiguration>& getConfigurations() const;

	std::vector<ONVIF::MetadataConfiguration>& getConfigurations();

	void setConfigurations(const std::vector<ONVIF::MetadataConfiguration>& val);

	void setConfigurations(std::vector<ONVIF::MetadataConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=Configurations
	//@ order=0
	std::vector<ONVIF::MetadataConfiguration> _configurations;

};


inline const std::vector<ONVIF::MetadataConfiguration>& GetCompatibleMetadataConfigurationsResponse::getConfigurations() const
{
	return _configurations;
}


inline std::vector<ONVIF::MetadataConfiguration>& GetCompatibleMetadataConfigurationsResponse::getConfigurations()
{
	return _configurations;
}


inline void GetCompatibleMetadataConfigurationsResponse::setConfigurations(const std::vector<ONVIF::MetadataConfiguration>& val)
{
	_configurations = val;
}


inline void GetCompatibleMetadataConfigurationsResponse::setConfigurations(std::vector<ONVIF::MetadataConfiguration>&& val)
{
	_configurations = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetCompatibleMetadataConfigurationsResponse_INCLUDED
