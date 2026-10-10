// 
// AudioOutputConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioOutputConfigurationOptions_INCLUDED
#define ONVIF_AudioOutputConfigurationOptions_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioOutputConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioOutputConfigurationOptions
{
public:
	AudioOutputConfigurationOptions();

	AudioOutputConfigurationOptions(
		const std::vector<std::string>& outputTokensAvailable, 
		const std::vector<Poco::URI>& sendPrimacyOptions, 
		const IntRange& outputLevelRange);

	virtual ~AudioOutputConfigurationOptions();

	const IntRange& getOutputLevelRange() const;

	const std::vector<std::string>& getOutputTokensAvailable() const;

	std::vector<std::string>& getOutputTokensAvailable();

	const std::vector<Poco::URI>& getSendPrimacyOptions() const;

	std::vector<Poco::URI>& getSendPrimacyOptions();

	void setOutputLevelRange(const IntRange& val);

	void setOutputLevelRange(IntRange&& val);

	void setOutputTokensAvailable(const std::vector<std::string>& val);

	void setOutputTokensAvailable(std::vector<std::string>&& val);

	void setSendPrimacyOptions(const std::vector<Poco::URI>& val);

	void setSendPrimacyOptions(std::vector<Poco::URI>&& val);

private:
	//@ name=OutputTokensAvailable
	//@ order=0
	std::vector<std::string> _outputTokensAvailable;

	//@ mandatory=false
	//@ name=SendPrimacyOptions
	//@ order=1
	std::vector<Poco::URI> _sendPrimacyOptions;

	//@ name=OutputLevelRange
	//@ order=2
	IntRange _outputLevelRange;

};


inline const IntRange& AudioOutputConfigurationOptions::getOutputLevelRange() const
{
	return _outputLevelRange;
}


inline const std::vector<std::string>& AudioOutputConfigurationOptions::getOutputTokensAvailable() const
{
	return _outputTokensAvailable;
}


inline std::vector<std::string>& AudioOutputConfigurationOptions::getOutputTokensAvailable()
{
	return _outputTokensAvailable;
}


inline const std::vector<Poco::URI>& AudioOutputConfigurationOptions::getSendPrimacyOptions() const
{
	return _sendPrimacyOptions;
}


inline std::vector<Poco::URI>& AudioOutputConfigurationOptions::getSendPrimacyOptions()
{
	return _sendPrimacyOptions;
}


inline void AudioOutputConfigurationOptions::setOutputLevelRange(const IntRange& val)
{
	_outputLevelRange = val;
}


inline void AudioOutputConfigurationOptions::setOutputLevelRange(IntRange&& val)
{
	_outputLevelRange = std::move(val);
}


inline void AudioOutputConfigurationOptions::setOutputTokensAvailable(const std::vector<std::string>& val)
{
	_outputTokensAvailable = val;
}


inline void AudioOutputConfigurationOptions::setOutputTokensAvailable(std::vector<std::string>&& val)
{
	_outputTokensAvailable = std::move(val);
}


inline void AudioOutputConfigurationOptions::setSendPrimacyOptions(const std::vector<Poco::URI>& val)
{
	_sendPrimacyOptions = val;
}


inline void AudioOutputConfigurationOptions::setSendPrimacyOptions(std::vector<Poco::URI>&& val)
{
	_sendPrimacyOptions = std::move(val);
}


} // ONVIF


#endif // ONVIF_AudioOutputConfigurationOptions_INCLUDED
