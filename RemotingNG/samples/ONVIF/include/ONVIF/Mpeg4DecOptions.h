// 
// Mpeg4DecOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Mpeg4DecOptions_INCLUDED
#define ONVIF_Mpeg4DecOptions_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Mpeg4DecOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Mpeg4DecOptions
{
public:
	Mpeg4DecOptions();

	Mpeg4DecOptions(
		const std::vector<VideoResolution>& resolutionsAvailable, 
		const std::vector<std::string>& supportedMpeg4Profiles, 
		const IntRange& supportedInputBitrate, 
		const IntRange& supportedFrameRate);

	virtual ~Mpeg4DecOptions();

	const std::vector<VideoResolution>& getResolutionsAvailable() const;

	std::vector<VideoResolution>& getResolutionsAvailable();

	const IntRange& getSupportedFrameRate() const;

	const IntRange& getSupportedInputBitrate() const;

	const std::vector<std::string>& getSupportedMpeg4Profiles() const;

	std::vector<std::string>& getSupportedMpeg4Profiles();

	void setResolutionsAvailable(const std::vector<VideoResolution>& val);

	void setResolutionsAvailable(std::vector<VideoResolution>&& val);

	void setSupportedFrameRate(const IntRange& val);

	void setSupportedFrameRate(IntRange&& val);

	void setSupportedInputBitrate(const IntRange& val);

	void setSupportedInputBitrate(IntRange&& val);

	void setSupportedMpeg4Profiles(const std::vector<std::string>& val);

	void setSupportedMpeg4Profiles(std::vector<std::string>&& val);

private:
	//@ name=ResolutionsAvailable
	//@ order=0
	std::vector<VideoResolution> _resolutionsAvailable;

	//@ name=SupportedMpeg4Profiles
	//@ order=1
	std::vector<std::string> _supportedMpeg4Profiles;

	//@ name=SupportedInputBitrate
	//@ order=2
	IntRange _supportedInputBitrate;

	//@ name=SupportedFrameRate
	//@ order=3
	IntRange _supportedFrameRate;

};


inline const std::vector<VideoResolution>& Mpeg4DecOptions::getResolutionsAvailable() const
{
	return _resolutionsAvailable;
}


inline std::vector<VideoResolution>& Mpeg4DecOptions::getResolutionsAvailable()
{
	return _resolutionsAvailable;
}


inline const IntRange& Mpeg4DecOptions::getSupportedFrameRate() const
{
	return _supportedFrameRate;
}


inline const IntRange& Mpeg4DecOptions::getSupportedInputBitrate() const
{
	return _supportedInputBitrate;
}


inline const std::vector<std::string>& Mpeg4DecOptions::getSupportedMpeg4Profiles() const
{
	return _supportedMpeg4Profiles;
}


inline std::vector<std::string>& Mpeg4DecOptions::getSupportedMpeg4Profiles()
{
	return _supportedMpeg4Profiles;
}


inline void Mpeg4DecOptions::setResolutionsAvailable(const std::vector<VideoResolution>& val)
{
	_resolutionsAvailable = val;
}


inline void Mpeg4DecOptions::setResolutionsAvailable(std::vector<VideoResolution>&& val)
{
	_resolutionsAvailable = std::move(val);
}


inline void Mpeg4DecOptions::setSupportedFrameRate(const IntRange& val)
{
	_supportedFrameRate = val;
}


inline void Mpeg4DecOptions::setSupportedFrameRate(IntRange&& val)
{
	_supportedFrameRate = std::move(val);
}


inline void Mpeg4DecOptions::setSupportedInputBitrate(const IntRange& val)
{
	_supportedInputBitrate = val;
}


inline void Mpeg4DecOptions::setSupportedInputBitrate(IntRange&& val)
{
	_supportedInputBitrate = std::move(val);
}


inline void Mpeg4DecOptions::setSupportedMpeg4Profiles(const std::vector<std::string>& val)
{
	_supportedMpeg4Profiles = val;
}


inline void Mpeg4DecOptions::setSupportedMpeg4Profiles(std::vector<std::string>&& val)
{
	_supportedMpeg4Profiles = std::move(val);
}


} // ONVIF


#endif // ONVIF_Mpeg4DecOptions_INCLUDED
