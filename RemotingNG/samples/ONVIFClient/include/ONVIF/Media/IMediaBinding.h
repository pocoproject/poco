//
// IMediaBinding.h
//
// Package: Generated
// Module:  IMediaBinding
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef IMediaBinding_INCLUDED
#define IMediaBinding_INCLUDED


#include "ONVIF/Media/MediaBinding.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"


namespace ONVIF {
namespace Media {


class IMediaBinding: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IMediaBinding>;

	IMediaBinding();
		/// Creates a IMediaBinding.

	virtual ~IMediaBinding();
		/// Destroys the IMediaBinding.

	virtual void addAudioDecoderConfiguration(const std::string& profileToken, const std::string& configurationToken) = 0;

	virtual void addAudioEncoderConfiguration(const std::string& profileToken, const std::string& configurationToken) = 0;

	virtual void addAudioOutputConfiguration(const std::string& profileToken, const std::string& configurationToken) = 0;

	virtual void addAudioSourceConfiguration(const std::string& profileToken, const std::string& configurationToken) = 0;

	virtual void addMetadataConfiguration(const std::string& profileToken, const std::string& configurationToken) = 0;

	virtual void addPTZConfiguration(const std::string& profileToken, const std::string& configurationToken) = 0;

	virtual void addVideoAnalyticsConfiguration(const std::string& profileToken, const std::string& configurationToken) = 0;

	virtual void addVideoEncoderConfiguration(const std::string& profileToken, const std::string& configurationToken) = 0;

	virtual void addVideoSourceConfiguration(const std::string& profileToken, const std::string& configurationToken) = 0;

	virtual void createOSD(const ONVIF::OSDConfiguration& oSD, std::string& oSDToken) = 0;

	virtual void createProfile(const std::string& name, const Poco::Optional<std::string>& token, ONVIF::Profile& profile) = 0;

	virtual void deleteOSD(const std::string& oSDToken) = 0;

	virtual void deleteProfile(const std::string& profileToken) = 0;

	virtual void getAudioDecoderConfiguration(const std::string& configurationToken, ONVIF::AudioDecoderConfiguration& configuration) = 0;

	virtual void getAudioDecoderConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::AudioDecoderConfigurationOptions& options) = 0;

	virtual void getAudioDecoderConfigurations(std::vector<ONVIF::AudioDecoderConfiguration>& configurations) = 0;

	virtual void getAudioEncoderConfiguration(const std::string& configurationToken, ONVIF::AudioEncoderConfiguration& configuration) = 0;

