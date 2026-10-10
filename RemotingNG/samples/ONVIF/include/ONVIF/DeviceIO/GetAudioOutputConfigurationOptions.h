// 
// GetAudioOutputConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetAudioOutputConfigurationOptions_INCLUDED
#define ONVIF_DeviceIO_GetAudioOutputConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetAudioOutputConfigurationOptions"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetAudioOutputConfigurationOptions
{
public:
	GetAudioOutputConfigurationOptions();

	GetAudioOutputConfigurationOptions(const std::string& audioOutputToken);

	virtual ~GetAudioOutputConfigurationOptions();

	const std::string& getAudioOutputToken() const;

	void setAudioOutputToken(const std::string& val);

	void setAudioOutputToken(std::string&& val);

private:
	//@ name=AudioOutputToken
	//@ order=0
	std::string _audioOutputToken;

};


inline const std::string& GetAudioOutputConfigurationOptions::getAudioOutputToken() const
{
	return _audioOutputToken;
}


inline void GetAudioOutputConfigurationOptions::setAudioOutputToken(const std::string& val)
{
	_audioOutputToken = val;
}


inline void GetAudioOutputConfigurationOptions::setAudioOutputToken(std::string&& val)
{
	_audioOutputToken = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetAudioOutputConfigurationOptions_INCLUDED
