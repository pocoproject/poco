// 
// AudioSourceConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioSourceConfiguration_INCLUDED
#define ONVIF_AudioSourceConfiguration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioSourceConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioSourceConfiguration: public ConfigurationEntity
{
public:
	AudioSourceConfiguration();

	AudioSourceConfiguration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const std::string& sourceToken);

	virtual ~AudioSourceConfiguration();

	const std::string& getSourceToken() const;

	void setSourceToken(const std::string& val);

	void setSourceToken(std::string&& val);

private:
	//@ name=SourceToken
	//@ order=0
	std::string _sourceToken;

};


inline const std::string& AudioSourceConfiguration::getSourceToken() const
{
	return _sourceToken;
}


inline void AudioSourceConfiguration::setSourceToken(const std::string& val)
{
	_sourceToken = val;
}


inline void AudioSourceConfiguration::setSourceToken(std::string&& val)
{
	_sourceToken = std::move(val);
}


} // ONVIF


#endif // ONVIF_AudioSourceConfiguration_INCLUDED
