// 
// GetAudioSourceConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetAudioSourceConfigurationOptionsResponse_INCLUDED
#define ONVIF_DeviceIO_GetAudioSourceConfigurationOptionsResponse_INCLUDED


#include "ONVIF/AudioSourceConfigurationOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetAudioSourceConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetAudioSourceConfigurationOptionsResponse
{
public:
	GetAudioSourceConfigurationOptionsResponse();

	GetAudioSourceConfigurationOptionsResponse(const ONVIF::AudioSourceConfigurationOptions& audioSourceOptions);

	virtual ~GetAudioSourceConfigurationOptionsResponse();

	const ONVIF::AudioSourceConfigurationOptions& getAudioSourceOptions() const;

	void setAudioSourceOptions(const ONVIF::AudioSourceConfigurationOptions& val);

	void setAudioSourceOptions(ONVIF::AudioSourceConfigurationOptions&& val);

private:
	//@ name=AudioSourceOptions
	//@ order=0
	ONVIF::AudioSourceConfigurationOptions _audioSourceOptions;

};


inline const ONVIF::AudioSourceConfigurationOptions& GetAudioSourceConfigurationOptionsResponse::getAudioSourceOptions() const
{
	return _audioSourceOptions;
}


inline void GetAudioSourceConfigurationOptionsResponse::setAudioSourceOptions(const ONVIF::AudioSourceConfigurationOptions& val)
{
	_audioSourceOptions = val;
}


inline void GetAudioSourceConfigurationOptionsResponse::setAudioSourceOptions(ONVIF::AudioSourceConfigurationOptions&& val)
{
	_audioSourceOptions = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetAudioSourceConfigurationOptionsResponse_INCLUDED
