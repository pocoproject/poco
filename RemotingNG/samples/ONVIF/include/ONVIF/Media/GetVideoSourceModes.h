// 
// GetVideoSourceModes.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoSourceModes_INCLUDED
#define ONVIF_Media_GetVideoSourceModes_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoSourceModes"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoSourceModes
{
public:
	GetVideoSourceModes();

	GetVideoSourceModes(const std::string& videoSourceToken);

	virtual ~GetVideoSourceModes();

	const std::string& getVideoSourceToken() const;

	void setVideoSourceToken(const std::string& val);

	void setVideoSourceToken(std::string&& val);

private:
	//@ name=VideoSourceToken
	//@ order=0
	std::string _videoSourceToken;

};


inline const std::string& GetVideoSourceModes::getVideoSourceToken() const
{
	return _videoSourceToken;
}


inline void GetVideoSourceModes::setVideoSourceToken(const std::string& val)
{
	_videoSourceToken = val;
}


inline void GetVideoSourceModes::setVideoSourceToken(std::string&& val)
{
	_videoSourceToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoSourceModes_INCLUDED
