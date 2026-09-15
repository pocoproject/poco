// 
// GetVideoAnalyticsConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoAnalyticsConfigurationsResponse_INCLUDED
#define ONVIF_Media_GetVideoAnalyticsConfigurationsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoAnalyticsConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoAnalyticsConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoAnalyticsConfigurationsResponse
{
public:
	GetVideoAnalyticsConfigurationsResponse();

	GetVideoAnalyticsConfigurationsResponse(const std::vector<ONVIF::VideoAnalyticsConfiguration>& configurations);

	virtual ~GetVideoAnalyticsConfigurationsResponse();

	const std::vector<ONVIF::VideoAnalyticsConfiguration>& getConfigurations() const;

	std::vector<ONVIF::VideoAnalyticsConfiguration>& getConfigurations();

	void setConfigurations(const std::vector<ONVIF::VideoAnalyticsConfiguration>& val);

	void setConfigurations(std::vector<ONVIF::VideoAnalyticsConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=Configurations
	//@ order=0
	std::vector<ONVIF::VideoAnalyticsConfiguration> _configurations;

};


inline const std::vector<ONVIF::VideoAnalyticsConfiguration>& GetVideoAnalyticsConfigurationsResponse::getConfigurations() const
{
	return _configurations;
}


inline std::vector<ONVIF::VideoAnalyticsConfiguration>& GetVideoAnalyticsConfigurationsResponse::getConfigurations()
{
	return _configurations;
}


inline void GetVideoAnalyticsConfigurationsResponse::setConfigurations(const std::vector<ONVIF::VideoAnalyticsConfiguration>& val)
{
	_configurations = val;
}


inline void GetVideoAnalyticsConfigurationsResponse::setConfigurations(std::vector<ONVIF::VideoAnalyticsConfiguration>&& val)
{
	_configurations = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoAnalyticsConfigurationsResponse_INCLUDED
