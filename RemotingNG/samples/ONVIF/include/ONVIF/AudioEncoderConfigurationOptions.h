// 
// AudioEncoderConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioEncoderConfigurationOptions_INCLUDED
#define ONVIF_AudioEncoderConfigurationOptions_INCLUDED


#include "ONVIF/AudioEncoderConfigurationOption.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioEncoderConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioEncoderConfigurationOptions
{
public:
	AudioEncoderConfigurationOptions();

	AudioEncoderConfigurationOptions(const std::vector<AudioEncoderConfigurationOption>& options);

	virtual ~AudioEncoderConfigurationOptions();

	const std::vector<AudioEncoderConfigurationOption>& getOptions() const;

	std::vector<AudioEncoderConfigurationOption>& getOptions();

	void setOptions(const std::vector<AudioEncoderConfigurationOption>& val);

	void setOptions(std::vector<AudioEncoderConfigurationOption>&& val);

private:
	//@ mandatory=false
	//@ name=Options
	//@ order=0
	std::vector<AudioEncoderConfigurationOption> _options;

};


inline const std::vector<AudioEncoderConfigurationOption>& AudioEncoderConfigurationOptions::getOptions() const
{
	return _options;
}


inline std::vector<AudioEncoderConfigurationOption>& AudioEncoderConfigurationOptions::getOptions()
{
	return _options;
}


inline void AudioEncoderConfigurationOptions::setOptions(const std::vector<AudioEncoderConfigurationOption>& val)
{
	_options = val;
}


inline void AudioEncoderConfigurationOptions::setOptions(std::vector<AudioEncoderConfigurationOption>&& val)
{
	_options = std::move(val);
}


} // ONVIF


#endif // ONVIF_AudioEncoderConfigurationOptions_INCLUDED
