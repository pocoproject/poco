// 
// GetVideoSourceModesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoSourceModesResponse_INCLUDED
#define ONVIF_Media_GetVideoSourceModesResponse_INCLUDED


#include "ONVIF/Media/VideoSourceMode.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoSourceModesResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoSourceModesResponse
{
public:
	GetVideoSourceModesResponse();

	GetVideoSourceModesResponse(const std::vector<VideoSourceMode>& videoSourceModes);

	virtual ~GetVideoSourceModesResponse();

	const std::vector<VideoSourceMode>& getVideoSourceModes() const;

	std::vector<VideoSourceMode>& getVideoSourceModes();

	void setVideoSourceModes(const std::vector<VideoSourceMode>& val);

	void setVideoSourceModes(std::vector<VideoSourceMode>&& val);

private:
	//@ name=VideoSourceModes
	//@ order=0
	std::vector<VideoSourceMode> _videoSourceModes;

};


inline const std::vector<VideoSourceMode>& GetVideoSourceModesResponse::getVideoSourceModes() const
{
	return _videoSourceModes;
}


inline std::vector<VideoSourceMode>& GetVideoSourceModesResponse::getVideoSourceModes()
{
	return _videoSourceModes;
}


inline void GetVideoSourceModesResponse::setVideoSourceModes(const std::vector<VideoSourceMode>& val)
{
	_videoSourceModes = val;
}


inline void GetVideoSourceModesResponse::setVideoSourceModes(std::vector<VideoSourceMode>&& val)
{
	_videoSourceModes = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoSourceModesResponse_INCLUDED
