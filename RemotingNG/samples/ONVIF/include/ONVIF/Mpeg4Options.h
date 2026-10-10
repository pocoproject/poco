// 
// Mpeg4Options.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Mpeg4Options_INCLUDED
#define ONVIF_Mpeg4Options_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Mpeg4Options
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Mpeg4Options
{
public:
	Mpeg4Options();

	Mpeg4Options(
		const std::vector<VideoResolution>& resolutionsAvailable, 
		const IntRange& govLengthRange, 
		const IntRange& frameRateRange, 
		const IntRange& encodingIntervalRange, 
		const std::vector<std::string>& mpeg4ProfilesSupported);

	virtual ~Mpeg4Options();

	const IntRange& getEncodingIntervalRange() const;

	const IntRange& getFrameRateRange() const;

	const IntRange& getGovLengthRange() const;

	const std::vector<std::string>& getMpeg4ProfilesSupported() const;

	std::vector<std::string>& getMpeg4ProfilesSupported();

	const std::vector<VideoResolution>& getResolutionsAvailable() const;

	std::vector<VideoResolution>& getResolutionsAvailable();

	void setEncodingIntervalRange(const IntRange& val);

	void setEncodingIntervalRange(IntRange&& val);

	void setFrameRateRange(const IntRange& val);

	void setFrameRateRange(IntRange&& val);

	void setGovLengthRange(const IntRange& val);

	void setGovLengthRange(IntRange&& val);

	void setMpeg4ProfilesSupported(const std::vector<std::string>& val);

	void setMpeg4ProfilesSupported(std::vector<std::string>&& val);

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

	//@ name=Mpeg4ProfilesSupported
	//@ order=4
	std::vector<std::string> _mpeg4ProfilesSupported;

};


inline const IntRange& Mpeg4Options::getEncodingIntervalRange() const
{
	return _encodingIntervalRange;
}


inline const IntRange& Mpeg4Options::getFrameRateRange() const
{
	return _frameRateRange;
}


inline const IntRange& Mpeg4Options::getGovLengthRange() const
{
	return _govLengthRange;
}


inline const std::vector<std::string>& Mpeg4Options::getMpeg4ProfilesSupported() const
{
	return _mpeg4ProfilesSupported;
}


inline std::vector<std::string>& Mpeg4Options::getMpeg4ProfilesSupported()
{
	return _mpeg4ProfilesSupported;
}


inline const std::vector<VideoResolution>& Mpeg4Options::getResolutionsAvailable() const
{
	return _resolutionsAvailable;
}


inline std::vector<VideoResolution>& Mpeg4Options::getResolutionsAvailable()
{
	return _resolutionsAvailable;
}


inline void Mpeg4Options::setEncodingIntervalRange(const IntRange& val)
{
	_encodingIntervalRange = val;
}


inline void Mpeg4Options::setEncodingIntervalRange(IntRange&& val)
{
	_encodingIntervalRange = std::move(val);
}


inline void Mpeg4Options::setFrameRateRange(const IntRange& val)
{
	_frameRateRange = val;
}


inline void Mpeg4Options::setFrameRateRange(IntRange&& val)
{
	_frameRateRange = std::move(val);
}


inline void Mpeg4Options::setGovLengthRange(const IntRange& val)
{
	_govLengthRange = val;
}


inline void Mpeg4Options::setGovLengthRange(IntRange&& val)
{
	_govLengthRange = std::move(val);
}


inline void Mpeg4Options::setMpeg4ProfilesSupported(const std::vector<std::string>& val)
{
	_mpeg4ProfilesSupported = val;
}


inline void Mpeg4Options::setMpeg4ProfilesSupported(std::vector<std::string>&& val)
{
	_mpeg4ProfilesSupported = std::move(val);
}


inline void Mpeg4Options::setResolutionsAvailable(const std::vector<VideoResolution>& val)
{
	_resolutionsAvailable = val;
}


inline void Mpeg4Options::setResolutionsAvailable(std::vector<VideoResolution>&& val)
{
	_resolutionsAvailable = std::move(val);
}


} // ONVIF


#endif // ONVIF_Mpeg4Options_INCLUDED
