// 
// GetAudioSourceConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetAudioSourceConfigurationResponse_INCLUDED
#define ONVIF_DeviceIO_GetAudioSourceConfigurationResponse_INCLUDED


#include "ONVIF/AudioSourceConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetAudioSourceConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetAudioSourceConfigurationResponse
{
public:
	GetAudioSourceConfigurationResponse();

	GetAudioSourceConfigurationResponse(const ONVIF::AudioSourceConfiguration& audioSourceConfiguration);

	virtual ~GetAudioSourceConfigurationResponse();

	const ONVIF::AudioSourceConfiguration& getAudioSourceConfiguration() const;

	void setAudioSourceConfiguration(const ONVIF::AudioSourceConfiguration& val);

	void setAudioSourceConfiguration(ONVIF::AudioSourceConfiguration&& val);

private:
	//@ name=AudioSourceConfiguration
	//@ order=0
	ONVIF::AudioSourceConfiguration _audioSourceConfiguration;

};


inline const ONVIF::AudioSourceConfiguration& GetAudioSourceConfigurationResponse::getAudioSourceConfiguration() const
{
	return _audioSourceConfiguration;
}


inline void GetAudioSourceConfigurationResponse::setAudioSourceConfiguration(const ONVIF::AudioSourceConfiguration& val)
{
	_audioSourceConfiguration = val;
}


inline void GetAudioSourceConfigurationResponse::setAudioSourceConfiguration(ONVIF::AudioSourceConfiguration&& val)
{
	_audioSourceConfiguration = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetAudioSourceConfigurationResponse_INCLUDED
