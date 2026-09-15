// 
// GetVideoAnalyticsConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoAnalyticsConfigurationResponse_INCLUDED
#define ONVIF_Media_GetVideoAnalyticsConfigurationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoAnalyticsConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoAnalyticsConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoAnalyticsConfigurationResponse
{
public:
	GetVideoAnalyticsConfigurationResponse();

	GetVideoAnalyticsConfigurationResponse(const ONVIF::VideoAnalyticsConfiguration& configuration);

	virtual ~GetVideoAnalyticsConfigurationResponse();

	const ONVIF::VideoAnalyticsConfiguration& getConfiguration() const;

	void setConfiguration(const ONVIF::VideoAnalyticsConfiguration& val);

	void setConfiguration(ONVIF::VideoAnalyticsConfiguration&& val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::VideoAnalyticsConfiguration _configuration;

};


inline const ONVIF::VideoAnalyticsConfiguration& GetVideoAnalyticsConfigurationResponse::getConfiguration() const
{
	return _configuration;
}


inline void GetVideoAnalyticsConfigurationResponse::setConfiguration(const ONVIF::VideoAnalyticsConfiguration& val)
{
	_configuration = val;
}


inline void GetVideoAnalyticsConfigurationResponse::setConfiguration(ONVIF::VideoAnalyticsConfiguration&& val)
{
	_configuration = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoAnalyticsConfigurationResponse_INCLUDED
