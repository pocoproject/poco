// 
// GetVideoSourceConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetVideoSourceConfigurationResponse_INCLUDED
#define ONVIF_DeviceIO_GetVideoSourceConfigurationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoSourceConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetVideoSourceConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetVideoSourceConfigurationResponse
{
public:
	GetVideoSourceConfigurationResponse();

	GetVideoSourceConfigurationResponse(const ONVIF::VideoSourceConfiguration& videoSourceConfiguration);

	virtual ~GetVideoSourceConfigurationResponse();

	const ONVIF::VideoSourceConfiguration& getVideoSourceConfiguration() const;

	void setVideoSourceConfiguration(const ONVIF::VideoSourceConfiguration& val);

	void setVideoSourceConfiguration(ONVIF::VideoSourceConfiguration&& val);

private:
	//@ name=VideoSourceConfiguration
	//@ order=0
	ONVIF::VideoSourceConfiguration _videoSourceConfiguration;

};


inline const ONVIF::VideoSourceConfiguration& GetVideoSourceConfigurationResponse::getVideoSourceConfiguration() const
{
	return _videoSourceConfiguration;
}


inline void GetVideoSourceConfigurationResponse::setVideoSourceConfiguration(const ONVIF::VideoSourceConfiguration& val)
{
	_videoSourceConfiguration = val;
}


inline void GetVideoSourceConfigurationResponse::setVideoSourceConfiguration(ONVIF::VideoSourceConfiguration&& val)
{
	_videoSourceConfiguration = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetVideoSourceConfigurationResponse_INCLUDED
