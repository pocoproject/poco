// 
// JpegDecOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_JpegDecOptions_INCLUDED
#define ONVIF_JpegDecOptions_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=JpegDecOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API JpegDecOptions
{
public:
	JpegDecOptions();

	JpegDecOptions(
		const std::vector<VideoResolution>& resolutionsAvailable, 
		const IntRange& supportedInputBitrate, 
		const IntRange& supportedFrameRate);

	virtual ~JpegDecOptions();

	const std::vector<VideoResolution>& getResolutionsAvailable() const;

	std::vector<VideoResolution>& getResolutionsAvailable();

	const IntRange& getSupportedFrameRate() const;

	const IntRange& getSupportedInputBitrate() const;

	void setResolutionsAvailable(const std::vector<VideoResolution>& val);

	void setResolutionsAvailable(std::vector<VideoResolution>&& val);

	void setSupportedFrameRate(const IntRange& val);

	void setSupportedFrameRate(IntRange&& val);

	void setSupportedInputBitrate(const IntRange& val);

	void setSupportedInputBitrate(IntRange&& val);

private:
	//@ name=ResolutionsAvailable
	//@ order=0
	std::vector<VideoResolution> _resolutionsAvailable;

	//@ name=SupportedInputBitrate
	//@ order=1
	IntRange _supportedInputBitrate;

	//@ name=SupportedFrameRate
	//@ order=2
	IntRange _supportedFrameRate;

};


inline const std::vector<VideoResolution>& JpegDecOptions::getResolutionsAvailable() const
{
	return _resolutionsAvailable;
}


inline std::vector<VideoResolution>& JpegDecOptions::getResolutionsAvailable()
{
	return _resolutionsAvailable;
}


inline const IntRange& JpegDecOptions::getSupportedFrameRate() const
{
	return _supportedFrameRate;
}


inline const IntRange& JpegDecOptions::getSupportedInputBitrate() const
{
	return _supportedInputBitrate;
}


inline void JpegDecOptions::setResolutionsAvailable(const std::vector<VideoResolution>& val)
{
	_resolutionsAvailable = val;
}


inline void JpegDecOptions::setResolutionsAvailable(std::vector<VideoResolution>&& val)
{
	_resolutionsAvailable = std::move(val);
}


inline void JpegDecOptions::setSupportedFrameRate(const IntRange& val)
{
	_supportedFrameRate = val;
}


inline void JpegDecOptions::setSupportedFrameRate(IntRange&& val)
{
	_supportedFrameRate = std::move(val);
}


inline void JpegDecOptions::setSupportedInputBitrate(const IntRange& val)
{
	_supportedInputBitrate = val;
}


inline void JpegDecOptions::setSupportedInputBitrate(IntRange&& val)
{
	_supportedInputBitrate = std::move(val);
}


} // ONVIF


#endif // ONVIF_JpegDecOptions_INCLUDED
