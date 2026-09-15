// 
// EngineConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_EngineConfiguration_INCLUDED
#define ONVIF_EngineConfiguration_INCLUDED


#include "ONVIF/AnalyticsEngineInputInfo.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoAnalyticsConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=EngineConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API EngineConfiguration
{
public:
	EngineConfiguration();

	EngineConfiguration(
		const VideoAnalyticsConfiguration& videoAnalyticsConfiguration, 
		const AnalyticsEngineInputInfo& analyticsEngineInputInfo);

	virtual ~EngineConfiguration();

	const AnalyticsEngineInputInfo& getAnalyticsEngineInputInfo() const;

	const VideoAnalyticsConfiguration& getVideoAnalyticsConfiguration() const;

	void setAnalyticsEngineInputInfo(const AnalyticsEngineInputInfo& val);

	void setAnalyticsEngineInputInfo(AnalyticsEngineInputInfo&& val);

	void setVideoAnalyticsConfiguration(const VideoAnalyticsConfiguration& val);

	void setVideoAnalyticsConfiguration(VideoAnalyticsConfiguration&& val);

private:
	//@ name=VideoAnalyticsConfiguration
	//@ order=0
	VideoAnalyticsConfiguration _videoAnalyticsConfiguration;

	//@ name=AnalyticsEngineInputInfo
	//@ order=1
	AnalyticsEngineInputInfo _analyticsEngineInputInfo;

};


inline const AnalyticsEngineInputInfo& EngineConfiguration::getAnalyticsEngineInputInfo() const
{
	return _analyticsEngineInputInfo;
}


inline const VideoAnalyticsConfiguration& EngineConfiguration::getVideoAnalyticsConfiguration() const
{
	return _videoAnalyticsConfiguration;
}


inline void EngineConfiguration::setAnalyticsEngineInputInfo(const AnalyticsEngineInputInfo& val)
{
	_analyticsEngineInputInfo = val;
}


inline void EngineConfiguration::setAnalyticsEngineInputInfo(AnalyticsEngineInputInfo&& val)
{
	_analyticsEngineInputInfo = std::move(val);
}


inline void EngineConfiguration::setVideoAnalyticsConfiguration(const VideoAnalyticsConfiguration& val)
{
	_videoAnalyticsConfiguration = val;
}


inline void EngineConfiguration::setVideoAnalyticsConfiguration(VideoAnalyticsConfiguration&& val)
{
	_videoAnalyticsConfiguration = std::move(val);
}


} // ONVIF


#endif // ONVIF_EngineConfiguration_INCLUDED
