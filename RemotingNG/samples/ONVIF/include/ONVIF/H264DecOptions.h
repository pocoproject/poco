// 
// H264DecOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_H264DecOptions_INCLUDED
#define ONVIF_H264DecOptions_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=H264DecOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API H264DecOptions
{
public:
	H264DecOptions();

	H264DecOptions(
		const std::vector<VideoResolution>& resolutionsAvailable, 
		const std::vector<std::string>& supportedH264Profiles, 
		const IntRange& supportedInputBitrate, 
		const IntRange& supportedFrameRate);

	virtual ~H264DecOptions();

	const std::vector<VideoResolution>& getResolutionsAvailable() const;

	std::vector<VideoResolution>& getResolutionsAvailable();

	const IntRange& getSupportedFrameRate() const;

	const std::vector<std::string>& getSupportedH264Profiles() const;

	std::vector<std::string>& getSupportedH264Profiles();

	const IntRange& getSupportedInputBitrate() const;

	void setResolutionsAvailable(const std::vector<VideoResolution>& val);

	void setResolutionsAvailable(std::vector<VideoResolution>&& val);

	void setSupportedFrameRate(const IntRange& val);

	void setSupportedFrameRate(IntRange&& val);

	void setSupportedH264Profiles(const std::vector<std::string>& val);

	void setSupportedH264Profiles(std::vector<std::string>&& val);

	void setSupportedInputBitrate(const IntRange& val);

	void setSupportedInputBitrate(IntRange&& val);

private:
	//@ name=ResolutionsAvailable
	//@ order=0
	std::vector<VideoResolution> _resolutionsAvailable;

	//@ name=SupportedH264Profiles
	//@ order=1
	std::vector<std::string> _supportedH264Profiles;

	//@ name=SupportedInputBitrate
	//@ order=2
	IntRange _supportedInputBitrate;

	//@ name=SupportedFrameRate
	//@ order=3
	IntRange _supportedFrameRate;

};


inline const std::vector<VideoResolution>& H264DecOptions::getResolutionsAvailable() const
{
	return _resolutionsAvailable;
}


inline std::vector<VideoResolution>& H264DecOptions::getResolutionsAvailable()
{
	return _resolutionsAvailable;
}


inline const IntRange& H264DecOptions::getSupportedFrameRate() const
{
	return _supportedFrameRate;
}


inline const std::vector<std::string>& H264DecOptions::getSupportedH264Profiles() const
{
	return _supportedH264Profiles;
}


inline std::vector<std::string>& H264DecOptions::getSupportedH264Profiles()
{
	return _supportedH264Profiles;
}


inline const IntRange& H264DecOptions::getSupportedInputBitrate() const
{
	return _supportedInputBitrate;
}


inline void H264DecOptions::setResolutionsAvailable(const std::vector<VideoResolution>& val)
{
	_resolutionsAvailable = val;
}


inline void H264DecOptions::setResolutionsAvailable(std::vector<VideoResolution>&& val)
{
	_resolutionsAvailable = std::move(val);
}


inline void H264DecOptions::setSupportedFrameRate(const IntRange& val)
{
	_supportedFrameRate = val;
}


inline void H264DecOptions::setSupportedFrameRate(IntRange&& val)
{
	_supportedFrameRate = std::move(val);
}


inline void H264DecOptions::setSupportedH264Profiles(const std::vector<std::string>& val)
{
	_supportedH264Profiles = val;
}


inline void H264DecOptions::setSupportedH264Profiles(std::vector<std::string>&& val)
{
	_supportedH264Profiles = std::move(val);
}


inline void H264DecOptions::setSupportedInputBitrate(const IntRange& val)
{
	_supportedInputBitrate = val;
}


inline void H264DecOptions::setSupportedInputBitrate(IntRange&& val)
{
	_supportedInputBitrate = std::move(val);
}


} // ONVIF


#endif // ONVIF_H264DecOptions_INCLUDED
