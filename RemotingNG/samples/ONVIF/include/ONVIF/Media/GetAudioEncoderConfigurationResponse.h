// 
// GetAudioEncoderConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioEncoderConfigurationResponse_INCLUDED
#define ONVIF_Media_GetAudioEncoderConfigurationResponse_INCLUDED


#include "ONVIF/AudioEncoderConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioEncoderConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioEncoderConfigurationResponse
{
public:
	GetAudioEncoderConfigurationResponse();

	GetAudioEncoderConfigurationResponse(const ONVIF::AudioEncoderConfiguration& configuration);

	virtual ~GetAudioEncoderConfigurationResponse();

	const ONVIF::AudioEncoderConfiguration& getConfiguration() const;

	void setConfiguration(const ONVIF::AudioEncoderConfiguration& val);

	void setConfiguration(ONVIF::AudioEncoderConfiguration&& val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::AudioEncoderConfiguration _configuration;

};


inline const ONVIF::AudioEncoderConfiguration& GetAudioEncoderConfigurationResponse::getConfiguration() const
{
	return _configuration;
}


inline void GetAudioEncoderConfigurationResponse::setConfiguration(const ONVIF::AudioEncoderConfiguration& val)
{
	_configuration = val;
}


inline void GetAudioEncoderConfigurationResponse::setConfiguration(ONVIF::AudioEncoderConfiguration&& val)
{
	_configuration = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioEncoderConfigurationResponse_INCLUDED
