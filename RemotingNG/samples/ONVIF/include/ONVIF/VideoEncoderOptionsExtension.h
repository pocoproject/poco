// 
// VideoEncoderOptionsExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoEncoderOptionsExtension_INCLUDED
#define ONVIF_VideoEncoderOptionsExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class H264Options2;
class JpegOptions2;
class Mpeg4Options2;
class VideoEncoderOptionsExtension2;
} 


namespace ONVIF {


//@ name=VideoEncoderOptionsExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoEncoderOptionsExtension
{
public:
	VideoEncoderOptionsExtension();

	VideoEncoderOptionsExtension(
		const Poco::SharedPtr<JpegOptions2>& jPEG, 
		const Poco::SharedPtr<Mpeg4Options2>& mPEG4, 
		const Poco::SharedPtr<H264Options2>& h264, 
		const Poco::SharedPtr<VideoEncoderOptionsExtension2>& extension);

	virtual ~VideoEncoderOptionsExtension();

	const Poco::SharedPtr<VideoEncoderOptionsExtension2>& getExtension() const;

	const Poco::SharedPtr<H264Options2>& getH264() const;

	const Poco::SharedPtr<JpegOptions2>& getJPEG() const;

	const Poco::SharedPtr<Mpeg4Options2>& getMPEG4() const;

	void setExtension(const Poco::SharedPtr<VideoEncoderOptionsExtension2>& val);

	void setExtension(Poco::SharedPtr<VideoEncoderOptionsExtension2>&& val);

	void setH264(const Poco::SharedPtr<H264Options2>& val);

	void setH264(Poco::SharedPtr<H264Options2>&& val);

	void setJPEG(const Poco::SharedPtr<JpegOptions2>& val);

	void setJPEG(Poco::SharedPtr<JpegOptions2>&& val);

	void setMPEG4(const Poco::SharedPtr<Mpeg4Options2>& val);

	void setMPEG4(Poco::SharedPtr<Mpeg4Options2>&& val);

private:
	//@ mandatory=false
	//@ name=JPEG
	//@ order=0
	Poco::SharedPtr<JpegOptions2> _jPEG;

	//@ mandatory=false
	//@ name=MPEG4
	//@ order=1
	Poco::SharedPtr<Mpeg4Options2> _mPEG4;

	//@ mandatory=false
	//@ name=H264
	//@ order=2
	Poco::SharedPtr<H264Options2> _h264;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<VideoEncoderOptionsExtension2> _extension;

};


inline const Poco::SharedPtr<VideoEncoderOptionsExtension2>& VideoEncoderOptionsExtension::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<H264Options2>& VideoEncoderOptionsExtension::getH264() const
{
	return _h264;
}


inline const Poco::SharedPtr<JpegOptions2>& VideoEncoderOptionsExtension::getJPEG() const
{
	return _jPEG;
}


inline const Poco::SharedPtr<Mpeg4Options2>& VideoEncoderOptionsExtension::getMPEG4() const
{
	return _mPEG4;
}


inline void VideoEncoderOptionsExtension::setExtension(const Poco::SharedPtr<VideoEncoderOptionsExtension2>& val)
{
	_extension = val;
}


inline void VideoEncoderOptionsExtension::setExtension(Poco::SharedPtr<VideoEncoderOptionsExtension2>&& val)
{
	_extension = std::move(val);
}


inline void VideoEncoderOptionsExtension::setH264(const Poco::SharedPtr<H264Options2>& val)
{
	_h264 = val;
}


inline void VideoEncoderOptionsExtension::setH264(Poco::SharedPtr<H264Options2>&& val)
{
	_h264 = std::move(val);
}


inline void VideoEncoderOptionsExtension::setJPEG(const Poco::SharedPtr<JpegOptions2>& val)
{
	_jPEG = val;
}


inline void VideoEncoderOptionsExtension::setJPEG(Poco::SharedPtr<JpegOptions2>&& val)
{
	_jPEG = std::move(val);
}


inline void VideoEncoderOptionsExtension::setMPEG4(const Poco::SharedPtr<Mpeg4Options2>& val)
{
	_mPEG4 = val;
}


inline void VideoEncoderOptionsExtension::setMPEG4(Poco::SharedPtr<Mpeg4Options2>&& val)
{
	_mPEG4 = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/H264Options2.h"
#include "ONVIF/JpegOptions2.h"
#include "ONVIF/Mpeg4Options2.h"
#include "ONVIF/VideoEncoderOptionsExtension2.h"


#endif // ONVIF_VideoEncoderOptionsExtension_INCLUDED
