// 
// AudioEncoderConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioEncoderConfiguration_INCLUDED
#define ONVIF_AudioEncoderConfiguration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/MulticastConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioEncoderConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioEncoderConfiguration: public ConfigurationEntity
{
public:
	AudioEncoderConfiguration();

	AudioEncoderConfiguration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const std::string& encoding, 
		int bitrate, 
		int sampleRate, 
		const MulticastConfiguration& multicast, 
		const std::string& sessionTimeout);

	virtual ~AudioEncoderConfiguration();

	int getBitrate() const;

	const std::string& getEncoding() const;

	const MulticastConfiguration& getMulticast() const;

	int getSampleRate() const;

	const std::string& getSessionTimeout() const;

	void setBitrate(int val);

	void setEncoding(const std::string& val);

	void setEncoding(std::string&& val);

	void setMulticast(const MulticastConfiguration& val);

	void setMulticast(MulticastConfiguration&& val);

	void setSampleRate(int val);

	void setSessionTimeout(const std::string& val);

	void setSessionTimeout(std::string&& val);

private:
	//@ name=Encoding
	//@ order=0
	std::string _encoding;

	//@ name=Bitrate
	//@ order=1
	int _bitrate;

	//@ name=SampleRate
	//@ order=2
	int _sampleRate;

	//@ name=Multicast
	//@ order=3
	MulticastConfiguration _multicast;

	//@ name=SessionTimeout
	//@ order=4
	std::string _sessionTimeout;

};


inline int AudioEncoderConfiguration::getBitrate() const
{
	return _bitrate;
}


inline const std::string& AudioEncoderConfiguration::getEncoding() const
{
	return _encoding;
}


inline const MulticastConfiguration& AudioEncoderConfiguration::getMulticast() const
{
	return _multicast;
}


inline int AudioEncoderConfiguration::getSampleRate() const
{
	return _sampleRate;
}


inline const std::string& AudioEncoderConfiguration::getSessionTimeout() const
{
	return _sessionTimeout;
}


inline void AudioEncoderConfiguration::setBitrate(int val)
{
	_bitrate = val;
}


inline void AudioEncoderConfiguration::setEncoding(const std::string& val)
{
	_encoding = val;
}


inline void AudioEncoderConfiguration::setEncoding(std::string&& val)
{
	_encoding = std::move(val);
}


inline void AudioEncoderConfiguration::setMulticast(const MulticastConfiguration& val)
{
	_multicast = val;
}


inline void AudioEncoderConfiguration::setMulticast(MulticastConfiguration&& val)
{
	_multicast = std::move(val);
}


inline void AudioEncoderConfiguration::setSampleRate(int val)
{
	_sampleRate = val;
}


inline void AudioEncoderConfiguration::setSessionTimeout(const std::string& val)
{
	_sessionTimeout = val;
}


inline void AudioEncoderConfiguration::setSessionTimeout(std::string&& val)
{
	_sessionTimeout = std::move(val);
}


} // ONVIF


#endif // ONVIF_AudioEncoderConfiguration_INCLUDED
