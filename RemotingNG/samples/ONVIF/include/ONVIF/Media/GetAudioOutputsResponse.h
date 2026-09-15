// 
// GetAudioOutputsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioOutputsResponse_INCLUDED
#define ONVIF_Media_GetAudioOutputsResponse_INCLUDED


#include "ONVIF/AudioOutput.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioOutputsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioOutputsResponse
{
public:
	GetAudioOutputsResponse();

	GetAudioOutputsResponse(const std::vector<ONVIF::AudioOutput>& audioOutputs);

	virtual ~GetAudioOutputsResponse();

	const std::vector<ONVIF::AudioOutput>& getAudioOutputs() const;

	std::vector<ONVIF::AudioOutput>& getAudioOutputs();

	void setAudioOutputs(const std::vector<ONVIF::AudioOutput>& val);

	void setAudioOutputs(std::vector<ONVIF::AudioOutput>&& val);

private:
	//@ mandatory=false
	//@ name=AudioOutputs
	//@ order=0
	std::vector<ONVIF::AudioOutput> _audioOutputs;

};


inline const std::vector<ONVIF::AudioOutput>& GetAudioOutputsResponse::getAudioOutputs() const
{
	return _audioOutputs;
}


inline std::vector<ONVIF::AudioOutput>& GetAudioOutputsResponse::getAudioOutputs()
{
	return _audioOutputs;
}


inline void GetAudioOutputsResponse::setAudioOutputs(const std::vector<ONVIF::AudioOutput>& val)
{
	_audioOutputs = val;
}


inline void GetAudioOutputsResponse::setAudioOutputs(std::vector<ONVIF::AudioOutput>&& val)
{
	_audioOutputs = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioOutputsResponse_INCLUDED
