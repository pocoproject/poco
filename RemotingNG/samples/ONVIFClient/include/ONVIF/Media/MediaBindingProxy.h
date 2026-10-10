//
// MediaBindingProxy.h
//
// Package: Generated
// Module:  MediaBindingProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef MediaBindingProxy_INCLUDED
#define MediaBindingProxy_INCLUDED


#include "ONVIF/Media/IMediaBinding.h"
#include "Poco/RemotingNG/Proxy.h"


namespace ONVIF {
namespace Media {


class MediaBindingProxy: public ONVIF::Media::IMediaBinding, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<MediaBindingProxy>;

	MediaBindingProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a MediaBindingProxy.

	virtual ~MediaBindingProxy();
		/// Destroys the MediaBindingProxy.

	virtual void addAudioDecoderConfiguration(const std::string& profileToken, const std::string& configurationToken);

	virtual void addAudioEncoderConfiguration(const std::string& profileToken, const std::string& configurationToken);

	virtual void addAudioOutputConfiguration(const std::string& profileToken, const std::string& configurationToken);

	virtual void addAudioSourceConfiguration(const std::string& profileToken, const std::string& configurationToken);

	virtual void addMetadataConfiguration(const std::string& profileToken, const std::string& configurationToken);

	virtual void addPTZConfiguration(const std::string& profileToken, const std::string& configurationToken);

	virtual void addVideoAnalyticsConfiguration(const std::string& profileToken, const std::string& configurationToken);

	virtual void addVideoEncoderConfiguration(const std::string& profileToken, const std::string& configurationToken);

	virtual void addVideoSourceConfiguration(const std::string& profileToken, const std::string& configurationToken);

	virtual void createOSD(const ONVIF::OSDConfiguration& oSD, std::string& oSDToken);

	virtual void createProfile(const std::string& name, const Poco::Optional<std::string>& token, ONVIF::Profile& profile);

	virtual void deleteOSD(const std::string& oSDToken);

	virtual void deleteProfile(const std::string& profileToken);

	virtual void getAudioDecoderConfiguration(const std::string& configurationToken, ONVIF::AudioDecoderConfiguration& configuration);

	virtual void getAudioDecoderConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::AudioDecoderConfigurationOptions& options);

	virtual void getAudioDecoderConfigurations(std::vector<ONVIF::AudioDecoderConfiguration>& configurations);

	virtual void getAudioEncoderConfiguration(const std::string& configurationToken, ONVIF::AudioEncoderConfiguration& configuration);

