// 
// GetVideoSourcesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoSourcesResponse_INCLUDED
#define ONVIF_Media_GetVideoSourcesResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoSource.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoSourcesResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoSourcesResponse
{
public:
	GetVideoSourcesResponse();

	GetVideoSourcesResponse(const std::vector<ONVIF::VideoSource>& videoSources);

	virtual ~GetVideoSourcesResponse();

	const std::vector<ONVIF::VideoSource>& getVideoSources() const;

	std::vector<ONVIF::VideoSource>& getVideoSources();

	void setVideoSources(const std::vector<ONVIF::VideoSource>& val);

	void setVideoSources(std::vector<ONVIF::VideoSource>&& val);

private:
	//@ mandatory=false
	//@ name=VideoSources
	//@ order=0
	std::vector<ONVIF::VideoSource> _videoSources;

};


inline const std::vector<ONVIF::VideoSource>& GetVideoSourcesResponse::getVideoSources() const
{
	return _videoSources;
}


inline std::vector<ONVIF::VideoSource>& GetVideoSourcesResponse::getVideoSources()
{
	return _videoSources;
}


inline void GetVideoSourcesResponse::setVideoSources(const std::vector<ONVIF::VideoSource>& val)
{
	_videoSources = val;
}


inline void GetVideoSourcesResponse::setVideoSources(std::vector<ONVIF::VideoSource>&& val)
{
	_videoSources = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoSourcesResponse_INCLUDED
