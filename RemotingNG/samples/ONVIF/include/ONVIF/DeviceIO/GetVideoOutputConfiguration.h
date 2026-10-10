// 
// GetVideoOutputConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetVideoOutputConfiguration_INCLUDED
#define ONVIF_DeviceIO_GetVideoOutputConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetVideoOutputConfiguration"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetVideoOutputConfiguration
{
public:
	GetVideoOutputConfiguration();

	GetVideoOutputConfiguration(const std::string& videoOutputToken);

	virtual ~GetVideoOutputConfiguration();

	const std::string& getVideoOutputToken() const;

	void setVideoOutputToken(const std::string& val);

	void setVideoOutputToken(std::string&& val);

private:
	//@ name=VideoOutputToken
	//@ order=0
	std::string _videoOutputToken;

};


inline const std::string& GetVideoOutputConfiguration::getVideoOutputToken() const
{
	return _videoOutputToken;
}


inline void GetVideoOutputConfiguration::setVideoOutputToken(const std::string& val)
{
	_videoOutputToken = val;
}


inline void GetVideoOutputConfiguration::setVideoOutputToken(std::string&& val)
{
	_videoOutputToken = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetVideoOutputConfiguration_INCLUDED
