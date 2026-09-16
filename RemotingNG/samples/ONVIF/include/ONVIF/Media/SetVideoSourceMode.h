// 
// SetVideoSourceMode.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_SetVideoSourceMode_INCLUDED
#define ONVIF_Media_SetVideoSourceMode_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#SetVideoSourceMode"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API SetVideoSourceMode
{
public:
	SetVideoSourceMode();

	SetVideoSourceMode(
		const std::string& videoSourceToken, 
		const std::string& videoSourceModeToken);

	virtual ~SetVideoSourceMode();

	const std::string& getVideoSourceModeToken() const;

	const std::string& getVideoSourceToken() const;

	void setVideoSourceModeToken(const std::string& val);

	void setVideoSourceModeToken(std::string&& val);

	void setVideoSourceToken(const std::string& val);

	void setVideoSourceToken(std::string&& val);

private:
	//@ name=VideoSourceToken
	//@ order=0
	std::string _videoSourceToken;

	//@ name=VideoSourceModeToken
	//@ order=1
	std::string _videoSourceModeToken;

};


inline const std::string& SetVideoSourceMode::getVideoSourceModeToken() const
{
	return _videoSourceModeToken;
}


inline const std::string& SetVideoSourceMode::getVideoSourceToken() const
{
	return _videoSourceToken;
}


inline void SetVideoSourceMode::setVideoSourceModeToken(const std::string& val)
{
	_videoSourceModeToken = val;
}


inline void SetVideoSourceMode::setVideoSourceModeToken(std::string&& val)
{
	_videoSourceModeToken = std::move(val);
}


inline void SetVideoSourceMode::setVideoSourceToken(const std::string& val)
{
	_videoSourceToken = val;
}


inline void SetVideoSourceMode::setVideoSourceToken(std::string&& val)
{
	_videoSourceToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_SetVideoSourceMode_INCLUDED
