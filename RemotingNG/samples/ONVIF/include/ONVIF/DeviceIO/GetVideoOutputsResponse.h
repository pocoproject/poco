// 
// GetVideoOutputsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetVideoOutputsResponse_INCLUDED
#define ONVIF_DeviceIO_GetVideoOutputsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoOutput.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetVideoOutputsResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetVideoOutputsResponse
{
public:
	GetVideoOutputsResponse();

	GetVideoOutputsResponse(const std::vector<ONVIF::VideoOutput>& videoOutputs);

	virtual ~GetVideoOutputsResponse();

	const std::vector<ONVIF::VideoOutput>& getVideoOutputs() const;

	std::vector<ONVIF::VideoOutput>& getVideoOutputs();

	void setVideoOutputs(const std::vector<ONVIF::VideoOutput>& val);

	void setVideoOutputs(std::vector<ONVIF::VideoOutput>&& val);

private:
	//@ mandatory=false
	//@ name=VideoOutputs
	//@ order=0
	std::vector<ONVIF::VideoOutput> _videoOutputs;

};


inline const std::vector<ONVIF::VideoOutput>& GetVideoOutputsResponse::getVideoOutputs() const
{
	return _videoOutputs;
}


inline std::vector<ONVIF::VideoOutput>& GetVideoOutputsResponse::getVideoOutputs()
{
	return _videoOutputs;
}


inline void GetVideoOutputsResponse::setVideoOutputs(const std::vector<ONVIF::VideoOutput>& val)
{
	_videoOutputs = val;
}


inline void GetVideoOutputsResponse::setVideoOutputs(std::vector<ONVIF::VideoOutput>&& val)
{
	_videoOutputs = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetVideoOutputsResponse_INCLUDED
