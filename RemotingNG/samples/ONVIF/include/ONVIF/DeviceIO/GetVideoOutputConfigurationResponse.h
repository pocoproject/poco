// 
// GetVideoOutputConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetVideoOutputConfigurationResponse_INCLUDED
#define ONVIF_DeviceIO_GetVideoOutputConfigurationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoOutputConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetVideoOutputConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetVideoOutputConfigurationResponse
{
public:
	GetVideoOutputConfigurationResponse();

	GetVideoOutputConfigurationResponse(const ONVIF::VideoOutputConfiguration& videoOutputConfiguration);

	virtual ~GetVideoOutputConfigurationResponse();

	const ONVIF::VideoOutputConfiguration& getVideoOutputConfiguration() const;

	void setVideoOutputConfiguration(const ONVIF::VideoOutputConfiguration& val);

	void setVideoOutputConfiguration(ONVIF::VideoOutputConfiguration&& val);

private:
	//@ name=VideoOutputConfiguration
	//@ order=0
	ONVIF::VideoOutputConfiguration _videoOutputConfiguration;

};


inline const ONVIF::VideoOutputConfiguration& GetVideoOutputConfigurationResponse::getVideoOutputConfiguration() const
{
	return _videoOutputConfiguration;
}


inline void GetVideoOutputConfigurationResponse::setVideoOutputConfiguration(const ONVIF::VideoOutputConfiguration& val)
{
	_videoOutputConfiguration = val;
}


inline void GetVideoOutputConfigurationResponse::setVideoOutputConfiguration(ONVIF::VideoOutputConfiguration&& val)
{
	_videoOutputConfiguration = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetVideoOutputConfigurationResponse_INCLUDED
