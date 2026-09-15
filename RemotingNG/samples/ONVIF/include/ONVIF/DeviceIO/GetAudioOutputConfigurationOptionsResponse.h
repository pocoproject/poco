// 
// GetAudioOutputConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetAudioOutputConfigurationOptionsResponse_INCLUDED
#define ONVIF_DeviceIO_GetAudioOutputConfigurationOptionsResponse_INCLUDED


#include "ONVIF/AudioOutputConfigurationOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetAudioOutputConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetAudioOutputConfigurationOptionsResponse
{
public:
	GetAudioOutputConfigurationOptionsResponse();

	GetAudioOutputConfigurationOptionsResponse(const ONVIF::AudioOutputConfigurationOptions& audioOutputOptions);

	virtual ~GetAudioOutputConfigurationOptionsResponse();

	const ONVIF::AudioOutputConfigurationOptions& getAudioOutputOptions() const;

	void setAudioOutputOptions(const ONVIF::AudioOutputConfigurationOptions& val);

	void setAudioOutputOptions(ONVIF::AudioOutputConfigurationOptions&& val);

private:
	//@ name=AudioOutputOptions
	//@ order=0
	ONVIF::AudioOutputConfigurationOptions _audioOutputOptions;

};


inline const ONVIF::AudioOutputConfigurationOptions& GetAudioOutputConfigurationOptionsResponse::getAudioOutputOptions() const
{
	return _audioOutputOptions;
}


inline void GetAudioOutputConfigurationOptionsResponse::setAudioOutputOptions(const ONVIF::AudioOutputConfigurationOptions& val)
{
	_audioOutputOptions = val;
}


inline void GetAudioOutputConfigurationOptionsResponse::setAudioOutputOptions(ONVIF::AudioOutputConfigurationOptions&& val)
{
	_audioOutputOptions = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetAudioOutputConfigurationOptionsResponse_INCLUDED