	virtual void getAudioEncoderConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::AudioEncoderConfigurationOptions& options);

	virtual void getAudioEncoderConfigurations(std::vector<ONVIF::AudioEncoderConfiguration>& configurations);

	virtual void getAudioOutputConfiguration(const std::string& configurationToken, ONVIF::AudioOutputConfiguration& configuration);

	virtual void getAudioOutputConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::AudioOutputConfigurationOptions& options);

	virtual void getAudioOutputConfigurations(std::vector<ONVIF::AudioOutputConfiguration>& configurations);

	virtual void getAudioOutputs(std::vector<ONVIF::AudioOutput>& audioOutputs);

	virtual void getAudioSourceConfiguration(const std::string& configurationToken, ONVIF::AudioSourceConfiguration& configuration);

	virtual void getAudioSourceConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::AudioSourceConfigurationOptions& options);

	virtual void getAudioSourceConfigurations(std::vector<ONVIF::AudioSourceConfiguration>& configurations);

	virtual void getAudioSources(std::vector<ONVIF::AudioSource>& audioSources);

	virtual void getCompatibleAudioDecoderConfigurations(const std::string& profileToken, std::vector<ONVIF::AudioDecoderConfiguration>& configurations);

	virtual void getCompatibleAudioEncoderConfigurations(const std::string& profileToken, std::vector<ONVIF::AudioEncoderConfiguration>& configurations);

	virtual void getCompatibleAudioOutputConfigurations(const std::string& profileToken, std::vector<ONVIF::AudioOutputConfiguration>& configurations);

	virtual void getCompatibleAudioSourceConfigurations(const std::string& profileToken, std::vector<ONVIF::AudioSourceConfiguration>& configurations);

	virtual void getCompatibleMetadataConfigurations(const std::string& profileToken, std::vector<ONVIF::MetadataConfiguration>& configurations);

	virtual void getCompatibleVideoAnalyticsConfigurations(const std::string& profileToken, std::vector<ONVIF::VideoAnalyticsConfiguration>& configurations);

	virtual void getCompatibleVideoEncoderConfigurations(const std::string& profileToken, std::vector<ONVIF::VideoEncoderConfiguration>& configurations);

	virtual void getCompatibleVideoSourceConfigurations(const std::string& profileToken, std::vector<ONVIF::VideoSourceConfiguration>& configurations);

	virtual void getGuaranteedNumberOfVideoEncoderInstances(const std::string& configurationToken, int& totalNumber, Poco::Optional<int>& jPEG, Poco::Optional<int>& h264, Poco::Optional<int>& mPEG4);

	virtual void getMetadataConfiguration(const std::string& configurationToken, ONVIF::MetadataConfiguration& configuration);

	virtual void getMetadataConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::MetadataConfigurationOptions& options);

	virtual void getMetadataConfigurations(std::vector<ONVIF::MetadataConfiguration>& configurations);

	virtual void getOSD(const std::string& oSDToken, ONVIF::OSDConfiguration& oSD);

	virtual void getOSDOptions(const std::string& configurationToken, ONVIF::OSDConfigurationOptions& oSDOptions);

	virtual void getOSDs(const Poco::Optional<std::string>& configurationToken, std::vector<ONVIF::OSDConfiguration>& oSDs);

	virtual void getProfile(const std::string& profileToken, ONVIF::Profile& profile);

	virtual void getProfiles(std::vector<ONVIF::Profile>& profiles);

	virtual void getServiceCapabilities(ONVIF::Media::Capabilities& capabilities);

	virtual void getSnapshotUri(const std::string& profileToken, ONVIF::MediaUri& mediaUri);

	virtual void getStreamUri(const ONVIF::StreamSetup& streamSetup, const std::string& profileToken, ONVIF::MediaUri& mediaUri);

	virtual void getVideoAnalyticsConfiguration(const std::string& configurationToken, ONVIF::VideoAnalyticsConfiguration& configuration);

	virtual void getVideoAnalyticsConfigurations(std::vector<ONVIF::VideoAnalyticsConfiguration>& configurations);

	virtual void getVideoEncoderConfiguration(const std::string& configurationToken, ONVIF::VideoEncoderConfiguration& configuration);

	virtual void getVideoEncoderConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::VideoEncoderConfigurationOptions& options);

	virtual void getVideoEncoderConfigurations(std::vector<ONVIF::VideoEncoderConfiguration>& configurations);

	virtual void getVideoSourceConfiguration(const std::string& configurationToken, ONVIF::VideoSourceConfiguration& configuration);

	virtual void getVideoSourceConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::VideoSourceConfigurationOptions& options);

	virtual void getVideoSourceConfigurations(std::vector<ONVIF::VideoSourceConfiguration>& configurations);

	virtual void getVideoSourceModes(const std::string& videoSourceToken, std::vector<ONVIF::Media::VideoSourceMode>& videoSourceModes);

	virtual void getVideoSources(std::vector<ONVIF::VideoSource>& videoSources);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	virtual void removeAudioDecoderConfiguration(const std::string& profileToken);

	virtual void removeAudioEncoderConfiguration(const std::string& profileToken);

	virtual void removeAudioOutputConfiguration(const std::string& profileToken);

	virtual void removeAudioSourceConfiguration(const std::string& profileToken);

	virtual void removeMetadataConfiguration(const std::string& profileToken);

	virtual void removePTZConfiguration(const std::string& profileToken);

	virtual void removeVideoAnalyticsConfiguration(const std::string& profileToken);

	virtual void removeVideoEncoderConfiguration(const std::string& profileToken);

	virtual void removeVideoSourceConfiguration(const std::string& profileToken);

	virtual void setAudioDecoderConfiguration(const ONVIF::AudioDecoderConfiguration& configuration, bool forcePersistence);

	virtual void setAudioEncoderConfiguration(const ONVIF::AudioEncoderConfiguration& configuration, bool forcePersistence);

	virtual void setAudioOutputConfiguration(const ONVIF::AudioOutputConfiguration& configuration, bool forcePersistence);

	virtual void setAudioSourceConfiguration(const ONVIF::AudioSourceConfiguration& configuration, bool forcePersistence);

	virtual void setMetadataConfiguration(const ONVIF::MetadataConfiguration& configuration, bool forcePersistence);

	virtual void setOSD(const ONVIF::OSDConfiguration& oSD);

	virtual void setSynchronizationPoint(const std::string& profileToken);

	virtual void setVideoAnalyticsConfiguration(const ONVIF::VideoAnalyticsConfiguration& configuration, bool forcePersistence);

	virtual void setVideoEncoderConfiguration(const ONVIF::VideoEncoderConfiguration& configuration, bool forcePersistence);

	virtual void setVideoSourceConfiguration(const ONVIF::VideoSourceConfiguration& configuration, bool forcePersistence);

	virtual void setVideoSourceMode(const std::string& videoSourceToken, const std::string& videoSourceModeToken, bool& reboot);

	virtual void startMulticastStreaming(const std::string& profileToken);

	virtual void stopMulticastStreaming(const std::string& profileToken);

private:
	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& MediaBindingProxy::remoting__typeId() const
{
	return IMediaBinding::remoting__typeId();
}


} // namespace Media
} // namespace ONVIF


#endif // MediaBindingProxy_INCLUDED

