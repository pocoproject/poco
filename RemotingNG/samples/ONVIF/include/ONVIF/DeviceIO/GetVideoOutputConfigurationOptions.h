// 
// GetVideoOutputConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetVideoOutputConfigurationOptions_INCLUDED
#define ONVIF_DeviceIO_GetVideoOutputConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetVideoOutputConfigurationOptions"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetVideoOutputConfigurationOptions
{
public:
	GetVideoOutputConfigurationOptions();

	GetVideoOutputConfigurationOptions(const std::string& videoOutputToken);

	virtual ~GetVideoOutputConfigurationOptions();

	const std::string& getVideoOutputToken() const;

	void setVideoOutputToken(const std::string& val);

	void setVideoOutputToken(std::string&& val);

private:
	//@ name=VideoOutputToken
	//@ order=0
	std::string _videoOutputToken;

};


inline const std::string& GetVideoOutputConfigurationOptions::getVideoOutputToken() const
{
	return _videoOutputToken;
}


inline void GetVideoOutputConfigurationOptions::setVideoOutputToken(const std::string& val)
{
	_videoOutputToken = val;
}


inline void GetVideoOutputConfigurationOptions::setVideoOutputToken(std::string&& val)
{
	_videoOutputToken = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetVideoOutputConfigurationOptions_INCLUDED
