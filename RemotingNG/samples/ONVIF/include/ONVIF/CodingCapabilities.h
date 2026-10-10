// 
// CodingCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_CodingCapabilities_INCLUDED
#define ONVIF_CodingCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoDecoderConfigurationOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AudioDecoderConfigurationOptions;
class AudioEncoderConfigurationOptions;
} 


namespace ONVIF {


//@ name=CodingCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API CodingCapabilities
{
public:
	CodingCapabilities();

	CodingCapabilities(
		const Poco::SharedPtr<AudioEncoderConfigurationOptions>& audioEncodingCapabilities, 
		const Poco::SharedPtr<AudioDecoderConfigurationOptions>& audioDecodingCapabilities, 
		const VideoDecoderConfigurationOptions& videoDecodingCapabilities);

	virtual ~CodingCapabilities();

	const Poco::SharedPtr<AudioDecoderConfigurationOptions>& getAudioDecodingCapabilities() const;

	const Poco::SharedPtr<AudioEncoderConfigurationOptions>& getAudioEncodingCapabilities() const;

	const VideoDecoderConfigurationOptions& getVideoDecodingCapabilities() const;

	void setAudioDecodingCapabilities(const Poco::SharedPtr<AudioDecoderConfigurationOptions>& val);

	void setAudioDecodingCapabilities(Poco::SharedPtr<AudioDecoderConfigurationOptions>&& val);

	void setAudioEncodingCapabilities(const Poco::SharedPtr<AudioEncoderConfigurationOptions>& val);

	void setAudioEncodingCapabilities(Poco::SharedPtr<AudioEncoderConfigurationOptions>&& val);

	void setVideoDecodingCapabilities(const VideoDecoderConfigurationOptions& val);

	void setVideoDecodingCapabilities(VideoDecoderConfigurationOptions&& val);

private:
	//@ mandatory=false
	//@ name=AudioEncodingCapabilities
	//@ order=0
	Poco::SharedPtr<AudioEncoderConfigurationOptions> _audioEncodingCapabilities;

	//@ mandatory=false
	//@ name=AudioDecodingCapabilities
	//@ order=1
	Poco::SharedPtr<AudioDecoderConfigurationOptions> _audioDecodingCapabilities;

	//@ name=VideoDecodingCapabilities
	//@ order=2
	VideoDecoderConfigurationOptions _videoDecodingCapabilities;

};


inline const Poco::SharedPtr<AudioDecoderConfigurationOptions>& CodingCapabilities::getAudioDecodingCapabilities() const
{
	return _audioDecodingCapabilities;
}


inline const Poco::SharedPtr<AudioEncoderConfigurationOptions>& CodingCapabilities::getAudioEncodingCapabilities() const
{
	return _audioEncodingCapabilities;
}


inline const VideoDecoderConfigurationOptions& CodingCapabilities::getVideoDecodingCapabilities() const
{
	return _videoDecodingCapabilities;
}


inline void CodingCapabilities::setAudioDecodingCapabilities(const Poco::SharedPtr<AudioDecoderConfigurationOptions>& val)
{
	_audioDecodingCapabilities = val;
}


inline void CodingCapabilities::setAudioDecodingCapabilities(Poco::SharedPtr<AudioDecoderConfigurationOptions>&& val)
{
	_audioDecodingCapabilities = std::move(val);
}


inline void CodingCapabilities::setAudioEncodingCapabilities(const Poco::SharedPtr<AudioEncoderConfigurationOptions>& val)
{
	_audioEncodingCapabilities = val;
}


inline void CodingCapabilities::setAudioEncodingCapabilities(Poco::SharedPtr<AudioEncoderConfigurationOptions>&& val)
{
	_audioEncodingCapabilities = std::move(val);
}


inline void CodingCapabilities::setVideoDecodingCapabilities(const VideoDecoderConfigurationOptions& val)
{
	_videoDecodingCapabilities = val;
}


inline void CodingCapabilities::setVideoDecodingCapabilities(VideoDecoderConfigurationOptions&& val)
{
	_videoDecodingCapabilities = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AudioDecoderConfigurationOptions.h"
#include "ONVIF/AudioEncoderConfigurationOptions.h"


#endif // ONVIF_CodingCapabilities_INCLUDED
