// 
// GetAudioEncoderConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioEncoderConfigurationOptionsResponse_INCLUDED
#define ONVIF_Media_GetAudioEncoderConfigurationOptionsResponse_INCLUDED


#include "ONVIF/AudioEncoderConfigurationOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioEncoderConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioEncoderConfigurationOptionsResponse
{
public:
	GetAudioEncoderConfigurationOptionsResponse();

	GetAudioEncoderConfigurationOptionsResponse(const ONVIF::AudioEncoderConfigurationOptions& options);

	virtual ~GetAudioEncoderConfigurationOptionsResponse();

	const ONVIF::AudioEncoderConfigurationOptions& getOptions() const;

	void setOptions(const ONVIF::AudioEncoderConfigurationOptions& val);

	void setOptions(ONVIF::AudioEncoderConfigurationOptions&& val);

private:
	//@ name=Options
	//@ order=0
	ONVIF::AudioEncoderConfigurationOptions _options;

};


inline const ONVIF::AudioEncoderConfigurationOptions& GetAudioEncoderConfigurationOptionsResponse::getOptions() const
{
	return _options;
}


inline void GetAudioEncoderConfigurationOptionsResponse::setOptions(const ONVIF::AudioEncoderConfigurationOptions& val)
{
	_options = val;
}


inline void GetAudioEncoderConfigurationOptionsResponse::setOptions(ONVIF::AudioEncoderConfigurationOptions&& val)
{
	_options = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioEncoderConfigurationOptionsResponse_INCLUDED
