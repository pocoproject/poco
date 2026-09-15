// 
// VideoEncoder2Configuration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoEncoder2Configuration_INCLUDED
#define ONVIF_VideoEncoder2Configuration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution2.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class MulticastConfiguration;
class VideoRateControl2;
} 


namespace ONVIF {


//@ name=VideoEncoder2Configuration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoEncoder2Configuration: public ConfigurationEntity
{
public:
	VideoEncoder2Configuration();

	VideoEncoder2Configuration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		Poco::Optional<int> govLength, 
		const Poco::Optional<std::string>& profile, 
		Poco::Optional<bool> guaranteedFrameRate, 
		const std::string& encoding, 
		const VideoResolution2& resolution, 
		const Poco::SharedPtr<VideoRateControl2>& rateControl, 
		const Poco::SharedPtr<MulticastConfiguration>& multicast, 
		float quality);

	virtual ~VideoEncoder2Configuration();

	const std::string& getEncoding() const;

	Poco::Optional<int> getGovLength() const;

	Poco::Optional<bool> getGuaranteedFrameRate() const;

	const Poco::SharedPtr<MulticastConfiguration>& getMulticast() const;

	const Poco::Optional<std::string>& getProfile() const;

	float getQuality() const;

	const Poco::SharedPtr<VideoRateControl2>& getRateControl() const;

	const VideoResolution2& getResolution() const;

	void setEncoding(const std::string& val);

	void setEncoding(std::string&& val);

	void setGovLength(Poco::Optional<int> val);

	void setGuaranteedFrameRate(Poco::Optional<bool> val);

	void setMulticast(const Poco::SharedPtr<MulticastConfiguration>& val);

	void setMulticast(Poco::SharedPtr<MulticastConfiguration>&& val);

	void setProfile(const Poco::Optional<std::string>& val);

	void setProfile(Poco::Optional<std::string>&& val);

	void setQuality(float val);

	void setRateControl(const Poco::SharedPtr<VideoRateControl2>& val);

	void setRateControl(Poco::SharedPtr<VideoRateControl2>&& val);

	void setResolution(const VideoResolution2& val);

	void setResolution(VideoResolution2&& val);

private:
	//@ mandatory=false
	//@ name=GovLength
	//@ order=0
	//@ type=attr
	Poco::Optional<int> _govLength;

	//@ mandatory=false
	//@ name=Profile
	//@ order=1
	//@ type=attr
	Poco::Optional<std::string> _profile;

	//@ mandatory=false
	//@ name=GuaranteedFrameRate
	//@ order=2
	//@ type=attr
	Poco::Optional<bool> _guaranteedFrameRate;

	//@ name=Encoding
	//@ order=3
	std::string _encoding;

	//@ name=Resolution
	//@ order=4
	VideoResolution2 _resolution;

	//@ mandatory=false
	//@ name=RateControl
	//@ order=5
	Poco::SharedPtr<VideoRateControl2> _rateControl;

	//@ mandatory=false
	//@ name=Multicast
	//@ order=6
	Poco::SharedPtr<MulticastConfiguration> _multicast;

	//@ name=Quality
	//@ order=7
	float _quality;

};


inline const std::string& VideoEncoder2Configuration::getEncoding() const
{
	return _encoding;
}


inline Poco::Optional<int> VideoEncoder2Configuration::getGovLength() const
{
	return _govLength;
}


inline Poco::Optional<bool> VideoEncoder2Configuration::getGuaranteedFrameRate() const
{
	return _guaranteedFrameRate;
}


inline const Poco::SharedPtr<MulticastConfiguration>& VideoEncoder2Configuration::getMulticast() const
{
	return _multicast;
}


inline const Poco::Optional<std::string>& VideoEncoder2Configuration::getProfile() const
{
	return _profile;
}


inline float VideoEncoder2Configuration::getQuality() const
{
	return _quality;
}


inline const Poco::SharedPtr<VideoRateControl2>& VideoEncoder2Configuration::getRateControl() const
{
	return _rateControl;
}


inline const VideoResolution2& VideoEncoder2Configuration::getResolution() const
{
	return _resolution;
}


inline void VideoEncoder2Configuration::setEncoding(const std::string& val)
{
	_encoding = val;
}


inline void VideoEncoder2Configuration::setEncoding(std::string&& val)
{
	_encoding = std::move(val);
}


inline void VideoEncoder2Configuration::setGovLength(Poco::Optional<int> val)
{
	_govLength = val;
}


inline void VideoEncoder2Configuration::setGuaranteedFrameRate(Poco::Optional<bool> val)
{
	_guaranteedFrameRate = val;
}


inline void VideoEncoder2Configuration::setMulticast(const Poco::SharedPtr<MulticastConfiguration>& val)
{
	_multicast = val;
}


inline void VideoEncoder2Configuration::setMulticast(Poco::SharedPtr<MulticastConfiguration>&& val)
{
	_multicast = std::move(val);
}


inline void VideoEncoder2Configuration::setProfile(const Poco::Optional<std::string>& val)
{
	_profile = val;
}


inline void VideoEncoder2Configuration::setProfile(Poco::Optional<std::string>&& val)
{
	_profile = std::move(val);
}


inline void VideoEncoder2Configuration::setQuality(float val)
{
	_quality = val;
}


inline void VideoEncoder2Configuration::setRateControl(const Poco::SharedPtr<VideoRateControl2>& val)
{
	_rateControl = val;
}


inline void VideoEncoder2Configuration::setRateControl(Poco::SharedPtr<VideoRateControl2>&& val)
{
	_rateControl = std::move(val);
}


inline void VideoEncoder2Configuration::setResolution(const VideoResolution2& val)
{
	_resolution = val;
}


inline void VideoEncoder2Configuration::setResolution(VideoResolution2&& val)
{
	_resolution = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/MulticastConfiguration.h"
#include "ONVIF/VideoRateControl2.h"


#endif // ONVIF_VideoEncoder2Configuration_INCLUDED
