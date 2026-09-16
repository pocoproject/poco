// 
// AudioEncoderConfigurationOption.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioEncoderConfigurationOption_INCLUDED
#define ONVIF_AudioEncoderConfigurationOption_INCLUDED


#include "ONVIF/IntItems.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioEncoderConfigurationOption
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioEncoderConfigurationOption
{
public:
	AudioEncoderConfigurationOption();

	AudioEncoderConfigurationOption(
		const std::string& encoding, 
		const IntItems& bitrateList, 
		const IntItems& sampleRateList);

	virtual ~AudioEncoderConfigurationOption();

	const IntItems& getBitrateList() const;

	const std::string& getEncoding() const;

	const IntItems& getSampleRateList() const;

	void setBitrateList(const IntItems& val);

	void setBitrateList(IntItems&& val);

	void setEncoding(const std::string& val);

	void setEncoding(std::string&& val);

	void setSampleRateList(const IntItems& val);

	void setSampleRateList(IntItems&& val);

private:
	//@ name=Encoding
	//@ order=0
	std::string _encoding;

	//@ name=BitrateList
	//@ order=1
	IntItems _bitrateList;

	//@ name=SampleRateList
	//@ order=2
	IntItems _sampleRateList;

};


inline const IntItems& AudioEncoderConfigurationOption::getBitrateList() const
{
	return _bitrateList;
}


inline const std::string& AudioEncoderConfigurationOption::getEncoding() const
{
	return _encoding;
}


inline const IntItems& AudioEncoderConfigurationOption::getSampleRateList() const
{
	return _sampleRateList;
}


inline void AudioEncoderConfigurationOption::setBitrateList(const IntItems& val)
{
	_bitrateList = val;
}


inline void AudioEncoderConfigurationOption::setBitrateList(IntItems&& val)
{
	_bitrateList = std::move(val);
}


inline void AudioEncoderConfigurationOption::setEncoding(const std::string& val)
{
	_encoding = val;
}


inline void AudioEncoderConfigurationOption::setEncoding(std::string&& val)
{
	_encoding = std::move(val);
}


inline void AudioEncoderConfigurationOption::setSampleRateList(const IntItems& val)
{
	_sampleRateList = val;
}


inline void AudioEncoderConfigurationOption::setSampleRateList(IntItems&& val)
{
	_sampleRateList = std::move(val);
}


} // ONVIF


#endif // ONVIF_AudioEncoderConfigurationOption_INCLUDED
