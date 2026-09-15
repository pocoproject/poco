// 
// VideoEncoderConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoEncoderConfiguration_INCLUDED
#define ONVIF_VideoEncoderConfiguration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/MulticastConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class H264Configuration;
class Mpeg4Configuration;
class VideoRateControl;
} 


namespace ONVIF {


//@ name=VideoEncoderConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoEncoderConfiguration: public ConfigurationEntity
{
public:
	VideoEncoderConfiguration();

	VideoEncoderConfiguration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		Poco::Optional<bool> guaranteedFrameRate, 
		const std::string& encoding, 
		const VideoResolution& resolution, 
		float quality, 
		const Poco::SharedPtr<VideoRateControl>& rateControl, 
		const Poco::SharedPtr<Mpeg4Configuration>& mPEG4, 
		const Poco::SharedPtr<H264Configuration>& h264, 
		const MulticastConfiguration& multicast, 
		const std::string& sessionTimeout);

	virtual ~VideoEncoderConfiguration();

	const std::string& getEncoding() const;

	Poco::Optional<bool> getGuaranteedFrameRate() const;

	const Poco::SharedPtr<H264Configuration>& getH264() const;

	const Poco::SharedPtr<Mpeg4Configuration>& getMPEG4() const;

	const MulticastConfiguration& getMulticast() const;

	float getQuality() const;

	const Poco::SharedPtr<VideoRateControl>& getRateControl() const;

	const VideoResolution& getResolution() const;

	const std::string& getSessionTimeout() const;

	void setEncoding(const std::string& val);

	void setEncoding(std::string&& val);

	void setGuaranteedFrameRate(Poco::Optional<bool> val);

	void setH264(const Poco::SharedPtr<H264Configuration>& val);

	void setH264(Poco::SharedPtr<H264Configuration>&& val);

	void setMPEG4(const Poco::SharedPtr<Mpeg4Configuration>& val);

	void setMPEG4(Poco::SharedPtr<Mpeg4Configuration>&& val);

	void setMulticast(const MulticastConfiguration& val);

	void setMulticast(MulticastConfiguration&& val);

	void setQuality(float val);

	void setRateControl(const Poco::SharedPtr<VideoRateControl>& val);

	void setRateControl(Poco::SharedPtr<VideoRateControl>&& val);

	void setResolution(const VideoResolution& val);

	void setResolution(VideoResolution&& val);

	void setSessionTimeout(const std::string& val);

	void setSessionTimeout(std::string&& val);

private:
	//@ mandatory=false
	//@ name=GuaranteedFrameRate
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _guaranteedFrameRate;

	//@ name=Encoding
	//@ order=1
	std::string _encoding;

	//@ name=Resolution
	//@ order=2
	VideoResolution _resolution;

	//@ name=Quality
	//@ order=3
	float _quality;

	//@ mandatory=false
	//@ name=RateControl
	//@ order=4
	Poco::SharedPtr<VideoRateControl> _rateControl;

	//@ mandatory=false
	//@ name=MPEG4
	//@ order=5
	Poco::SharedPtr<Mpeg4Configuration> _mPEG4;

	//@ mandatory=false
	//@ name=H264
	//@ order=6
	Poco::SharedPtr<H264Configuration> _h264;

	//@ name=Multicast
	//@ order=7
	MulticastConfiguration _multicast;

	//@ name=SessionTimeout
	//@ order=8
	std::string _sessionTimeout;

};


inline const std::string& VideoEncoderConfiguration::getEncoding() const
{
	return _encoding;
}


inline Poco::Optional<bool> VideoEncoderConfiguration::getGuaranteedFrameRate() const
{
	return _guaranteedFrameRate;
}


inline const Poco::SharedPtr<H264Configuration>& VideoEncoderConfiguration::getH264() const
{
	return _h264;
}


inline const Poco::SharedPtr<Mpeg4Configuration>& VideoEncoderConfiguration::getMPEG4() const
{
	return _mPEG4;
}


inline const MulticastConfiguration& VideoEncoderConfiguration::getMulticast() const
{
	return _multicast;
}


inline float VideoEncoderConfiguration::getQuality() const
{
	return _quality;
}


inline const Poco::SharedPtr<VideoRateControl>& VideoEncoderConfiguration::getRateControl() const
{
	return _rateControl;
}


inline const VideoResolution& VideoEncoderConfiguration::getResolution() const
{
	return _resolution;
}


inline const std::string& VideoEncoderConfiguration::getSessionTimeout() const
{
	return _sessionTimeout;
}


inline void VideoEncoderConfiguration::setEncoding(const std::string& val)
{
	_encoding = val;
}


inline void VideoEncoderConfiguration::setEncoding(std::string&& val)
{
	_encoding = std::move(val);
}


inline void VideoEncoderConfiguration::setGuaranteedFrameRate(Poco::Optional<bool> val)
{
	_guaranteedFrameRate = val;
}


inline void VideoEncoderConfiguration::setH264(const Poco::SharedPtr<H264Configuration>& val)
{
	_h264 = val;
}


inline void VideoEncoderConfiguration::setH264(Poco::SharedPtr<H264Configuration>&& val)
{
	_h264 = std::move(val);
}


inline void VideoEncoderConfiguration::setMPEG4(const Poco::SharedPtr<Mpeg4Configuration>& val)
{
	_mPEG4 = val;
}


inline void VideoEncoderConfiguration::setMPEG4(Poco::SharedPtr<Mpeg4Configuration>&& val)
{
	_mPEG4 = std::move(val);
}


inline void VideoEncoderConfiguration::setMulticast(const MulticastConfiguration& val)
{
	_multicast = val;
}


inline void VideoEncoderConfiguration::setMulticast(MulticastConfiguration&& val)
{
	_multicast = std::move(val);
}


inline void VideoEncoderConfiguration::setQuality(float val)
{
	_quality = val;
}


inline void VideoEncoderConfiguration::setRateControl(const Poco::SharedPtr<VideoRateControl>& val)
{
	_rateControl = val;
}


inline void VideoEncoderConfiguration::setRateControl(Poco::SharedPtr<VideoRateControl>&& val)
{
	_rateControl = std::move(val);
}


inline void VideoEncoderConfiguration::setResolution(const VideoResolution& val)
{
	_resolution = val;
}


inline void VideoEncoderConfiguration::setResolution(VideoResolution&& val)
{
	_resolution = std::move(val);
}


inline void VideoEncoderConfiguration::setSessionTimeout(const std::string& val)
{
	_sessionTimeout = val;
}


inline void VideoEncoderConfiguration::setSessionTimeout(std::string&& val)
{
	_sessionTimeout = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/H264Configuration.h"
#include "ONVIF/Mpeg4Configuration.h"
#include "ONVIF/VideoRateControl.h"


#endif // ONVIF_VideoEncoderConfiguration_INCLUDED
