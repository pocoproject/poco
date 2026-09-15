// 
// GetVideoOutputConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetVideoOutputConfigurationOptionsResponse_INCLUDED
#define ONVIF_DeviceIO_GetVideoOutputConfigurationOptionsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoOutputConfigurationOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetVideoOutputConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetVideoOutputConfigurationOptionsResponse
{
public:
	GetVideoOutputConfigurationOptionsResponse();

	GetVideoOutputConfigurationOptionsResponse(const ONVIF::VideoOutputConfigurationOptions& videoOutputConfigurationOptions);

	virtual ~GetVideoOutputConfigurationOptionsResponse();

	const ONVIF::VideoOutputConfigurationOptions& getVideoOutputConfigurationOptions() const;

	void setVideoOutputConfigurationOptions(const ONVIF::VideoOutputConfigurationOptions& val);

	void setVideoOutputConfigurationOptions(ONVIF::VideoOutputConfigurationOptions&& val);

private:
	//@ name=VideoOutputConfigurationOptions
	//@ order=0
	ONVIF::VideoOutputConfigurationOptions _videoOutputConfigurationOptions;

};


inline const ONVIF::VideoOutputConfigurationOptions& GetVideoOutputConfigurationOptionsResponse::getVideoOutputConfigurationOptions() const
{
	return _videoOutputConfigurationOptions;
}


inline void GetVideoOutputConfigurationOptionsResponse::setVideoOutputConfigurationOptions(const ONVIF::VideoOutputConfigurationOptions& val)
{
	_videoOutputConfigurationOptions = val;
}


inline void GetVideoOutputConfigurationOptionsResponse::setVideoOutputConfigurationOptions(ONVIF::VideoOutputConfigurationOptions&& val)
{
	_videoOutputConfigurationOptions = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetVideoOutputConfigurationOptionsResponse_INCLUDED
