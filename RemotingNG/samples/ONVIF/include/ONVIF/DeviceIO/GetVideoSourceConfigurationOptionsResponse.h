// 
// GetVideoSourceConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetVideoSourceConfigurationOptionsResponse_INCLUDED
#define ONVIF_DeviceIO_GetVideoSourceConfigurationOptionsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoSourceConfigurationOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetVideoSourceConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetVideoSourceConfigurationOptionsResponse
{
public:
	GetVideoSourceConfigurationOptionsResponse();

	GetVideoSourceConfigurationOptionsResponse(const ONVIF::VideoSourceConfigurationOptions& videoSourceConfigurationOptions);

	virtual ~GetVideoSourceConfigurationOptionsResponse();

	const ONVIF::VideoSourceConfigurationOptions& getVideoSourceConfigurationOptions() const;

	void setVideoSourceConfigurationOptions(const ONVIF::VideoSourceConfigurationOptions& val);

	void setVideoSourceConfigurationOptions(ONVIF::VideoSourceConfigurationOptions&& val);

private:
	//@ name=VideoSourceConfigurationOptions
	//@ order=0
	ONVIF::VideoSourceConfigurationOptions _videoSourceConfigurationOptions;

};


inline const ONVIF::VideoSourceConfigurationOptions& GetVideoSourceConfigurationOptionsResponse::getVideoSourceConfigurationOptions() const
{
	return _videoSourceConfigurationOptions;
}


inline void GetVideoSourceConfigurationOptionsResponse::setVideoSourceConfigurationOptions(const ONVIF::VideoSourceConfigurationOptions& val)
{
	_videoSourceConfigurationOptions = val;
}


inline void GetVideoSourceConfigurationOptionsResponse::setVideoSourceConfigurationOptions(ONVIF::VideoSourceConfigurationOptions&& val)
{
	_videoSourceConfigurationOptions = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetVideoSourceConfigurationOptionsResponse_INCLUDED
