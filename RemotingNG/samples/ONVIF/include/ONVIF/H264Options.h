// 
// H264Options.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_H264Options_INCLUDED
#define ONVIF_H264Options_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=H264Options
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API H264Options
{
public:
	H264Options();

	H264Options(
		const std::vector<VideoResolution>& resolutionsAvailable, 
		const IntRange& govLengthRange, 
		const IntRange& frameRateRange, 
		const IntRange& encodingIntervalRange, 
		const std::vector<std::string>& h264ProfilesSupported);

	virtual ~H264Options();

	const IntRange& getEncodingIntervalRange() const;

	const IntRange& getFrameRateRange() const;

	const IntRange& getGovLengthRange() const;

	const std::vector<std::string>& getH264ProfilesSupported() const;

	std::vector<std::string>& getH264ProfilesSupported();

	const std::vector<VideoResolution>& getResolutionsAvailable() const;

	std::vector<VideoResolution>& getResolutionsAvailable();

	void setEncodingIntervalRange(const IntRange& val);

	void setEncodingIntervalRange(IntRange&& val);

	void setFrameRateRange(const IntRange& val);

	void setFrameRateRange(IntRange&& val);

	void setGovLengthRange(const IntRange& val);

	void setGovLengthRange(IntRange&& val);

	void setH264ProfilesSupported(const std::vector<std::string>& val);

	void setH264ProfilesSupported(std::vector<std::string>&& val);

	void setResolutionsAvailable(const std::vector<VideoResolution>& val);

	void setResolutionsAvailable(std::vector<VideoResolution>&& val);

private:
	//@ name=ResolutionsAvailable
	//@ order=0
	std::vector<VideoResolution> _resolutionsAvailable;

	//@ name=GovLengthRange
	//@ order=1
	IntRange _govLengthRange;

	//@ name=FrameRateRange
	//@ order=2
	IntRange _frameRateRange;

	//@ name=EncodingIntervalRange
	//@ order=3
	IntRange _encodingIntervalRange;

	//@ name=H264ProfilesSupported
	//@ order=4
	std::vector<std::string> _h264ProfilesSupported;

};


inline const IntRange& H264Options::getEncodingIntervalRange() const
{
	return _encodingIntervalRange;
}


inline const IntRange& H264Options::getFrameRateRange() const
{
	return _frameRateRange;
}


inline const IntRange& H264Options::getGovLengthRange() const
{
	return _govLengthRange;
}


inline const std::vector<std::string>& H264Options::getH264ProfilesSupported() const
{
	return _h264ProfilesSupported;
}


inline std::vector<std::string>& H264Options::getH264ProfilesSupported()
{
	return _h264ProfilesSupported;
}


inline const std::vector<VideoResolution>& H264Options::getResolutionsAvailable() const
{
	return _resolutionsAvailable;
}


inline std::vector<VideoResolution>& H264Options::getResolutionsAvailable()
{
	return _resolutionsAvailable;
}


inline void H264Options::setEncodingIntervalRange(const IntRange& val)
{
	_encodingIntervalRange = val;
}


inline void H264Options::setEncodingIntervalRange(IntRange&& val)
{
	_encodingIntervalRange = std::move(val);
}


inline void H264Options::setFrameRateRange(const IntRange& val)
{
	_frameRateRange = val;
}


inline void H264Options::setFrameRateRange(IntRange&& val)
{
	_frameRateRange = std::move(val);
}


inline void H264Options::setGovLengthRange(const IntRange& val)
{
	_govLengthRange = val;
}


inline void H264Options::setGovLengthRange(IntRange&& val)
{
	_govLengthRange = std::move(val);
}


inline void H264Options::setH264ProfilesSupported(const std::vector<std::string>& val)
{
	_h264ProfilesSupported = val;
}


inline void H264Options::setH264ProfilesSupported(std::vector<std::string>&& val)
{
	_h264ProfilesSupported = std::move(val);
}


inline void H264Options::setResolutionsAvailable(const std::vector<VideoResolution>& val)
{
	_resolutionsAvailable = val;
}


inline void H264Options::setResolutionsAvailable(std::vector<VideoResolution>&& val)
{
	_resolutionsAvailable = std::move(val);
}


} // ONVIF


#endif // ONVIF_H264Options_INCLUDED
