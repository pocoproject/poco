// 
// GetVideoSourceConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetVideoSourceConfigurationOptions_INCLUDED
#define ONVIF_DeviceIO_GetVideoSourceConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetVideoSourceConfigurationOptions"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetVideoSourceConfigurationOptions
{
public:
	GetVideoSourceConfigurationOptions();

	GetVideoSourceConfigurationOptions(const std::string& videoSourceToken);

	virtual ~GetVideoSourceConfigurationOptions();

	const std::string& getVideoSourceToken() const;

	void setVideoSourceToken(const std::string& val);

	void setVideoSourceToken(std::string&& val);

private:
	//@ name=VideoSourceToken
	//@ order=0
	std::string _videoSourceToken;

};


inline const std::string& GetVideoSourceConfigurationOptions::getVideoSourceToken() const
{
	return _videoSourceToken;
}


inline void GetVideoSourceConfigurationOptions::setVideoSourceToken(const std::string& val)
{
	_videoSourceToken = val;
}


inline void GetVideoSourceConfigurationOptions::setVideoSourceToken(std::string&& val)
{
	_videoSourceToken = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetVideoSourceConfigurationOptions_INCLUDED
