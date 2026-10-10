// 
// JpegOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_JpegOptions_INCLUDED
#define ONVIF_JpegOptions_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=JpegOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API JpegOptions
{
public:
	JpegOptions();

	JpegOptions(
		const std::vector<VideoResolution>& resolutionsAvailable, 
		const IntRange& frameRateRange, 
		const IntRange& encodingIntervalRange);

	virtual ~JpegOptions();

	const IntRange& getEncodingIntervalRange() const;

	const IntRange& getFrameRateRange() const;

	const std::vector<VideoResolution>& getResolutionsAvailable() const;

	std::vector<VideoResolution>& getResolutionsAvailable();

	void setEncodingIntervalRange(const IntRange& val);

	void setEncodingIntervalRange(IntRange&& val);

	void setFrameRateRange(const IntRange& val);

	void setFrameRateRange(IntRange&& val);

	void setResolutionsAvailable(const std::vector<VideoResolution>& val);

	void setResolutionsAvailable(std::vector<VideoResolution>&& val);

private:
	//@ name=ResolutionsAvailable
	//@ order=0
	std::vector<VideoResolution> _resolutionsAvailable;

	//@ name=FrameRateRange
	//@ order=1
	IntRange _frameRateRange;

	//@ name=EncodingIntervalRange
	//@ order=2
	IntRange _encodingIntervalRange;

};


inline const IntRange& JpegOptions::getEncodingIntervalRange() const
{
	return _encodingIntervalRange;
}


inline const IntRange& JpegOptions::getFrameRateRange() const
{
	return _frameRateRange;
}


inline const std::vector<VideoResolution>& JpegOptions::getResolutionsAvailable() const
{
	return _resolutionsAvailable;
}


inline std::vector<VideoResolution>& JpegOptions::getResolutionsAvailable()
{
	return _resolutionsAvailable;
}


inline void JpegOptions::setEncodingIntervalRange(const IntRange& val)
{
	_encodingIntervalRange = val;
}


inline void JpegOptions::setEncodingIntervalRange(IntRange&& val)
{
	_encodingIntervalRange = std::move(val);
}


inline void JpegOptions::setFrameRateRange(const IntRange& val)
{
	_frameRateRange = val;
}


inline void JpegOptions::setFrameRateRange(IntRange&& val)
{
	_frameRateRange = std::move(val);
}


inline void JpegOptions::setResolutionsAvailable(const std::vector<VideoResolution>& val)
{
	_resolutionsAvailable = val;
}


inline void JpegOptions::setResolutionsAvailable(std::vector<VideoResolution>&& val)
{
	_resolutionsAvailable = std::move(val);
}


} // ONVIF


#endif // ONVIF_JpegOptions_INCLUDED
