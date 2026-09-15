// 
// AudioSourceConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioSourceConfigurationOptions_INCLUDED
#define ONVIF_AudioSourceConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AudioSourceOptionsExtension;
} 


namespace ONVIF {


//@ name=AudioSourceConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioSourceConfigurationOptions
{
public:
	AudioSourceConfigurationOptions();

	AudioSourceConfigurationOptions(
		const std::vector<std::string>& inputTokensAvailable, 
		const Poco::SharedPtr<AudioSourceOptionsExtension>& extension);

	virtual ~AudioSourceConfigurationOptions();

	const Poco::SharedPtr<AudioSourceOptionsExtension>& getExtension() const;

	const std::vector<std::string>& getInputTokensAvailable() const;

	std::vector<std::string>& getInputTokensAvailable();

	void setExtension(const Poco::SharedPtr<AudioSourceOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<AudioSourceOptionsExtension>&& val);

	void setInputTokensAvailable(const std::vector<std::string>& val);

	void setInputTokensAvailable(std::vector<std::string>&& val);

private:
	//@ name=InputTokensAvailable
	//@ order=0
	std::vector<std::string> _inputTokensAvailable;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<AudioSourceOptionsExtension> _extension;

};


inline const Poco::SharedPtr<AudioSourceOptionsExtension>& AudioSourceConfigurationOptions::getExtension() const
{
	return _extension;
}


inline const std::vector<std::string>& AudioSourceConfigurationOptions::getInputTokensAvailable() const
{
	return _inputTokensAvailable;
}


inline std::vector<std::string>& AudioSourceConfigurationOptions::getInputTokensAvailable()
{
	return _inputTokensAvailable;
}


inline void AudioSourceConfigurationOptions::setExtension(const Poco::SharedPtr<AudioSourceOptionsExtension>& val)
{
	_extension = val;
}


inline void AudioSourceConfigurationOptions::setExtension(Poco::SharedPtr<AudioSourceOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void AudioSourceConfigurationOptions::setInputTokensAvailable(const std::vector<std::string>& val)
{
	_inputTokensAvailable = val;
}


inline void AudioSourceConfigurationOptions::setInputTokensAvailable(std::vector<std::string>&& val)
{
	_inputTokensAvailable = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AudioSourceOptionsExtension.h"


#endif // ONVIF_AudioSourceConfigurationOptions_INCLUDED
