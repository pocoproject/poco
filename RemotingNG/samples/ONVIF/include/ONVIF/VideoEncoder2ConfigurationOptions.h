// 
// VideoEncoder2ConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoEncoder2ConfigurationOptions_INCLUDED
#define ONVIF_VideoEncoder2ConfigurationOptions_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution2.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=VideoEncoder2ConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoEncoder2ConfigurationOptions
{
public:
	VideoEncoder2ConfigurationOptions();

	VideoEncoder2ConfigurationOptions(
		const std::vector<std::vector<int>>& govLengthRange, 
		const std::vector<std::vector<float>>& frameRatesSupported, 
		const std::vector<std::vector<std::string>>& profilesSupported, 
		Poco::Optional<bool> constantBitRateSupported, 
		Poco::Optional<bool> guaranteedFrameRateSupported, 
		const std::string& encoding, 
		const FloatRange& qualityRange, 
		const std::vector<VideoResolution2>& resolutionsAvailable, 
		const IntRange& bitrateRange);

	virtual ~VideoEncoder2ConfigurationOptions();

	const IntRange& getBitrateRange() const;

	Poco::Optional<bool> getConstantBitRateSupported() const;

	const std::string& getEncoding() const;

	const std::vector<std::vector<float>>& getFrameRatesSupported() const;

	std::vector<std::vector<float>>& getFrameRatesSupported();

	const std::vector<std::vector<int>>& getGovLengthRange() const;

	std::vector<std::vector<int>>& getGovLengthRange();

	Poco::Optional<bool> getGuaranteedFrameRateSupported() const;

	const std::vector<std::vector<std::string>>& getProfilesSupported() const;

	std::vector<std::vector<std::string>>& getProfilesSupported();

	const FloatRange& getQualityRange() const;

	const std::vector<VideoResolution2>& getResolutionsAvailable() const;

	std::vector<VideoResolution2>& getResolutionsAvailable();

	void setBitrateRange(const IntRange& val);

	void setBitrateRange(IntRange&& val);

	void setConstantBitRateSupported(Poco::Optional<bool> val);

	void setEncoding(const std::string& val);

	void setEncoding(std::string&& val);

	void setFrameRatesSupported(const std::vector<std::vector<float>>& val);

	void setFrameRatesSupported(std::vector<std::vector<float>>&& val);

	void setGovLengthRange(const std::vector<std::vector<int>>& val);

	void setGovLengthRange(std::vector<std::vector<int>>&& val);

	void setGuaranteedFrameRateSupported(Poco::Optional<bool> val);

	void setProfilesSupported(const std::vector<std::vector<std::string>>& val);

	void setProfilesSupported(std::vector<std::vector<std::string>>&& val);

	void setQualityRange(const FloatRange& val);

	void setQualityRange(FloatRange&& val);

	void setResolutionsAvailable(const std::vector<VideoResolution2>& val);

	void setResolutionsAvailable(std::vector<VideoResolution2>&& val);

private:
	//@ mandatory=false
	//@ name=GovLengthRange
	//@ order=0
	//@ type=attr
	std::vector<std::vector<int>> _govLengthRange;

	//@ mandatory=false
	//@ name=FrameRatesSupported
	//@ order=1
	//@ type=attr
	std::vector<std::vector<float>> _frameRatesSupported;

	//@ mandatory=false
	//@ name=ProfilesSupported
	//@ order=2
	//@ type=attr
	std::vector<std::vector<std::string>> _profilesSupported;

	//@ mandatory=false
	//@ name=ConstantBitRateSupported
	//@ order=3
	//@ type=attr
	Poco::Optional<bool> _constantBitRateSupported;

	//@ mandatory=false
	//@ name=GuaranteedFrameRateSupported
	//@ order=4
	//@ type=attr
	Poco::Optional<bool> _guaranteedFrameRateSupported;

	//@ name=Encoding
	//@ order=5
	std::string _encoding;

	//@ name=QualityRange
	//@ order=6
	FloatRange _qualityRange;

	//@ name=ResolutionsAvailable
	//@ order=7
	std::vector<VideoResolution2> _resolutionsAvailable;

	//@ name=BitrateRange
	//@ order=8
	IntRange _bitrateRange;

};


inline const IntRange& VideoEncoder2ConfigurationOptions::getBitrateRange() const
{
	return _bitrateRange;
}


