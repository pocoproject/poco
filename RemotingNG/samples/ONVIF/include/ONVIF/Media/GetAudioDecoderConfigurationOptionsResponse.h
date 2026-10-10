// 
// GetAudioDecoderConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioDecoderConfigurationOptionsResponse_INCLUDED
#define ONVIF_Media_GetAudioDecoderConfigurationOptionsResponse_INCLUDED


#include "ONVIF/AudioDecoderConfigurationOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioDecoderConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioDecoderConfigurationOptionsResponse
{
public:
	GetAudioDecoderConfigurationOptionsResponse();

	GetAudioDecoderConfigurationOptionsResponse(const ONVIF::AudioDecoderConfigurationOptions& options);

	virtual ~GetAudioDecoderConfigurationOptionsResponse();

	const ONVIF::AudioDecoderConfigurationOptions& getOptions() const;

	void setOptions(const ONVIF::AudioDecoderConfigurationOptions& val);

	void setOptions(ONVIF::AudioDecoderConfigurationOptions&& val);

private:
	//@ name=Options
	//@ order=0
	ONVIF::AudioDecoderConfigurationOptions _options;

};


inline const ONVIF::AudioDecoderConfigurationOptions& GetAudioDecoderConfigurationOptionsResponse::getOptions() const
{
	return _options;
}


inline void GetAudioDecoderConfigurationOptionsResponse::setOptions(const ONVIF::AudioDecoderConfigurationOptions& val)
{
	_options = val;
}


inline void GetAudioDecoderConfigurationOptionsResponse::setOptions(ONVIF::AudioDecoderConfigurationOptions&& val)
{
	_options = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioDecoderConfigurationOptionsResponse_INCLUDED
