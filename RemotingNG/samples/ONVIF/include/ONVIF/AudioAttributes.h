// 
// AudioAttributes.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioAttributes_INCLUDED
#define ONVIF_AudioAttributes_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioAttributes
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioAttributes
{
public:
	AudioAttributes();

	AudioAttributes(
		Poco::Optional<int> bitrate, 
		const std::string& encoding, 
		int samplerate);

	virtual ~AudioAttributes();

	Poco::Optional<int> getBitrate() const;

	const std::string& getEncoding() const;

	int getSamplerate() const;

	void setBitrate(Poco::Optional<int> val);

	void setEncoding(const std::string& val);

	void setEncoding(std::string&& val);

	void setSamplerate(int val);

private:
	//@ mandatory=false
	//@ name=Bitrate
	//@ order=0
	Poco::Optional<int> _bitrate;

	//@ name=Encoding
	//@ order=1
	std::string _encoding;

	//@ name=Samplerate
	//@ order=2
	int _samplerate;

};


inline Poco::Optional<int> AudioAttributes::getBitrate() const
{
	return _bitrate;
}


inline const std::string& AudioAttributes::getEncoding() const
{
	return _encoding;
}


inline int AudioAttributes::getSamplerate() const
{
	return _samplerate;
}


inline void AudioAttributes::setBitrate(Poco::Optional<int> val)
{
	_bitrate = val;
}


inline void AudioAttributes::setEncoding(const std::string& val)
{
	_encoding = val;
}


inline void AudioAttributes::setEncoding(std::string&& val)
{
	_encoding = std::move(val);
}


inline void AudioAttributes::setSamplerate(int val)
{
	_samplerate = val;
}


} // ONVIF


#endif // ONVIF_AudioAttributes_INCLUDED
