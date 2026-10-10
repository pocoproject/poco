// 
// GetVideoSourceConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetVideoSourceConfiguration_INCLUDED
#define ONVIF_DeviceIO_GetVideoSourceConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetVideoSourceConfiguration"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetVideoSourceConfiguration
{
public:
	GetVideoSourceConfiguration();

	GetVideoSourceConfiguration(const std::string& videoSourceToken);

	virtual ~GetVideoSourceConfiguration();

	const std::string& getVideoSourceToken() const;

	void setVideoSourceToken(const std::string& val);

	void setVideoSourceToken(std::string&& val);

private:
	//@ name=VideoSourceToken
	//@ order=0
	std::string _videoSourceToken;

};


inline const std::string& GetVideoSourceConfiguration::getVideoSourceToken() const
{
	return _videoSourceToken;
}


inline void GetVideoSourceConfiguration::setVideoSourceToken(const std::string& val)
{
	_videoSourceToken = val;
}


inline void GetVideoSourceConfiguration::setVideoSourceToken(std::string&& val)
{
	_videoSourceToken = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetVideoSourceConfiguration_INCLUDED
