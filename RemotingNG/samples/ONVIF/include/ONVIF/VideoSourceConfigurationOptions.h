// 
// VideoSourceConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoSourceConfigurationOptions_INCLUDED
#define ONVIF_VideoSourceConfigurationOptions_INCLUDED


#include "ONVIF/IntRectangleRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class VideoSourceConfigurationOptionsExtension;
} 


namespace ONVIF {


//@ name=VideoSourceConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoSourceConfigurationOptions
{
public:
	VideoSourceConfigurationOptions();

	VideoSourceConfigurationOptions(
		Poco::Optional<int> maximumNumberOfProfiles, 
		const IntRectangleRange& boundsRange, 
		const std::vector<std::string>& videoSourceTokensAvailable, 
		const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension>& extension);

	virtual ~VideoSourceConfigurationOptions();

	const IntRectangleRange& getBoundsRange() const;

	const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension>& getExtension() const;

	Poco::Optional<int> getMaximumNumberOfProfiles() const;

	const std::vector<std::string>& getVideoSourceTokensAvailable() const;

	std::vector<std::string>& getVideoSourceTokensAvailable();

	void setBoundsRange(const IntRectangleRange& val);

	void setBoundsRange(IntRectangleRange&& val);

	void setExtension(const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<VideoSourceConfigurationOptionsExtension>&& val);

	void setMaximumNumberOfProfiles(Poco::Optional<int> val);

	void setVideoSourceTokensAvailable(const std::vector<std::string>& val);

	void setVideoSourceTokensAvailable(std::vector<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=MaximumNumberOfProfiles
	//@ order=0
	//@ type=attr
	Poco::Optional<int> _maximumNumberOfProfiles;

	//@ name=BoundsRange
	//@ order=1
	IntRectangleRange _boundsRange;

	//@ name=VideoSourceTokensAvailable
	//@ order=2
	std::vector<std::string> _videoSourceTokensAvailable;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<VideoSourceConfigurationOptionsExtension> _extension;

};


inline const IntRectangleRange& VideoSourceConfigurationOptions::getBoundsRange() const
{
	return _boundsRange;
}


inline const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension>& VideoSourceConfigurationOptions::getExtension() const
{
	return _extension;
}


inline Poco::Optional<int> VideoSourceConfigurationOptions::getMaximumNumberOfProfiles() const
{
	return _maximumNumberOfProfiles;
}


inline const std::vector<std::string>& VideoSourceConfigurationOptions::getVideoSourceTokensAvailable() const
{
	return _videoSourceTokensAvailable;
}


inline std::vector<std::string>& VideoSourceConfigurationOptions::getVideoSourceTokensAvailable()
{
	return _videoSourceTokensAvailable;
}


inline void VideoSourceConfigurationOptions::setBoundsRange(const IntRectangleRange& val)
{
	_boundsRange = val;
}


inline void VideoSourceConfigurationOptions::setBoundsRange(IntRectangleRange&& val)
{
	_boundsRange = std::move(val);
}


inline void VideoSourceConfigurationOptions::setExtension(const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension>& val)
{
	_extension = val;
}


inline void VideoSourceConfigurationOptions::setExtension(Poco::SharedPtr<VideoSourceConfigurationOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void VideoSourceConfigurationOptions::setMaximumNumberOfProfiles(Poco::Optional<int> val)
{
	_maximumNumberOfProfiles = val;
}


inline void VideoSourceConfigurationOptions::setVideoSourceTokensAvailable(const std::vector<std::string>& val)
{
	_videoSourceTokensAvailable = val;
}


inline void VideoSourceConfigurationOptions::setVideoSourceTokensAvailable(std::vector<std::string>&& val)
{
	_videoSourceTokensAvailable = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/VideoSourceConfigurationOptionsExtension.h"


#endif // ONVIF_VideoSourceConfigurationOptions_INCLUDED
