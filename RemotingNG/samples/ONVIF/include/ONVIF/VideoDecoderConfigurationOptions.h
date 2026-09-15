// 
// VideoDecoderConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoDecoderConfigurationOptions_INCLUDED
#define ONVIF_VideoDecoderConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class H264DecOptions;
class JpegDecOptions;
class Mpeg4DecOptions;
class VideoDecoderConfigurationOptionsExtension;
} 


namespace ONVIF {


//@ name=VideoDecoderConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoDecoderConfigurationOptions
{
public:
	VideoDecoderConfigurationOptions();

	VideoDecoderConfigurationOptions(
		const Poco::SharedPtr<JpegDecOptions>& jpegDecOptions, 
		const Poco::SharedPtr<H264DecOptions>& h264DecOptions, 
		const Poco::SharedPtr<Mpeg4DecOptions>& mpeg4DecOptions, 
		const Poco::SharedPtr<VideoDecoderConfigurationOptionsExtension>& extension);

	virtual ~VideoDecoderConfigurationOptions();

	const Poco::SharedPtr<VideoDecoderConfigurationOptionsExtension>& getExtension() const;

	const Poco::SharedPtr<H264DecOptions>& getH264DecOptions() const;

	const Poco::SharedPtr<JpegDecOptions>& getJpegDecOptions() const;

	const Poco::SharedPtr<Mpeg4DecOptions>& getMpeg4DecOptions() const;

	void setExtension(const Poco::SharedPtr<VideoDecoderConfigurationOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<VideoDecoderConfigurationOptionsExtension>&& val);

	void setH264DecOptions(const Poco::SharedPtr<H264DecOptions>& val);

	void setH264DecOptions(Poco::SharedPtr<H264DecOptions>&& val);

	void setJpegDecOptions(const Poco::SharedPtr<JpegDecOptions>& val);

	void setJpegDecOptions(Poco::SharedPtr<JpegDecOptions>&& val);

	void setMpeg4DecOptions(const Poco::SharedPtr<Mpeg4DecOptions>& val);

	void setMpeg4DecOptions(Poco::SharedPtr<Mpeg4DecOptions>&& val);

private:
	//@ mandatory=false
	//@ name=JpegDecOptions
	//@ order=0
	Poco::SharedPtr<JpegDecOptions> _jpegDecOptions;

	//@ mandatory=false
	//@ name=H264DecOptions
	//@ order=1
	Poco::SharedPtr<H264DecOptions> _h264DecOptions;

	//@ mandatory=false
	//@ name=Mpeg4DecOptions
	//@ order=2
	Poco::SharedPtr<Mpeg4DecOptions> _mpeg4DecOptions;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<VideoDecoderConfigurationOptionsExtension> _extension;

};


inline const Poco::SharedPtr<VideoDecoderConfigurationOptionsExtension>& VideoDecoderConfigurationOptions::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<H264DecOptions>& VideoDecoderConfigurationOptions::getH264DecOptions() const
{
	return _h264DecOptions;
}


inline const Poco::SharedPtr<JpegDecOptions>& VideoDecoderConfigurationOptions::getJpegDecOptions() const
{
	return _jpegDecOptions;
}


inline const Poco::SharedPtr<Mpeg4DecOptions>& VideoDecoderConfigurationOptions::getMpeg4DecOptions() const
{
	return _mpeg4DecOptions;
}


inline void VideoDecoderConfigurationOptions::setExtension(const Poco::SharedPtr<VideoDecoderConfigurationOptionsExtension>& val)
{
	_extension = val;
}


inline void VideoDecoderConfigurationOptions::setExtension(Poco::SharedPtr<VideoDecoderConfigurationOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void VideoDecoderConfigurationOptions::setH264DecOptions(const Poco::SharedPtr<H264DecOptions>& val)
{
	_h264DecOptions = val;
}


inline void VideoDecoderConfigurationOptions::setH264DecOptions(Poco::SharedPtr<H264DecOptions>&& val)
{
	_h264DecOptions = std::move(val);
}


inline void VideoDecoderConfigurationOptions::setJpegDecOptions(const Poco::SharedPtr<JpegDecOptions>& val)
{
	_jpegDecOptions = val;
}


inline void VideoDecoderConfigurationOptions::setJpegDecOptions(Poco::SharedPtr<JpegDecOptions>&& val)
{
	_jpegDecOptions = std::move(val);
}


inline void VideoDecoderConfigurationOptions::setMpeg4DecOptions(const Poco::SharedPtr<Mpeg4DecOptions>& val)
{
	_mpeg4DecOptions = val;
}


inline void VideoDecoderConfigurationOptions::setMpeg4DecOptions(Poco::SharedPtr<Mpeg4DecOptions>&& val)
{
	_mpeg4DecOptions = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/H264DecOptions.h"
#include "ONVIF/JpegDecOptions.h"
#include "ONVIF/Mpeg4DecOptions.h"
#include "ONVIF/VideoDecoderConfigurationOptionsExtension.h"


#endif // ONVIF_VideoDecoderConfigurationOptions_INCLUDED
