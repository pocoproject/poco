// 
// GetAudioDecoderConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioDecoderConfigurationResponse_INCLUDED
#define ONVIF_Media_GetAudioDecoderConfigurationResponse_INCLUDED


#include "ONVIF/AudioDecoderConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioDecoderConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioDecoderConfigurationResponse
{
public:
	GetAudioDecoderConfigurationResponse();

	GetAudioDecoderConfigurationResponse(const ONVIF::AudioDecoderConfiguration& configuration);

	virtual ~GetAudioDecoderConfigurationResponse();

	const ONVIF::AudioDecoderConfiguration& getConfiguration() const;

	void setConfiguration(const ONVIF::AudioDecoderConfiguration& val);

	void setConfiguration(ONVIF::AudioDecoderConfiguration&& val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::AudioDecoderConfiguration _configuration;

};


inline const ONVIF::AudioDecoderConfiguration& GetAudioDecoderConfigurationResponse::getConfiguration() const
{
	return _configuration;
}


inline void GetAudioDecoderConfigurationResponse::setConfiguration(const ONVIF::AudioDecoderConfiguration& val)
{
	_configuration = val;
}


inline void GetAudioDecoderConfigurationResponse::setConfiguration(ONVIF::AudioDecoderConfiguration&& val)
{
	_configuration = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioDecoderConfigurationResponse_INCLUDED
