// 
// VideoEncoderConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoEncoderConfigurationOptions_INCLUDED
#define ONVIF_VideoEncoderConfigurationOptions_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class H264Options;
class JpegOptions;
class Mpeg4Options;
class VideoEncoderOptionsExtension;
} 


namespace ONVIF {


//@ name=VideoEncoderConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoEncoderConfigurationOptions
{
public:
	VideoEncoderConfigurationOptions();

	VideoEncoderConfigurationOptions(
		Poco::Optional<bool> guaranteedFrameRateSupported, 
		const IntRange& qualityRange, 
		const Poco::SharedPtr<JpegOptions>& jPEG, 
		const Poco::SharedPtr<Mpeg4Options>& mPEG4, 
		const Poco::SharedPtr<H264Options>& h264, 
		const Poco::SharedPtr<VideoEncoderOptionsExtension>& extension);

	virtual ~VideoEncoderConfigurationOptions();

	const Poco::SharedPtr<VideoEncoderOptionsExtension>& getExtension() const;

	Poco::Optional<bool> getGuaranteedFrameRateSupported() const;

	const Poco::SharedPtr<H264Options>& getH264() const;

	const Poco::SharedPtr<JpegOptions>& getJPEG() const;

	const Poco::SharedPtr<Mpeg4Options>& getMPEG4() const;

	const IntRange& getQualityRange() const;

	void setExtension(const Poco::SharedPtr<VideoEncoderOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<VideoEncoderOptionsExtension>&& val);

	void setGuaranteedFrameRateSupported(Poco::Optional<bool> val);

	void setH264(const Poco::SharedPtr<H264Options>& val);

	void setH264(Poco::SharedPtr<H264Options>&& val);

	void setJPEG(const Poco::SharedPtr<JpegOptions>& val);

	void setJPEG(Poco::SharedPtr<JpegOptions>&& val);

	void setMPEG4(const Poco::SharedPtr<Mpeg4Options>& val);

	void setMPEG4(Poco::SharedPtr<Mpeg4Options>&& val);

	void setQualityRange(const IntRange& val);

	void setQualityRange(IntRange&& val);

private:
	//@ mandatory=false
	//@ name=GuaranteedFrameRateSupported
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _guaranteedFrameRateSupported;

	//@ name=QualityRange
	//@ order=1
	IntRange _qualityRange;

	//@ mandatory=false
	//@ name=JPEG
	//@ order=2
	Poco::SharedPtr<JpegOptions> _jPEG;

	//@ mandatory=false
	//@ name=MPEG4
	//@ order=3
	Poco::SharedPtr<Mpeg4Options> _mPEG4;

	//@ mandatory=false
	//@ name=H264
	//@ order=4
	Poco::SharedPtr<H264Options> _h264;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<VideoEncoderOptionsExtension> _extension;

};


inline const Poco::SharedPtr<VideoEncoderOptionsExtension>& VideoEncoderConfigurationOptions::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> VideoEncoderConfigurationOptions::getGuaranteedFrameRateSupported() const
{
	return _guaranteedFrameRateSupported;
}


inline const Poco::SharedPtr<H264Options>& VideoEncoderConfigurationOptions::getH264() const
{
	return _h264;
}


inline const Poco::SharedPtr<JpegOptions>& VideoEncoderConfigurationOptions::getJPEG() const
{
	return _jPEG;
}


inline const Poco::SharedPtr<Mpeg4Options>& VideoEncoderConfigurationOptions::getMPEG4() const
{
	return _mPEG4;
}


inline const IntRange& VideoEncoderConfigurationOptions::getQualityRange() const
{
	return _qualityRange;
}


inline void VideoEncoderConfigurationOptions::setExtension(const Poco::SharedPtr<VideoEncoderOptionsExtension>& val)
{
	_extension = val;
}


inline void VideoEncoderConfigurationOptions::setExtension(Poco::SharedPtr<VideoEncoderOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void VideoEncoderConfigurationOptions::setGuaranteedFrameRateSupported(Poco::Optional<bool> val)
{
	_guaranteedFrameRateSupported = val;
}


inline void VideoEncoderConfigurationOptions::setH264(const Poco::SharedPtr<H264Options>& val)
{
	_h264 = val;
}


inline void VideoEncoderConfigurationOptions::setH264(Poco::SharedPtr<H264Options>&& val)
{
	_h264 = std::move(val);
}


inline void VideoEncoderConfigurationOptions::setJPEG(const Poco::SharedPtr<JpegOptions>& val)
{
	_jPEG = val;
}


inline void VideoEncoderConfigurationOptions::setJPEG(Poco::SharedPtr<JpegOptions>&& val)
{
	_jPEG = std::move(val);
}


inline void VideoEncoderConfigurationOptions::setMPEG4(const Poco::SharedPtr<Mpeg4Options>& val)
{
	_mPEG4 = val;
}


inline void VideoEncoderConfigurationOptions::setMPEG4(Poco::SharedPtr<Mpeg4Options>&& val)
{
	_mPEG4 = std::move(val);
}


inline void VideoEncoderConfigurationOptions::setQualityRange(const IntRange& val)
{
	_qualityRange = val;
}


inline void VideoEncoderConfigurationOptions::setQualityRange(IntRange&& val)
{
	_qualityRange = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/H264Options.h"
#include "ONVIF/JpegOptions.h"
#include "ONVIF/Mpeg4Options.h"
#include "ONVIF/VideoEncoderOptionsExtension.h"


#endif // ONVIF_VideoEncoderConfigurationOptions_INCLUDED
