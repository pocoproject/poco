// 
// GetAudioOutputConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioOutputConfigurationOptionsResponse_INCLUDED
#define ONVIF_Media_GetAudioOutputConfigurationOptionsResponse_INCLUDED


#include "ONVIF/AudioOutputConfigurationOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioOutputConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioOutputConfigurationOptionsResponse
{
public:
	GetAudioOutputConfigurationOptionsResponse();

	GetAudioOutputConfigurationOptionsResponse(const ONVIF::AudioOutputConfigurationOptions& options);

	virtual ~GetAudioOutputConfigurationOptionsResponse();

	const ONVIF::AudioOutputConfigurationOptions& getOptions() const;

	void setOptions(const ONVIF::AudioOutputConfigurationOptions& val);

	void setOptions(ONVIF::AudioOutputConfigurationOptions&& val);

private:
	//@ name=Options
	//@ order=0
	ONVIF::AudioOutputConfigurationOptions _options;

};


inline const ONVIF::AudioOutputConfigurationOptions& GetAudioOutputConfigurationOptionsResponse::getOptions() const
{
	return _options;
}


inline void GetAudioOutputConfigurationOptionsResponse::setOptions(const ONVIF::AudioOutputConfigurationOptions& val)
{
	_options = val;
}


inline void GetAudioOutputConfigurationOptionsResponse::setOptions(ONVIF::AudioOutputConfigurationOptions&& val)
{
	_options = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioOutputConfigurationOptionsResponse_INCLUDED
