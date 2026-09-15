// 
// AudioEncoder2ConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioEncoder2ConfigurationOptions_INCLUDED
#define ONVIF_AudioEncoder2ConfigurationOptions_INCLUDED


#include "ONVIF/IntItems.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioEncoder2ConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioEncoder2ConfigurationOptions
{
public:
	AudioEncoder2ConfigurationOptions();

	AudioEncoder2ConfigurationOptions(
		const std::string& encoding, 
		const IntItems& bitrateList, 
		const IntItems& sampleRateList);

	virtual ~AudioEncoder2ConfigurationOptions();

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


inline const IntItems& AudioEncoder2ConfigurationOptions::getBitrateList() const
{
	return _bitrateList;
}


inline const std::string& AudioEncoder2ConfigurationOptions::getEncoding() const
{
	return _encoding;
}


inline const IntItems& AudioEncoder2ConfigurationOptions::getSampleRateList() const
{
	return _sampleRateList;
}


inline void AudioEncoder2ConfigurationOptions::setBitrateList(const IntItems& val)
{
	_bitrateList = val;
}


inline void AudioEncoder2ConfigurationOptions::setBitrateList(IntItems&& val)
{
	_bitrateList = std::move(val);
}


inline void AudioEncoder2ConfigurationOptions::setEncoding(const std::string& val)
{
	_encoding = val;
}


inline void AudioEncoder2ConfigurationOptions::setEncoding(std::string&& val)
{
	_encoding = std::move(val);
}


inline void AudioEncoder2ConfigurationOptions::setSampleRateList(const IntItems& val)
{
	_sampleRateList = val;
}


inline void AudioEncoder2ConfigurationOptions::setSampleRateList(IntItems&& val)
{
	_sampleRateList = std::move(val);
}


} // ONVIF


#endif // ONVIF_AudioEncoder2ConfigurationOptions_INCLUDED
