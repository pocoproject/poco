// 
// GetAudioOutputConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetAudioOutputConfigurationResponse_INCLUDED
#define ONVIF_DeviceIO_GetAudioOutputConfigurationResponse_INCLUDED


#include "ONVIF/AudioOutputConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetAudioOutputConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetAudioOutputConfigurationResponse
{
public:
	GetAudioOutputConfigurationResponse();

	GetAudioOutputConfigurationResponse(const ONVIF::AudioOutputConfiguration& audioOutputConfiguration);

	virtual ~GetAudioOutputConfigurationResponse();

	const ONVIF::AudioOutputConfiguration& getAudioOutputConfiguration() const;

	void setAudioOutputConfiguration(const ONVIF::AudioOutputConfiguration& val);

	void setAudioOutputConfiguration(ONVIF::AudioOutputConfiguration&& val);

private:
	//@ name=AudioOutputConfiguration
	//@ order=0
	ONVIF::AudioOutputConfiguration _audioOutputConfiguration;

};


inline const ONVIF::AudioOutputConfiguration& GetAudioOutputConfigurationResponse::getAudioOutputConfiguration() const
{
	return _audioOutputConfiguration;
}


inline void GetAudioOutputConfigurationResponse::setAudioOutputConfiguration(const ONVIF::AudioOutputConfiguration& val)
{
	_audioOutputConfiguration = val;
}


inline void GetAudioOutputConfigurationResponse::setAudioOutputConfiguration(ONVIF::AudioOutputConfiguration&& val)
{
	_audioOutputConfiguration = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetAudioOutputConfigurationResponse_INCLUDED
