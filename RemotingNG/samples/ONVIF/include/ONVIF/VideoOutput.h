// 
// VideoOutput.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoOutput_INCLUDED
#define ONVIF_VideoOutput_INCLUDED


#include "ONVIF/DeviceEntity.h"
#include "ONVIF/Layout.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class VideoOutputExtension;
class VideoResolution;
} 


namespace ONVIF {


//@ name=VideoOutput
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoOutput: public DeviceEntity
{
public:
	VideoOutput();

	VideoOutput(
		const std::string& token, 
		const Layout& layout, 
		const Poco::SharedPtr<VideoResolution>& resolution, 
		Poco::Optional<float> refreshRate, 
		Poco::Optional<float> aspectRatio, 
		const Poco::SharedPtr<VideoOutputExtension>& extension);

	virtual ~VideoOutput();

	Poco::Optional<float> getAspectRatio() const;

	const Poco::SharedPtr<VideoOutputExtension>& getExtension() const;

	const Layout& getLayout() const;

	Poco::Optional<float> getRefreshRate() const;

	const Poco::SharedPtr<VideoResolution>& getResolution() const;

	void setAspectRatio(Poco::Optional<float> val);

	void setExtension(const Poco::SharedPtr<VideoOutputExtension>& val);

	void setExtension(Poco::SharedPtr<VideoOutputExtension>&& val);

	void setLayout(const Layout& val);

	void setLayout(Layout&& val);

	void setRefreshRate(Poco::Optional<float> val);

	void setResolution(const Poco::SharedPtr<VideoResolution>& val);

	void setResolution(Poco::SharedPtr<VideoResolution>&& val);

private:
	//@ name=Layout
	//@ order=0
	Layout _layout;

	//@ mandatory=false
	//@ name=Resolution
	//@ order=1
	Poco::SharedPtr<VideoResolution> _resolution;

	//@ mandatory=false
	//@ name=RefreshRate
	//@ order=2
	Poco::Optional<float> _refreshRate;

	//@ mandatory=false
	//@ name=AspectRatio
	//@ order=3
	Poco::Optional<float> _aspectRatio;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<VideoOutputExtension> _extension;

};


inline Poco::Optional<float> VideoOutput::getAspectRatio() const
{
	return _aspectRatio;
}


inline const Poco::SharedPtr<VideoOutputExtension>& VideoOutput::getExtension() const
{
	return _extension;
}


inline const Layout& VideoOutput::getLayout() const
{
	return _layout;
}


inline Poco::Optional<float> VideoOutput::getRefreshRate() const
{
	return _refreshRate;
}


inline const Poco::SharedPtr<VideoResolution>& VideoOutput::getResolution() const
{
	return _resolution;
}


inline void VideoOutput::setAspectRatio(Poco::Optional<float> val)
{
	_aspectRatio = val;
}


inline void VideoOutput::setExtension(const Poco::SharedPtr<VideoOutputExtension>& val)
{
	_extension = val;
}


inline void VideoOutput::setExtension(Poco::SharedPtr<VideoOutputExtension>&& val)
{
	_extension = std::move(val);
}


inline void VideoOutput::setLayout(const Layout& val)
{
	_layout = val;
}


inline void VideoOutput::setLayout(Layout&& val)
{
	_layout = std::move(val);
}


inline void VideoOutput::setRefreshRate(Poco::Optional<float> val)
{
	_refreshRate = val;
}


inline void VideoOutput::setResolution(const Poco::SharedPtr<VideoResolution>& val)
{
	_resolution = val;
}


inline void VideoOutput::setResolution(Poco::SharedPtr<VideoResolution>&& val)
{
	_resolution = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/VideoOutputExtension.h"
#include "ONVIF/VideoResolution.h"


#endif // ONVIF_VideoOutput_INCLUDED