	virtual void getAudioEncoderConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::AudioEncoderConfigurationOptions& options) = 0;

	virtual void getAudioEncoderConfigurations(std::vector<ONVIF::AudioEncoderConfiguration>& configurations) = 0;

	virtual void getAudioOutputConfiguration(const std::string& configurationToken, ONVIF::AudioOutputConfiguration& configuration) = 0;

	virtual void getAudioOutputConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::AudioOutputConfigurationOptions& options) = 0;

	virtual void getAudioOutputConfigurations(std::vector<ONVIF::AudioOutputConfiguration>& configurations) = 0;

	virtual void getAudioOutputs(std::vector<ONVIF::AudioOutput>& audioOutputs) = 0;

	virtual void getAudioSourceConfiguration(const std::string& configurationToken, ONVIF::AudioSourceConfiguration& configuration) = 0;

	virtual void getAudioSourceConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::AudioSourceConfigurationOptions& options) = 0;

	virtual void getAudioSourceConfigurations(std::vector<ONVIF::AudioSourceConfiguration>& configurations) = 0;

	virtual void getAudioSources(std::vector<ONVIF::AudioSource>& audioSources) = 0;

	virtual void getCompatibleAudioDecoderConfigurations(const std::string& profileToken, std::vector<ONVIF::AudioDecoderConfiguration>& configurations) = 0;

	virtual void getCompatibleAudioEncoderConfigurations(const std::string& profileToken, std::vector<ONVIF::AudioEncoderConfiguration>& configurations) = 0;

	virtual void getCompatibleAudioOutputConfigurations(const std::string& profileToken, std::vector<ONVIF::AudioOutputConfiguration>& configurations) = 0;

	virtual void getCompatibleAudioSourceConfigurations(const std::string& profileToken, std::vector<ONVIF::AudioSourceConfiguration>& configurations) = 0;

	virtual void getCompatibleMetadataConfigurations(const std::string& profileToken, std::vector<ONVIF::MetadataConfiguration>& configurations) = 0;

	virtual void getCompatibleVideoAnalyticsConfigurations(const std::string& profileToken, std::vector<ONVIF::VideoAnalyticsConfiguration>& configurations) = 0;

	virtual void getCompatibleVideoEncoderConfigurations(const std::string& profileToken, std::vector<ONVIF::VideoEncoderConfiguration>& configurations) = 0;

	virtual void getCompatibleVideoSourceConfigurations(const std::string& profileToken, std::vector<ONVIF::VideoSourceConfiguration>& configurations) = 0;

	virtual void getGuaranteedNumberOfVideoEncoderInstances(const std::string& configurationToken, int& totalNumber, Poco::Optional<int>& jPEG, Poco::Optional<int>& h264, Poco::Optional<int>& mPEG4) = 0;

	virtual void getMetadataConfiguration(const std::string& configurationToken, ONVIF::MetadataConfiguration& configuration) = 0;

	virtual void getMetadataConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::MetadataConfigurationOptions& options) = 0;

	virtual void getMetadataConfigurations(std::vector<ONVIF::MetadataConfiguration>& configurations) = 0;

	virtual void getOSD(const std::string& oSDToken, ONVIF::OSDConfiguration& oSD) = 0;

	virtual void getOSDOptions(const std::string& configurationToken, ONVIF::OSDConfigurationOptions& oSDOptions) = 0;

	virtual void getOSDs(const Poco::Optional<std::string>& configurationToken, std::vector<ONVIF::OSDConfiguration>& oSDs) = 0;

	virtual void getProfile(const std::string& profileToken, ONVIF::Profile& profile) = 0;

	virtual void getProfiles(std::vector<ONVIF::Profile>& profiles) = 0;

	virtual void getServiceCapabilities(ONVIF::Media::Capabilities& capabilities) = 0;

	virtual void getSnapshotUri(const std::string& profileToken, ONVIF::MediaUri& mediaUri) = 0;

	virtual void getStreamUri(const ONVIF::StreamSetup& streamSetup, const std::string& profileToken, ONVIF::MediaUri& mediaUri) = 0;

	virtual void getVideoAnalyticsConfiguration(const std::string& configurationToken, ONVIF::VideoAnalyticsConfiguration& configuration) = 0;

	virtual void getVideoAnalyticsConfigurations(std::vector<ONVIF::VideoAnalyticsConfiguration>& configurations) = 0;

	virtual void getVideoEncoderConfiguration(const std::string& configurationToken, ONVIF::VideoEncoderConfiguration& configuration) = 0;

	virtual void getVideoEncoderConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::VideoEncoderConfigurationOptions& options) = 0;

	virtual void getVideoEncoderConfigurations(std::vector<ONVIF::VideoEncoderConfiguration>& configurations) = 0;

	virtual void getVideoSourceConfiguration(const std::string& configurationToken, ONVIF::VideoSourceConfiguration& configuration) = 0;

	virtual void getVideoSourceConfigurationOptions(const Poco::Optional<std::string>& configurationToken, const Poco::Optional<std::string>& profileToken, ONVIF::VideoSourceConfigurationOptions& options) = 0;

	virtual void getVideoSourceConfigurations(std::vector<ONVIF::VideoSourceConfiguration>& configurations) = 0;

	virtual void getVideoSourceModes(const std::string& videoSourceToken, std::vector<ONVIF::Media::VideoSourceMode>& videoSourceModes) = 0;

	virtual void getVideoSources(std::vector<ONVIF::VideoSource>& videoSources) = 0;

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

	virtual void removeAudioDecoderConfiguration(const std::string& profileToken) = 0;

	virtual void removeAudioEncoderConfiguration(const std::string& profileToken) = 0;

	virtual void removeAudioOutputConfiguration(const std::string& profileToken) = 0;

	virtual void removeAudioSourceConfiguration(const std::string& profileToken) = 0;

	virtual void removeMetadataConfiguration(const std::string& profileToken) = 0;

	virtual void removePTZConfiguration(const std::string& profileToken) = 0;

	virtual void removeVideoAnalyticsConfiguration(const std::string& profileToken) = 0;

	virtual void removeVideoEncoderConfiguration(const std::string& profileToken) = 0;

	virtual void removeVideoSourceConfiguration(const std::string& profileToken) = 0;

	virtual void setAudioDecoderConfiguration(const ONVIF::AudioDecoderConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setAudioEncoderConfiguration(const ONVIF::AudioEncoderConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setAudioOutputConfiguration(const ONVIF::AudioOutputConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setAudioSourceConfiguration(const ONVIF::AudioSourceConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setMetadataConfiguration(const ONVIF::MetadataConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setOSD(const ONVIF::OSDConfiguration& oSD) = 0;

	virtual void setSynchronizationPoint(const std::string& profileToken) = 0;

	virtual void setVideoAnalyticsConfiguration(const ONVIF::VideoAnalyticsConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setVideoEncoderConfiguration(const ONVIF::VideoEncoderConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setVideoSourceConfiguration(const ONVIF::VideoSourceConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setVideoSourceMode(const std::string& videoSourceToken, const std::string& videoSourceModeToken, bool& reboot) = 0;

	virtual void startMulticastStreaming(const std::string& profileToken) = 0;

	virtual void stopMulticastStreaming(const std::string& profileToken) = 0;

};


} // namespace Media
} // namespace ONVIF


#endif // IMediaBinding_INCLUDED

