// 
// Profile.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Profile_INCLUDED
#define ONVIF_Profile_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AudioEncoderConfiguration;
class AudioSourceConfiguration;
class MetadataConfiguration;
class PTZConfiguration;
class ProfileExtension;
class VideoAnalyticsConfiguration;
class VideoEncoderConfiguration;
class VideoSourceConfiguration;
} 


namespace ONVIF {


//@ name=Profile
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Profile
{
public:
	Profile();

	Profile(
		const std::string& token, 
		Poco::Optional<bool> fixed, 
		const std::string& name, 
		const Poco::SharedPtr<VideoSourceConfiguration>& videoSourceConfiguration, 
		const Poco::SharedPtr<AudioSourceConfiguration>& audioSourceConfiguration, 
		const Poco::SharedPtr<VideoEncoderConfiguration>& videoEncoderConfiguration, 
		const Poco::SharedPtr<AudioEncoderConfiguration>& audioEncoderConfiguration, 
		const Poco::SharedPtr<VideoAnalyticsConfiguration>& videoAnalyticsConfiguration, 
		const Poco::SharedPtr<PTZConfiguration>& pTZConfiguration, 
		const Poco::SharedPtr<MetadataConfiguration>& metadataConfiguration, 
		const Poco::SharedPtr<ProfileExtension>& extension);

	virtual ~Profile();

	const Poco::SharedPtr<AudioEncoderConfiguration>& getAudioEncoderConfiguration() const;

	const Poco::SharedPtr<AudioSourceConfiguration>& getAudioSourceConfiguration() const;

	const Poco::SharedPtr<ProfileExtension>& getExtension() const;

	Poco::Optional<bool> getFixed() const;

	const Poco::SharedPtr<MetadataConfiguration>& getMetadataConfiguration() const;

	const std::string& getName() const;

	const Poco::SharedPtr<PTZConfiguration>& getPTZConfiguration() const;

	const std::string& getToken() const;

	const Poco::SharedPtr<VideoAnalyticsConfiguration>& getVideoAnalyticsConfiguration() const;

	const Poco::SharedPtr<VideoEncoderConfiguration>& getVideoEncoderConfiguration() const;

	const Poco::SharedPtr<VideoSourceConfiguration>& getVideoSourceConfiguration() const;

	void setAudioEncoderConfiguration(const Poco::SharedPtr<AudioEncoderConfiguration>& val);

	void setAudioEncoderConfiguration(Poco::SharedPtr<AudioEncoderConfiguration>&& val);

	void setAudioSourceConfiguration(const Poco::SharedPtr<AudioSourceConfiguration>& val);

	void setAudioSourceConfiguration(Poco::SharedPtr<AudioSourceConfiguration>&& val);

	void setExtension(const Poco::SharedPtr<ProfileExtension>& val);

	void setExtension(Poco::SharedPtr<ProfileExtension>&& val);

	void setFixed(Poco::Optional<bool> val);

	void setMetadataConfiguration(const Poco::SharedPtr<MetadataConfiguration>& val);

	void setMetadataConfiguration(Poco::SharedPtr<MetadataConfiguration>&& val);

	void setName(const std::string& val);

	void setName(std::string&& val);

	void setPTZConfiguration(const Poco::SharedPtr<PTZConfiguration>& val);

	void setPTZConfiguration(Poco::SharedPtr<PTZConfiguration>&& val);

	void setToken(const std::string& val);

	void setToken(std::string&& val);

	void setVideoAnalyticsConfiguration(const Poco::SharedPtr<VideoAnalyticsConfiguration>& val);

	void setVideoAnalyticsConfiguration(Poco::SharedPtr<VideoAnalyticsConfiguration>&& val);

	void setVideoEncoderConfiguration(const Poco::SharedPtr<VideoEncoderConfiguration>& val);

	void setVideoEncoderConfiguration(Poco::SharedPtr<VideoEncoderConfiguration>&& val);

	void setVideoSourceConfiguration(const Poco::SharedPtr<VideoSourceConfiguration>& val);

	void setVideoSourceConfiguration(Poco::SharedPtr<VideoSourceConfiguration>&& val);

private:
	//@ name=token
	//@ order=0
	//@ type=attr
	std::string _token;

	//@ mandatory=false
	//@ name=fixed
	//@ order=1
	//@ type=attr
	Poco::Optional<bool> _fixed;

	//@ name=Name
	//@ order=2
	std::string _name;

	//@ mandatory=false
	//@ name=VideoSourceConfiguration
	//@ order=3
	Poco::SharedPtr<VideoSourceConfiguration> _videoSourceConfiguration;

	//@ mandatory=false
	//@ name=AudioSourceConfiguration
	//@ order=4
	Poco::SharedPtr<AudioSourceConfiguration> _audioSourceConfiguration;

	//@ mandatory=false
	//@ name=VideoEncoderConfiguration
	//@ order=5
	Poco::SharedPtr<VideoEncoderConfiguration> _videoEncoderConfiguration;

	//@ mandatory=false
	//@ name=AudioEncoderConfiguration
	//@ order=6
	Poco::SharedPtr<AudioEncoderConfiguration> _audioEncoderConfiguration;

	//@ mandatory=false
	//@ name=VideoAnalyticsConfiguration
	//@ order=7
	Poco::SharedPtr<VideoAnalyticsConfiguration> _videoAnalyticsConfiguration;

	//@ mandatory=false
	//@ name=PTZConfiguration
	//@ order=8
	Poco::SharedPtr<PTZConfiguration> _pTZConfiguration;

	//@ mandatory=false
	//@ name=MetadataConfiguration
	//@ order=9
	Poco::SharedPtr<MetadataConfiguration> _metadataConfiguration;

	//@ mandatory=false
	//@ name=Extension
	//@ order=10
	Poco::SharedPtr<ProfileExtension> _extension;

};


inline const Poco::SharedPtr<AudioEncoderConfiguration>& Profile::getAudioEncoderConfiguration() const
{
	return _audioEncoderConfiguration;
}


inline const Poco::SharedPtr<AudioSourceConfiguration>& Profile::getAudioSourceConfiguration() const
{
	return _audioSourceConfiguration;
}


inline const Poco::SharedPtr<ProfileExtension>& Profile::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> Profile::getFixed() const
{
	return _fixed;
}


inline const Poco::SharedPtr<MetadataConfiguration>& Profile::getMetadataConfiguration() const
{
	return _metadataConfiguration;
}


inline const std::string& Profile::getName() const
{
	return _name;
}


inline const Poco::SharedPtr<PTZConfiguration>& Profile::getPTZConfiguration() const
{
	return _pTZConfiguration;
}


inline const std::string& Profile::getToken() const
{
	return _token;
}


inline const Poco::SharedPtr<VideoAnalyticsConfiguration>& Profile::getVideoAnalyticsConfiguration() const
{
	return _videoAnalyticsConfiguration;
}


inline const Poco::SharedPtr<VideoEncoderConfiguration>& Profile::getVideoEncoderConfiguration() const
{
	return _videoEncoderConfiguration;
}


inline const Poco::SharedPtr<VideoSourceConfiguration>& Profile::getVideoSourceConfiguration() const
{
	return _videoSourceConfiguration;
}


inline void Profile::setAudioEncoderConfiguration(const Poco::SharedPtr<AudioEncoderConfiguration>& val)
{
	_audioEncoderConfiguration = val;
}


inline void Profile::setAudioEncoderConfiguration(Poco::SharedPtr<AudioEncoderConfiguration>&& val)
{
	_audioEncoderConfiguration = std::move(val);
}


inline void Profile::setAudioSourceConfiguration(const Poco::SharedPtr<AudioSourceConfiguration>& val)
{
	_audioSourceConfiguration = val;
}


inline void Profile::setAudioSourceConfiguration(Poco::SharedPtr<AudioSourceConfiguration>&& val)
{
	_audioSourceConfiguration = std::move(val);
}


inline void Profile::setExtension(const Poco::SharedPtr<ProfileExtension>& val)
{
	_extension = val;
}


inline void Profile::setExtension(Poco::SharedPtr<ProfileExtension>&& val)
{
	_extension = std::move(val);
}


inline void Profile::setFixed(Poco::Optional<bool> val)
{
	_fixed = val;
}


inline void Profile::setMetadataConfiguration(const Poco::SharedPtr<MetadataConfiguration>& val)
{
	_metadataConfiguration = val;
}


inline void Profile::setMetadataConfiguration(Poco::SharedPtr<MetadataConfiguration>&& val)
{
	_metadataConfiguration = std::move(val);
}


inline void Profile::setName(const std::string& val)
{
	_name = val;
}


inline void Profile::setName(std::string&& val)
{
	_name = std::move(val);
}


inline void Profile::setPTZConfiguration(const Poco::SharedPtr<PTZConfiguration>& val)
{
	_pTZConfiguration = val;
}


inline void Profile::setPTZConfiguration(Poco::SharedPtr<PTZConfiguration>&& val)
{
	_pTZConfiguration = std::move(val);
}


inline void Profile::setToken(const std::string& val)
{
	_token = val;
}


inline void Profile::setToken(std::string&& val)
{
	_token = std::move(val);
}


inline void Profile::setVideoAnalyticsConfiguration(const Poco::SharedPtr<VideoAnalyticsConfiguration>& val)
{
	_videoAnalyticsConfiguration = val;
}


inline void Profile::setVideoAnalyticsConfiguration(Poco::SharedPtr<VideoAnalyticsConfiguration>&& val)
{
	_videoAnalyticsConfiguration = std::move(val);
}


inline void Profile::setVideoEncoderConfiguration(const Poco::SharedPtr<VideoEncoderConfiguration>& val)
{
	_videoEncoderConfiguration = val;
}


inline void Profile::setVideoEncoderConfiguration(Poco::SharedPtr<VideoEncoderConfiguration>&& val)
{
	_videoEncoderConfiguration = std::move(val);
}


inline void Profile::setVideoSourceConfiguration(const Poco::SharedPtr<VideoSourceConfiguration>& val)
{
	_videoSourceConfiguration = val;
}


inline void Profile::setVideoSourceConfiguration(Poco::SharedPtr<VideoSourceConfiguration>&& val)
{
	_videoSourceConfiguration = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AudioEncoderConfiguration.h"
#include "ONVIF/AudioSourceConfiguration.h"
#include "ONVIF/MetadataConfiguration.h"
#include "ONVIF/PTZConfiguration.h"
#include "ONVIF/ProfileExtension.h"
#include "ONVIF/VideoAnalyticsConfiguration.h"
#include "ONVIF/VideoEncoderConfiguration.h"
#include "ONVIF/VideoSourceConfiguration.h"


#endif // ONVIF_Profile_INCLUDED
