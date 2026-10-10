// 
// GetAudioSourceConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioSourceConfigurationResponse_INCLUDED
#define ONVIF_Media_GetAudioSourceConfigurationResponse_INCLUDED


#include "ONVIF/AudioSourceConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioSourceConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioSourceConfigurationResponse
{
public:
	GetAudioSourceConfigurationResponse();

	GetAudioSourceConfigurationResponse(const ONVIF::AudioSourceConfiguration& configuration);

	virtual ~GetAudioSourceConfigurationResponse();

	const ONVIF::AudioSourceConfiguration& getConfiguration() const;

	void setConfiguration(const ONVIF::AudioSourceConfiguration& val);

	void setConfiguration(ONVIF::AudioSourceConfiguration&& val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::AudioSourceConfiguration _configuration;

};


inline const ONVIF::AudioSourceConfiguration& GetAudioSourceConfigurationResponse::getConfiguration() const
{
	return _configuration;
}


inline void GetAudioSourceConfigurationResponse::setConfiguration(const ONVIF::AudioSourceConfiguration& val)
{
	_configuration = val;
}


inline void GetAudioSourceConfigurationResponse::setConfiguration(ONVIF::AudioSourceConfiguration&& val)
{
	_configuration = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioSourceConfigurationResponse_INCLUDED
