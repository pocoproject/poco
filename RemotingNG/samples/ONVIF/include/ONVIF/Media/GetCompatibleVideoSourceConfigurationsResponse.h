// 
// GetCompatibleVideoSourceConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetCompatibleVideoSourceConfigurationsResponse_INCLUDED
#define ONVIF_Media_GetCompatibleVideoSourceConfigurationsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoSourceConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetCompatibleVideoSourceConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetCompatibleVideoSourceConfigurationsResponse
{
public:
	GetCompatibleVideoSourceConfigurationsResponse();

	GetCompatibleVideoSourceConfigurationsResponse(const std::vector<ONVIF::VideoSourceConfiguration>& configurations);

	virtual ~GetCompatibleVideoSourceConfigurationsResponse();

	const std::vector<ONVIF::VideoSourceConfiguration>& getConfigurations() const;

	std::vector<ONVIF::VideoSourceConfiguration>& getConfigurations();

	void setConfigurations(const std::vector<ONVIF::VideoSourceConfiguration>& val);

	void setConfigurations(std::vector<ONVIF::VideoSourceConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=Configurations
	//@ order=0
	std::vector<ONVIF::VideoSourceConfiguration> _configurations;

};


inline const std::vector<ONVIF::VideoSourceConfiguration>& GetCompatibleVideoSourceConfigurationsResponse::getConfigurations() const
{
	return _configurations;
}


inline std::vector<ONVIF::VideoSourceConfiguration>& GetCompatibleVideoSourceConfigurationsResponse::getConfigurations()
{
	return _configurations;
}


inline void GetCompatibleVideoSourceConfigurationsResponse::setConfigurations(const std::vector<ONVIF::VideoSourceConfiguration>& val)
{
	_configurations = val;
}


inline void GetCompatibleVideoSourceConfigurationsResponse::setConfigurations(std::vector<ONVIF::VideoSourceConfiguration>&& val)
{
	_configurations = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetCompatibleVideoSourceConfigurationsResponse_INCLUDED