inline Poco::Optional<bool> VideoEncoder2ConfigurationOptions::getConstantBitRateSupported() const
{
	return _constantBitRateSupported;
}


inline const std::string& VideoEncoder2ConfigurationOptions::getEncoding() const
{
	return _encoding;
}


inline const std::vector<std::vector<float>>& VideoEncoder2ConfigurationOptions::getFrameRatesSupported() const
{
	return _frameRatesSupported;
}


inline std::vector<std::vector<float>>& VideoEncoder2ConfigurationOptions::getFrameRatesSupported()
{
	return _frameRatesSupported;
}


inline const std::vector<std::vector<int>>& VideoEncoder2ConfigurationOptions::getGovLengthRange() const
{
	return _govLengthRange;
}


inline std::vector<std::vector<int>>& VideoEncoder2ConfigurationOptions::getGovLengthRange()
{
	return _govLengthRange;
}


inline Poco::Optional<bool> VideoEncoder2ConfigurationOptions::getGuaranteedFrameRateSupported() const
{
	return _guaranteedFrameRateSupported;
}


inline const std::vector<std::vector<std::string>>& VideoEncoder2ConfigurationOptions::getProfilesSupported() const
{
	return _profilesSupported;
}


inline std::vector<std::vector<std::string>>& VideoEncoder2ConfigurationOptions::getProfilesSupported()
{
	return _profilesSupported;
}


inline const FloatRange& VideoEncoder2ConfigurationOptions::getQualityRange() const
{
	return _qualityRange;
}


inline const std::vector<VideoResolution2>& VideoEncoder2ConfigurationOptions::getResolutionsAvailable() const
{
	return _resolutionsAvailable;
}


inline std::vector<VideoResolution2>& VideoEncoder2ConfigurationOptions::getResolutionsAvailable()
{
	return _resolutionsAvailable;
}


inline void VideoEncoder2ConfigurationOptions::setBitrateRange(const IntRange& val)
{
	_bitrateRange = val;
}


inline void VideoEncoder2ConfigurationOptions::setBitrateRange(IntRange&& val)
{
	_bitrateRange = std::move(val);
}


inline void VideoEncoder2ConfigurationOptions::setConstantBitRateSupported(Poco::Optional<bool> val)
{
	_constantBitRateSupported = val;
}


inline void VideoEncoder2ConfigurationOptions::setEncoding(const std::string& val)
{
	_encoding = val;
}


inline void VideoEncoder2ConfigurationOptions::setEncoding(std::string&& val)
{
	_encoding = std::move(val);
}


inline void VideoEncoder2ConfigurationOptions::setFrameRatesSupported(const std::vector<std::vector<float>>& val)
{
	_frameRatesSupported = val;
}


inline void VideoEncoder2ConfigurationOptions::setFrameRatesSupported(std::vector<std::vector<float>>&& val)
{
	_frameRatesSupported = std::move(val);
}


inline void VideoEncoder2ConfigurationOptions::setGovLengthRange(const std::vector<std::vector<int>>& val)
{
	_govLengthRange = val;
}


inline void VideoEncoder2ConfigurationOptions::setGovLengthRange(std::vector<std::vector<int>>&& val)
{
	_govLengthRange = std::move(val);
}


inline void VideoEncoder2ConfigurationOptions::setGuaranteedFrameRateSupported(Poco::Optional<bool> val)
{
	_guaranteedFrameRateSupported = val;
}


inline void VideoEncoder2ConfigurationOptions::setProfilesSupported(const std::vector<std::vector<std::string>>& val)
{
	_profilesSupported = val;
}


inline void VideoEncoder2ConfigurationOptions::setProfilesSupported(std::vector<std::vector<std::string>>&& val)
{
	_profilesSupported = std::move(val);
}


inline void VideoEncoder2ConfigurationOptions::setQualityRange(const FloatRange& val)
{
	_qualityRange = val;
}


inline void VideoEncoder2ConfigurationOptions::setQualityRange(FloatRange&& val)
{
	_qualityRange = std::move(val);
}


inline void VideoEncoder2ConfigurationOptions::setResolutionsAvailable(const std::vector<VideoResolution2>& val)
{
	_resolutionsAvailable = val;
}


inline void VideoEncoder2ConfigurationOptions::setResolutionsAvailable(std::vector<VideoResolution2>&& val)
{
	_resolutionsAvailable = std::move(val);
}


} // ONVIF


#endif // ONVIF_VideoEncoder2ConfigurationOptions_INCLUDED
