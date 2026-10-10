// 
// AudioDecoderConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioDecoderConfigurationOptions_INCLUDED
#define ONVIF_AudioDecoderConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AACDecOptions;
class AudioDecoderConfigurationOptionsExtension;
class G711DecOptions;
class G726DecOptions;
} 


namespace ONVIF {


//@ name=AudioDecoderConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioDecoderConfigurationOptions
{
public:
	AudioDecoderConfigurationOptions();

	AudioDecoderConfigurationOptions(
		const Poco::SharedPtr<AACDecOptions>& aACDecOptions, 
		const Poco::SharedPtr<G711DecOptions>& g711DecOptions, 
		const Poco::SharedPtr<G726DecOptions>& g726DecOptions, 
		const Poco::SharedPtr<AudioDecoderConfigurationOptionsExtension>& extension);

	virtual ~AudioDecoderConfigurationOptions();

	const Poco::SharedPtr<AACDecOptions>& getAACDecOptions() const;

	const Poco::SharedPtr<AudioDecoderConfigurationOptionsExtension>& getExtension() const;

	const Poco::SharedPtr<G711DecOptions>& getG711DecOptions() const;

	const Poco::SharedPtr<G726DecOptions>& getG726DecOptions() const;

	void setAACDecOptions(const Poco::SharedPtr<AACDecOptions>& val);

	void setAACDecOptions(Poco::SharedPtr<AACDecOptions>&& val);

	void setExtension(const Poco::SharedPtr<AudioDecoderConfigurationOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<AudioDecoderConfigurationOptionsExtension>&& val);

	void setG711DecOptions(const Poco::SharedPtr<G711DecOptions>& val);

	void setG711DecOptions(Poco::SharedPtr<G711DecOptions>&& val);

	void setG726DecOptions(const Poco::SharedPtr<G726DecOptions>& val);

	void setG726DecOptions(Poco::SharedPtr<G726DecOptions>&& val);

private:
	//@ mandatory=false
	//@ name=AACDecOptions
	//@ order=0
	Poco::SharedPtr<AACDecOptions> _aACDecOptions;

	//@ mandatory=false
	//@ name=G711DecOptions
	//@ order=1
	Poco::SharedPtr<G711DecOptions> _g711DecOptions;

	//@ mandatory=false
	//@ name=G726DecOptions
	//@ order=2
	Poco::SharedPtr<G726DecOptions> _g726DecOptions;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<AudioDecoderConfigurationOptionsExtension> _extension;

};


inline const Poco::SharedPtr<AACDecOptions>& AudioDecoderConfigurationOptions::getAACDecOptions() const
{
	return _aACDecOptions;
}


inline const Poco::SharedPtr<AudioDecoderConfigurationOptionsExtension>& AudioDecoderConfigurationOptions::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<G711DecOptions>& AudioDecoderConfigurationOptions::getG711DecOptions() const
{
	return _g711DecOptions;
}


inline const Poco::SharedPtr<G726DecOptions>& AudioDecoderConfigurationOptions::getG726DecOptions() const
{
	return _g726DecOptions;
}


inline void AudioDecoderConfigurationOptions::setAACDecOptions(const Poco::SharedPtr<AACDecOptions>& val)
{
	_aACDecOptions = val;
}


inline void AudioDecoderConfigurationOptions::setAACDecOptions(Poco::SharedPtr<AACDecOptions>&& val)
{
	_aACDecOptions = std::move(val);
}


inline void AudioDecoderConfigurationOptions::setExtension(const Poco::SharedPtr<AudioDecoderConfigurationOptionsExtension>& val)
{
	_extension = val;
}


inline void AudioDecoderConfigurationOptions::setExtension(Poco::SharedPtr<AudioDecoderConfigurationOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void AudioDecoderConfigurationOptions::setG711DecOptions(const Poco::SharedPtr<G711DecOptions>& val)
{
	_g711DecOptions = val;
}


inline void AudioDecoderConfigurationOptions::setG711DecOptions(Poco::SharedPtr<G711DecOptions>&& val)
{
	_g711DecOptions = std::move(val);
}


inline void AudioDecoderConfigurationOptions::setG726DecOptions(const Poco::SharedPtr<G726DecOptions>& val)
{
	_g726DecOptions = val;
}


inline void AudioDecoderConfigurationOptions::setG726DecOptions(Poco::SharedPtr<G726DecOptions>&& val)
{
	_g726DecOptions = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AACDecOptions.h"
#include "ONVIF/AudioDecoderConfigurationOptionsExtension.h"
#include "ONVIF/G711DecOptions.h"
#include "ONVIF/G726DecOptions.h"


#endif // ONVIF_AudioDecoderConfigurationOptions_INCLUDED
