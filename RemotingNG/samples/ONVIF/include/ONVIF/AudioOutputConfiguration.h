// 
// AudioOutputConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioOutputConfiguration_INCLUDED
#define ONVIF_AudioOutputConfiguration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioOutputConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioOutputConfiguration: public ConfigurationEntity
{
public:
	AudioOutputConfiguration();

	AudioOutputConfiguration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const std::string& outputToken, 
		const Poco::Optional<Poco::URI>& sendPrimacy, 
		int outputLevel);

	virtual ~AudioOutputConfiguration();

	int getOutputLevel() const;

	const std::string& getOutputToken() const;

	const Poco::Optional<Poco::URI>& getSendPrimacy() const;

	void setOutputLevel(int val);

	void setOutputToken(const std::string& val);

	void setOutputToken(std::string&& val);

	void setSendPrimacy(const Poco::Optional<Poco::URI>& val);

	void setSendPrimacy(Poco::Optional<Poco::URI>&& val);

private:
	//@ name=OutputToken
	//@ order=0
	std::string _outputToken;

	//@ mandatory=false
	//@ name=SendPrimacy
	//@ order=1
	Poco::Optional<Poco::URI> _sendPrimacy;

	//@ name=OutputLevel
	//@ order=2
	int _outputLevel;

};


inline int AudioOutputConfiguration::getOutputLevel() const
{
	return _outputLevel;
}


inline const std::string& AudioOutputConfiguration::getOutputToken() const
{
	return _outputToken;
}


inline const Poco::Optional<Poco::URI>& AudioOutputConfiguration::getSendPrimacy() const
{
	return _sendPrimacy;
}


inline void AudioOutputConfiguration::setOutputLevel(int val)
{
	_outputLevel = val;
}


inline void AudioOutputConfiguration::setOutputToken(const std::string& val)
{
	_outputToken = val;
}


inline void AudioOutputConfiguration::setOutputToken(std::string&& val)
{
	_outputToken = std::move(val);
}


inline void AudioOutputConfiguration::setSendPrimacy(const Poco::Optional<Poco::URI>& val)
{
	_sendPrimacy = val;
}


inline void AudioOutputConfiguration::setSendPrimacy(Poco::Optional<Poco::URI>&& val)
{
	_sendPrimacy = std::move(val);
}


} // ONVIF


#endif // ONVIF_AudioOutputConfiguration_INCLUDED
