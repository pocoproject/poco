// 
// VideoSource.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoSource_INCLUDED
#define ONVIF_VideoSource_INCLUDED


#include "ONVIF/DeviceEntity.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ImagingSettings;
class VideoSourceExtension;
} 


namespace ONVIF {


//@ name=VideoSource
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoSource: public DeviceEntity
{
public:
	VideoSource();

	VideoSource(
		const std::string& token, 
		float framerate, 
		const VideoResolution& resolution, 
		const Poco::SharedPtr<ImagingSettings>& imaging, 
		const Poco::SharedPtr<VideoSourceExtension>& extension);

	virtual ~VideoSource();

	const Poco::SharedPtr<VideoSourceExtension>& getExtension() const;

	float getFramerate() const;

	const Poco::SharedPtr<ImagingSettings>& getImaging() const;

	const VideoResolution& getResolution() const;

	void setExtension(const Poco::SharedPtr<VideoSourceExtension>& val);

	void setExtension(Poco::SharedPtr<VideoSourceExtension>&& val);

	void setFramerate(float val);

	void setImaging(const Poco::SharedPtr<ImagingSettings>& val);

	void setImaging(Poco::SharedPtr<ImagingSettings>&& val);

	void setResolution(const VideoResolution& val);

	void setResolution(VideoResolution&& val);

private:
	//@ name=Framerate
	//@ order=0
	float _framerate;

	//@ name=Resolution
	//@ order=1
	VideoResolution _resolution;

	//@ mandatory=false
	//@ name=Imaging
	//@ order=2
	Poco::SharedPtr<ImagingSettings> _imaging;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<VideoSourceExtension> _extension;

};


inline const Poco::SharedPtr<VideoSourceExtension>& VideoSource::getExtension() const
{
	return _extension;
}


inline float VideoSource::getFramerate() const
{
	return _framerate;
}


inline const Poco::SharedPtr<ImagingSettings>& VideoSource::getImaging() const
{
	return _imaging;
}


inline const VideoResolution& VideoSource::getResolution() const
{
	return _resolution;
}


inline void VideoSource::setExtension(const Poco::SharedPtr<VideoSourceExtension>& val)
{
	_extension = val;
}


inline void VideoSource::setExtension(Poco::SharedPtr<VideoSourceExtension>&& val)
{
	_extension = std::move(val);
}


inline void VideoSource::setFramerate(float val)
{
	_framerate = val;
}


inline void VideoSource::setImaging(const Poco::SharedPtr<ImagingSettings>& val)
{
	_imaging = val;
}


inline void VideoSource::setImaging(Poco::SharedPtr<ImagingSettings>&& val)
{
	_imaging = std::move(val);
}


inline void VideoSource::setResolution(const VideoResolution& val)
{
	_resolution = val;
}


inline void VideoSource::setResolution(VideoResolution&& val)
{
	_resolution = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ImagingSettings.h"
#include "ONVIF/VideoSourceExtension.h"


#endif // ONVIF_VideoSource_INCLUDED
