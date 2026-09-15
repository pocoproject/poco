// 
// AudioEncoder2Configuration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioEncoder2Configuration_INCLUDED
#define ONVIF_AudioEncoder2Configuration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class MulticastConfiguration;
} 


namespace ONVIF {


//@ name=AudioEncoder2Configuration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioEncoder2Configuration: public ConfigurationEntity
{
public:
	AudioEncoder2Configuration();

	AudioEncoder2Configuration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const std::string& encoding, 
		const Poco::SharedPtr<MulticastConfiguration>& multicast, 
		int bitrate, 
		int sampleRate);

	virtual ~AudioEncoder2Configuration();

	int getBitrate() const;

	const std::string& getEncoding() const;

	const Poco::SharedPtr<MulticastConfiguration>& getMulticast() const;

	int getSampleRate() const;

	void setBitrate(int val);

	void setEncoding(const std::string& val);

	void setEncoding(std::string&& val);

	void setMulticast(const Poco::SharedPtr<MulticastConfiguration>& val);

	void setMulticast(Poco::SharedPtr<MulticastConfiguration>&& val);

	void setSampleRate(int val);

private:
	//@ name=Encoding
	//@ order=0
	std::string _encoding;

	//@ mandatory=false
	//@ name=Multicast
	//@ order=1
	Poco::SharedPtr<MulticastConfiguration> _multicast;

	//@ name=Bitrate
	//@ order=2
	int _bitrate;

	//@ name=SampleRate
	//@ order=3
	int _sampleRate;

};


inline int AudioEncoder2Configuration::getBitrate() const
{
	return _bitrate;
}


inline const std::string& AudioEncoder2Configuration::getEncoding() const
{
	return _encoding;
}


inline const Poco::SharedPtr<MulticastConfiguration>& AudioEncoder2Configuration::getMulticast() const
{
	return _multicast;
}


inline int AudioEncoder2Configuration::getSampleRate() const
{
	return _sampleRate;
}


inline void AudioEncoder2Configuration::setBitrate(int val)
{
	_bitrate = val;
}


inline void AudioEncoder2Configuration::setEncoding(const std::string& val)
{
	_encoding = val;
}


inline void AudioEncoder2Configuration::setEncoding(std::string&& val)
{
	_encoding = std::move(val);
}


inline void AudioEncoder2Configuration::setMulticast(const Poco::SharedPtr<MulticastConfiguration>& val)
{
	_multicast = val;
}


inline void AudioEncoder2Configuration::setMulticast(Poco::SharedPtr<MulticastConfiguration>&& val)
{
	_multicast = std::move(val);
}


inline void AudioEncoder2Configuration::setSampleRate(int val)
{
	_sampleRate = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/MulticastConfiguration.h"


#endif // ONVIF_AudioEncoder2Configuration_INCLUDED
