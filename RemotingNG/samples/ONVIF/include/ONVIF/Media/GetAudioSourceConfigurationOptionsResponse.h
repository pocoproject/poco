// 
// GetAudioSourceConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioSourceConfigurationOptionsResponse_INCLUDED
#define ONVIF_Media_GetAudioSourceConfigurationOptionsResponse_INCLUDED


#include "ONVIF/AudioSourceConfigurationOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioSourceConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioSourceConfigurationOptionsResponse
{
public:
	GetAudioSourceConfigurationOptionsResponse();

	GetAudioSourceConfigurationOptionsResponse(const ONVIF::AudioSourceConfigurationOptions& options);

	virtual ~GetAudioSourceConfigurationOptionsResponse();

	const ONVIF::AudioSourceConfigurationOptions& getOptions() const;

	void setOptions(const ONVIF::AudioSourceConfigurationOptions& val);

	void setOptions(ONVIF::AudioSourceConfigurationOptions&& val);

private:
	//@ name=Options
	//@ order=0
	ONVIF::AudioSourceConfigurationOptions _options;

};


inline const ONVIF::AudioSourceConfigurationOptions& GetAudioSourceConfigurationOptionsResponse::getOptions() const
{
	return _options;
}


inline void GetAudioSourceConfigurationOptionsResponse::setOptions(const ONVIF::AudioSourceConfigurationOptions& val)
{
	_options = val;
}


inline void GetAudioSourceConfigurationOptionsResponse::setOptions(ONVIF::AudioSourceConfigurationOptions&& val)
{
	_options = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioSourceConfigurationOptionsResponse_INCLUDED
