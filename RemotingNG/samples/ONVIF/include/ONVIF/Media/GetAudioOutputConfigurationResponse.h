// 
// GetAudioOutputConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioOutputConfigurationResponse_INCLUDED
#define ONVIF_Media_GetAudioOutputConfigurationResponse_INCLUDED


#include "ONVIF/AudioOutputConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioOutputConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioOutputConfigurationResponse
{
public:
	GetAudioOutputConfigurationResponse();

	GetAudioOutputConfigurationResponse(const ONVIF::AudioOutputConfiguration& configuration);

	virtual ~GetAudioOutputConfigurationResponse();

	const ONVIF::AudioOutputConfiguration& getConfiguration() const;

	void setConfiguration(const ONVIF::AudioOutputConfiguration& val);

	void setConfiguration(ONVIF::AudioOutputConfiguration&& val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::AudioOutputConfiguration _configuration;

};


inline const ONVIF::AudioOutputConfiguration& GetAudioOutputConfigurationResponse::getConfiguration() const
{
	return _configuration;
}


inline void GetAudioOutputConfigurationResponse::setConfiguration(const ONVIF::AudioOutputConfiguration& val)
{
	_configuration = val;
}


inline void GetAudioOutputConfigurationResponse::setConfiguration(ONVIF::AudioOutputConfiguration&& val)
{
	_configuration = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioOutputConfigurationResponse_INCLUDED
