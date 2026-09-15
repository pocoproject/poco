// 
// GetMetadataConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetMetadataConfigurationsResponse_INCLUDED
#define ONVIF_Media_GetMetadataConfigurationsResponse_INCLUDED


#include "ONVIF/MetadataConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetMetadataConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetMetadataConfigurationsResponse
{
public:
	GetMetadataConfigurationsResponse();

	GetMetadataConfigurationsResponse(const std::vector<ONVIF::MetadataConfiguration>& configurations);

	virtual ~GetMetadataConfigurationsResponse();

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


inline const std::vector<ONVIF::MetadataConfiguration>& GetMetadataConfigurationsResponse::getConfigurations() const
{
	return _configurations;
}


inline std::vector<ONVIF::MetadataConfiguration>& GetMetadataConfigurationsResponse::getConfigurations()
{
	return _configurations;
}


inline void GetMetadataConfigurationsResponse::setConfigurations(const std::vector<ONVIF::MetadataConfiguration>& val)
{
	_configurations = val;
}


inline void GetMetadataConfigurationsResponse::setConfigurations(std::vector<ONVIF::MetadataConfiguration>&& val)
{
	_configurations = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetMetadataConfigurationsResponse_INCLUDED
