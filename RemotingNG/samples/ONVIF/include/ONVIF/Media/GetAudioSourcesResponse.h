// 
// GetAudioSourcesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioSourcesResponse_INCLUDED
#define ONVIF_Media_GetAudioSourcesResponse_INCLUDED


#include "ONVIF/AudioSource.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioSourcesResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioSourcesResponse
{
public:
	GetAudioSourcesResponse();

	GetAudioSourcesResponse(const std::vector<ONVIF::AudioSource>& audioSources);

	virtual ~GetAudioSourcesResponse();

	const std::vector<ONVIF::AudioSource>& getAudioSources() const;

	std::vector<ONVIF::AudioSource>& getAudioSources();

	void setAudioSources(const std::vector<ONVIF::AudioSource>& val);

	void setAudioSources(std::vector<ONVIF::AudioSource>&& val);

private:
	//@ mandatory=false
	//@ name=AudioSources
	//@ order=0
	std::vector<ONVIF::AudioSource> _audioSources;

};


inline const std::vector<ONVIF::AudioSource>& GetAudioSourcesResponse::getAudioSources() const
{
	return _audioSources;
}


inline std::vector<ONVIF::AudioSource>& GetAudioSourcesResponse::getAudioSources()
{
	return _audioSources;
}


inline void GetAudioSourcesResponse::setAudioSources(const std::vector<ONVIF::AudioSource>& val)
{
	_audioSources = val;
}


inline void GetAudioSourcesResponse::setAudioSources(std::vector<ONVIF::AudioSource>&& val)
{
	_audioSources = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioSourcesResponse_INCLUDED
