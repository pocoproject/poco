// 
// MediaBinding.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_MediaBinding_INCLUDED
#define ONVIF_Media_MediaBinding_INCLUDED


#include "ONVIF/AudioDecoderConfiguration.h"
#include "ONVIF/AudioDecoderConfigurationOptions.h"
#include "ONVIF/AudioEncoderConfiguration.h"
#include "ONVIF/AudioEncoderConfigurationOptions.h"
#include "ONVIF/AudioOutput.h"
#include "ONVIF/AudioOutputConfiguration.h"
#include "ONVIF/AudioOutputConfigurationOptions.h"
#include "ONVIF/AudioSource.h"
#include "ONVIF/AudioSourceConfiguration.h"
#include "ONVIF/AudioSourceConfigurationOptions.h"
#include "ONVIF/Media/Capabilities.h"
#include "ONVIF/Media/VideoSourceMode.h"
#include "ONVIF/MediaUri.h"
#include "ONVIF/MetadataConfiguration.h"
#include "ONVIF/MetadataConfigurationOptions.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/OSDConfiguration.h"
#include "ONVIF/OSDConfigurationOptions.h"
#include "ONVIF/Profile.h"
#include "ONVIF/StreamSetup.h"
#include "ONVIF/VideoAnalyticsConfiguration.h"
#include "ONVIF/VideoEncoderConfiguration.h"
#include "ONVIF/VideoEncoderConfigurationOptions.h"
#include "ONVIF/VideoSource.h"
#include "ONVIF/VideoSourceConfiguration.h"
#include "ONVIF/VideoSourceConfigurationOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name=MediaBinding
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ remote
class ONVIF_API MediaBinding
{
public:
	virtual ~MediaBinding();

	//@ action="http://www.onvif.org/ver10/media/wsdl/AddAudioDecoderConfiguration"
	//@ name=AddAudioDecoderConfiguration
	//@ replyName=AddAudioDecoderConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	virtual void addAudioDecoderConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/AddAudioEncoderConfiguration"
	//@ name=AddAudioEncoderConfiguration
	//@ replyName=AddAudioEncoderConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	virtual void addAudioEncoderConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/AddAudioOutputConfiguration"
	//@ name=AddAudioOutputConfiguration
	//@ replyName=AddAudioOutputConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	virtual void addAudioOutputConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/AddAudioSourceConfiguration"
	//@ name=AddAudioSourceConfiguration
	//@ replyName=AddAudioSourceConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	virtual void addAudioSourceConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/AddMetadataConfiguration"
	//@ name=AddMetadataConfiguration
	//@ replyName=AddMetadataConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	virtual void addMetadataConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/AddPTZConfiguration"
	//@ name=AddPTZConfiguration
	//@ replyName=AddPTZConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	virtual void addPTZConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/AddVideoAnalyticsConfiguration"
	//@ name=AddVideoAnalyticsConfiguration
	//@ replyName=AddVideoAnalyticsConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	virtual void addVideoAnalyticsConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/AddVideoEncoderConfiguration"
	//@ name=AddVideoEncoderConfiguration
	//@ replyName=AddVideoEncoderConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	virtual void addVideoEncoderConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/AddVideoSourceConfiguration"
	//@ name=AddVideoSourceConfiguration
	//@ replyName=AddVideoSourceConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	virtual void addVideoSourceConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/CreateOSD"
	//@ name=CreateOSD
	//@ replyName=CreateOSDResponse
	//@ $oSD={direction=in, name=OSD}
	//@ $oSDToken={direction=out, name=OSDToken}
	virtual void createOSD(
		const ONVIF::OSDConfiguration& oSD, 
		std::string& oSDToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/CreateProfile"
	//@ name=CreateProfile
	//@ replyName=CreateProfileResponse
	//@ $name={direction=in, name=Name}
	//@ $token={direction=in, name=Token}
	//@ $profile={direction=out, name=Profile}
	virtual void createProfile(
		const std::string& name, 
		const Poco::Optional<std::string>& token, 
		ONVIF::Profile& profile) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/DeleteOSD"
	//@ name=DeleteOSD
	//@ replyName=DeleteOSDResponse
	//@ $oSDToken={direction=in, name=OSDToken}
	virtual void deleteOSD(const std::string& oSDToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/DeleteProfile"
	//@ name=DeleteProfile
	//@ replyName=DeleteProfileResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void deleteProfile(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioDecoderConfiguration"
	//@ name=GetAudioDecoderConfiguration
	//@ replyName=GetAudioDecoderConfigurationResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $configuration={direction=out, name=Configuration}
	virtual void getAudioDecoderConfiguration(
		const std::string& configurationToken, 
		ONVIF::AudioDecoderConfiguration& configuration) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioDecoderConfigurationOptions"
	//@ name=GetAudioDecoderConfigurationOptions
	//@ replyName=GetAudioDecoderConfigurationOptionsResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $options={direction=out, name=Options}
	virtual void getAudioDecoderConfigurationOptions(
		const Poco::Optional<std::string>& configurationToken, 
		const Poco::Optional<std::string>& profileToken, 
		ONVIF::AudioDecoderConfigurationOptions& options) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioDecoderConfigurations"
	//@ name=GetAudioDecoderConfigurations
	//@ replyName=GetAudioDecoderConfigurationsResponse
	//@ $configurations={direction=out, name=Configurations}
	virtual void getAudioDecoderConfigurations(std::vector<ONVIF::AudioDecoderConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioEncoderConfiguration"
	//@ name=GetAudioEncoderConfiguration
	//@ replyName=GetAudioEncoderConfigurationResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $configuration={direction=out, name=Configuration}
	virtual void getAudioEncoderConfiguration(
		const std::string& configurationToken, 
		ONVIF::AudioEncoderConfiguration& configuration) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioEncoderConfigurationOptions"
	//@ name=GetAudioEncoderConfigurationOptions
	//@ replyName=GetAudioEncoderConfigurationOptionsResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $options={direction=out, name=Options}
	virtual void getAudioEncoderConfigurationOptions(
		const Poco::Optional<std::string>& configurationToken, 
		const Poco::Optional<std::string>& profileToken, 
		ONVIF::AudioEncoderConfigurationOptions& options) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioEncoderConfigurations"
	//@ name=GetAudioEncoderConfigurations
	//@ replyName=GetAudioEncoderConfigurationsResponse
	//@ $configurations={direction=out, name=Configurations}
	virtual void getAudioEncoderConfigurations(std::vector<ONVIF::AudioEncoderConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioOutputConfiguration"
	//@ name=GetAudioOutputConfiguration
	//@ replyName=GetAudioOutputConfigurationResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $configuration={direction=out, name=Configuration}
	virtual void getAudioOutputConfiguration(
		const std::string& configurationToken, 
		ONVIF::AudioOutputConfiguration& configuration) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioOutputConfigurationOptions"
	//@ name=GetAudioOutputConfigurationOptions
	//@ replyName=GetAudioOutputConfigurationOptionsResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $options={direction=out, name=Options}
	virtual void getAudioOutputConfigurationOptions(
		const Poco::Optional<std::string>& configurationToken, 
		const Poco::Optional<std::string>& profileToken, 
		ONVIF::AudioOutputConfigurationOptions& options) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioOutputConfigurations"
	//@ name=GetAudioOutputConfigurations
	//@ replyName=GetAudioOutputConfigurationsResponse
	//@ $configurations={direction=out, name=Configurations}
	virtual void getAudioOutputConfigurations(std::vector<ONVIF::AudioOutputConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioOutputs"
	//@ name=GetAudioOutputs
	//@ replyName=GetAudioOutputsResponse
	//@ $audioOutputs={direction=out, name=AudioOutputs}
	virtual void getAudioOutputs(std::vector<ONVIF::AudioOutput>& audioOutputs) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioSourceConfiguration"
	//@ name=GetAudioSourceConfiguration
	//@ replyName=GetAudioSourceConfigurationResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $configuration={direction=out, name=Configuration}
	virtual void getAudioSourceConfiguration(
		const std::string& configurationToken, 
		ONVIF::AudioSourceConfiguration& configuration) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioSourceConfigurationOptions"
	//@ name=GetAudioSourceConfigurationOptions
	//@ replyName=GetAudioSourceConfigurationOptionsResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $options={direction=out, name=Options}
	virtual void getAudioSourceConfigurationOptions(
		const Poco::Optional<std::string>& configurationToken, 
		const Poco::Optional<std::string>& profileToken, 
		ONVIF::AudioSourceConfigurationOptions& options) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdlGetAudioSourceConfigurations/"
	//@ name=GetAudioSourceConfigurations
	//@ replyName=GetAudioSourceConfigurationsResponse
	//@ $configurations={direction=out, name=Configurations}
	virtual void getAudioSourceConfigurations(std::vector<ONVIF::AudioSourceConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetAudioSources"
	//@ name=GetAudioSources
	//@ replyName=GetAudioSourcesResponse
	//@ $audioSources={direction=out, name=AudioSources}
	virtual void getAudioSources(std::vector<ONVIF::AudioSource>& audioSources) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetCompatibleAudioDecoderConfigurations"
	//@ name=GetCompatibleAudioDecoderConfigurations
	//@ replyName=GetCompatibleAudioDecoderConfigurationsResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurations={direction=out, name=Configurations}
	virtual void getCompatibleAudioDecoderConfigurations(
		const std::string& profileToken, 
		std::vector<ONVIF::AudioDecoderConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetCompatibleAudioEncoderConfigurations"
	//@ name=GetCompatibleAudioEncoderConfigurations
	//@ replyName=GetCompatibleAudioEncoderConfigurationsResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurations={direction=out, name=Configurations}
	virtual void getCompatibleAudioEncoderConfigurations(
		const std::string& profileToken, 
		std::vector<ONVIF::AudioEncoderConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetCompatibleAudioOutputConfigurations"
	//@ name=GetCompatibleAudioOutputConfigurations
	//@ replyName=GetCompatibleAudioOutputConfigurationsResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurations={direction=out, name=Configurations}
	virtual void getCompatibleAudioOutputConfigurations(
		const std::string& profileToken, 
		std::vector<ONVIF::AudioOutputConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetCompatibleAudioSourceConfigurations"
	//@ name=GetCompatibleAudioSourceConfigurations
	//@ replyName=GetCompatibleAudioSourceConfigurationsResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurations={direction=out, name=Configurations}
	virtual void getCompatibleAudioSourceConfigurations(
		const std::string& profileToken, 
		std::vector<ONVIF::AudioSourceConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetCompatibleMetadataConfigurations"
	//@ name=GetCompatibleMetadataConfigurations
	//@ replyName=GetCompatibleMetadataConfigurationsResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurations={direction=out, name=Configurations}
	virtual void getCompatibleMetadataConfigurations(
		const std::string& profileToken, 
		std::vector<ONVIF::MetadataConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetCompatibleVideoAnalyticsConfigurations"
	//@ name=GetCompatibleVideoAnalyticsConfigurations
	//@ replyName=GetCompatibleVideoAnalyticsConfigurationsResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurations={direction=out, name=Configurations}
	virtual void getCompatibleVideoAnalyticsConfigurations(
		const std::string& profileToken, 
		std::vector<ONVIF::VideoAnalyticsConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetCompatibleVideoEncoderConfigurations"
	//@ name=GetCompatibleVideoEncoderConfigurations
	//@ replyName=GetCompatibleVideoEncoderConfigurationsResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurations={direction=out, name=Configurations}
	virtual void getCompatibleVideoEncoderConfigurations(
		const std::string& profileToken, 
		std::vector<ONVIF::VideoEncoderConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetCompatibleVideoSourceConfigurations"
	//@ name=GetCompatibleVideoSourceConfigurations
	//@ replyName=GetCompatibleVideoSourceConfigurationsResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $configurations={direction=out, name=Configurations}
	virtual void getCompatibleVideoSourceConfigurations(
		const std::string& profileToken, 
		std::vector<ONVIF::VideoSourceConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetGuaranteedNumberOfVideoEncoderInstances"
	//@ name=GetGuaranteedNumberOfVideoEncoderInstances
	//@ replyName=GetGuaranteedNumberOfVideoEncoderInstancesResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $totalNumber={direction=out, name=TotalNumber}
	//@ $jPEG={direction=out, name=JPEG}
	//@ $h264={direction=out, name=H264}
	//@ $mPEG4={direction=out, name=MPEG4}
	virtual void getGuaranteedNumberOfVideoEncoderInstances(
		const std::string& configurationToken, 
		int& totalNumber, 
		Poco::Optional<int>& jPEG, 
		Poco::Optional<int>& h264, 
		Poco::Optional<int>& mPEG4) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetMetadataConfiguration"
	//@ name=GetMetadataConfiguration
	//@ replyName=GetMetadataConfigurationResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $configuration={direction=out, name=Configuration}
	virtual void getMetadataConfiguration(
		const std::string& configurationToken, 
		ONVIF::MetadataConfiguration& configuration) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetMetadataConfigurationOptions"
	//@ name=GetMetadataConfigurationOptions
	//@ replyName=GetMetadataConfigurationOptionsResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $options={direction=out, name=Options}
	virtual void getMetadataConfigurationOptions(
		const Poco::Optional<std::string>& configurationToken, 
		const Poco::Optional<std::string>& profileToken, 
		ONVIF::MetadataConfigurationOptions& options) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetMetadataConfigurations"
	//@ name=GetMetadataConfigurations
	//@ replyName=GetMetadataConfigurationsResponse
	//@ $configurations={direction=out, name=Configurations}
	virtual void getMetadataConfigurations(std::vector<ONVIF::MetadataConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetOSD"
	//@ name=GetOSD
	//@ replyName=GetOSDResponse
	//@ $oSDToken={direction=in, name=OSDToken}
	//@ $oSD={direction=out, name=OSD}
	virtual void getOSD(
		const std::string& oSDToken, 
		ONVIF::OSDConfiguration& oSD) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetOSDOptions"
	//@ name=GetOSDOptions
	//@ replyName=GetOSDOptionsResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $oSDOptions={direction=out, name=OSDOptions}
	virtual void getOSDOptions(
		const std::string& configurationToken, 
		ONVIF::OSDConfigurationOptions& oSDOptions) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetOSDs"
	//@ name=GetOSDs
	//@ replyName=GetOSDsResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $oSDs={direction=out, name=OSDs}
	virtual void getOSDs(
		const Poco::Optional<std::string>& configurationToken, 
		std::vector<ONVIF::OSDConfiguration>& oSDs) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdlGetProfile/"
	//@ name=GetProfile
	//@ replyName=GetProfileResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $profile={direction=out, name=Profile}
	virtual void getProfile(
		const std::string& profileToken, 
		ONVIF::Profile& profile) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetProfiles"
	//@ name=GetProfiles
	//@ replyName=GetProfilesResponse
	//@ $profiles={direction=out, name=Profiles}
	virtual void getProfiles(std::vector<ONVIF::Profile>& profiles) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetServiceCapabilities"
	//@ name=GetServiceCapabilities
	//@ replyName=GetServiceCapabilitiesResponse
	//@ $capabilities={direction=out, name=Capabilities}
	virtual void getServiceCapabilities(Capabilities& capabilities) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetSnapshotUri"
	//@ name=GetSnapshotUri
	//@ replyName=GetSnapshotUriResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $mediaUri={direction=out, name=MediaUri}
	virtual void getSnapshotUri(
		const std::string& profileToken, 
		ONVIF::MediaUri& mediaUri) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetStreamUri"
	//@ name=GetStreamUri
	//@ replyName=GetStreamUriResponse
	//@ $streamSetup={direction=in, name=StreamSetup}
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $mediaUri={direction=out, name=MediaUri}
	virtual void getStreamUri(
		const ONVIF::StreamSetup& streamSetup, 
		const std::string& profileToken, 
		ONVIF::MediaUri& mediaUri) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetVideoAnalyticsConfiguration"
	//@ name=GetVideoAnalyticsConfiguration
	//@ replyName=GetVideoAnalyticsConfigurationResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $configuration={direction=out, name=Configuration}
	virtual void getVideoAnalyticsConfiguration(
		const std::string& configurationToken, 
		ONVIF::VideoAnalyticsConfiguration& configuration) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetVideoAnalyticsConfigurations"
	//@ name=GetVideoAnalyticsConfigurations
	//@ replyName=GetVideoAnalyticsConfigurationsResponse
	//@ $configurations={direction=out, name=Configurations}
	virtual void getVideoAnalyticsConfigurations(std::vector<ONVIF::VideoAnalyticsConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetVideoEncoderConfiguration"
	//@ name=GetVideoEncoderConfiguration
	//@ replyName=GetVideoEncoderConfigurationResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $configuration={direction=out, name=Configuration}
	virtual void getVideoEncoderConfiguration(
		const std::string& configurationToken, 
		ONVIF::VideoEncoderConfiguration& configuration) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetVideoEncoderConfigurationOptions"
	//@ name=GetVideoEncoderConfigurationOptions
	//@ replyName=GetVideoEncoderConfigurationOptionsResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $options={direction=out, name=Options}
	virtual void getVideoEncoderConfigurationOptions(
		const Poco::Optional<std::string>& configurationToken, 
		const Poco::Optional<std::string>& profileToken, 
		ONVIF::VideoEncoderConfigurationOptions& options) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetVideoEncoderConfigurations"
	//@ name=GetVideoEncoderConfigurations
	//@ replyName=GetVideoEncoderConfigurationsResponse
	//@ $configurations={direction=out, name=Configurations}
	virtual void getVideoEncoderConfigurations(std::vector<ONVIF::VideoEncoderConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetVideoSourceConfiguration"
	//@ name=GetVideoSourceConfiguration
	//@ replyName=GetVideoSourceConfigurationResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $configuration={direction=out, name=Configuration}
	virtual void getVideoSourceConfiguration(
		const std::string& configurationToken, 
		ONVIF::VideoSourceConfiguration& configuration) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdlGetVideoSourceConfigurationOptions/"
	//@ name=GetVideoSourceConfigurationOptions
	//@ replyName=GetVideoSourceConfigurationOptionsResponse
	//@ $configurationToken={direction=in, name=ConfigurationToken}
	//@ $profileToken={direction=in, name=ProfileToken}
	//@ $options={direction=out, name=Options}
	virtual void getVideoSourceConfigurationOptions(
		const Poco::Optional<std::string>& configurationToken, 
		const Poco::Optional<std::string>& profileToken, 
		ONVIF::VideoSourceConfigurationOptions& options) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetVideoSourceConfigurations"
	//@ name=GetVideoSourceConfigurations
	//@ replyName=GetVideoSourceConfigurationsResponse
	//@ $configurations={direction=out, name=Configurations}
	virtual void getVideoSourceConfigurations(std::vector<ONVIF::VideoSourceConfiguration>& configurations) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/GetVideoSourceModes"
	//@ name=GetVideoSourceModes
	//@ replyName=GetVideoSourceModesResponse
	//@ $videoSourceToken={direction=in, name=VideoSourceToken}
	//@ $videoSourceModes={direction=out, name=VideoSourceModes}
	virtual void getVideoSourceModes(
		const std::string& videoSourceToken, 
		std::vector<VideoSourceMode>& videoSourceModes) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdlGetVideoSources/"
	//@ name=GetVideoSources
	//@ replyName=GetVideoSourcesResponse
	//@ $videoSources={direction=out, name=VideoSources}
	virtual void getVideoSources(std::vector<ONVIF::VideoSource>& videoSources) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/RemoveAudioDecoderConfiguration"
	//@ name=RemoveAudioDecoderConfiguration
	//@ replyName=RemoveAudioDecoderConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void removeAudioDecoderConfiguration(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/RemoveAudioEncoderConfiguration"
	//@ name=RemoveAudioEncoderConfiguration
	//@ replyName=RemoveAudioEncoderConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void removeAudioEncoderConfiguration(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/RemoveAudioOutputConfiguration"
	//@ name=RemoveAudioOutputConfiguration
	//@ replyName=RemoveAudioOutputConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void removeAudioOutputConfiguration(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/RemoveAudioSourceConfiguration"
	//@ name=RemoveAudioSourceConfiguration
	//@ replyName=RemoveAudioSourceConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void removeAudioSourceConfiguration(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/RemoveMetadataConfiguration"
	//@ name=RemoveMetadataConfiguration
	//@ replyName=RemoveMetadataConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void removeMetadataConfiguration(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/RemovePTZConfiguration"
	//@ name=RemovePTZConfiguration
	//@ replyName=RemovePTZConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void removePTZConfiguration(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/RemoveVideoAnalyticsConfiguration"
	//@ name=RemoveVideoAnalyticsConfiguration
	//@ replyName=RemoveVideoAnalyticsConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void removeVideoAnalyticsConfiguration(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/RemoveVideoEncoderConfiguration"
	//@ name=RemoveVideoEncoderConfiguration
	//@ replyName=RemoveVideoEncoderConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void removeVideoEncoderConfiguration(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/RemoveVideoSourceConfiguration"
	//@ name=RemoveVideoSourceConfiguration
	//@ replyName=RemoveVideoSourceConfigurationResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void removeVideoSourceConfiguration(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetAudioDecoderConfiguration"
	//@ name=SetAudioDecoderConfiguration
	//@ replyName=SetAudioDecoderConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setAudioDecoderConfiguration(
		const ONVIF::AudioDecoderConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetAudioEncoderConfiguration"
	//@ name=SetAudioEncoderConfiguration
	//@ replyName=SetAudioEncoderConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setAudioEncoderConfiguration(
		const ONVIF::AudioEncoderConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetAudioOutputConfiguration"
	//@ name=SetAudioOutputConfiguration
	//@ replyName=SetAudioOutputConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setAudioOutputConfiguration(
		const ONVIF::AudioOutputConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetAudioSourceConfiguration"
	//@ name=SetAudioSourceConfiguration
	//@ replyName=SetAudioSourceConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setAudioSourceConfiguration(
		const ONVIF::AudioSourceConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetMetadataConfiguration"
	//@ name=SetMetadataConfiguration
	//@ replyName=SetMetadataConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setMetadataConfiguration(
		const ONVIF::MetadataConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetOSD"
	//@ name=SetOSD
	//@ replyName=SetOSDResponse
	//@ $oSD={direction=in, name=OSD}
	virtual void setOSD(const ONVIF::OSDConfiguration& oSD) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetSynchronizationPoint"
	//@ name=SetSynchronizationPoint
	//@ replyName=SetSynchronizationPointResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void setSynchronizationPoint(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetVideoAnalyticsConfiguration"
	//@ name=SetVideoAnalyticsConfiguration
	//@ replyName=SetVideoAnalyticsConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setVideoAnalyticsConfiguration(
		const ONVIF::VideoAnalyticsConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetVideoEncoderConfiguration"
	//@ name=SetVideoEncoderConfiguration
	//@ replyName=SetVideoEncoderConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setVideoEncoderConfiguration(
		const ONVIF::VideoEncoderConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetVideoSourceConfiguration"
	//@ name=SetVideoSourceConfiguration
	//@ replyName=SetVideoSourceConfigurationResponse
	//@ $configuration={direction=in, name=Configuration}
	//@ $forcePersistence={direction=in, name=ForcePersistence}
	virtual void setVideoSourceConfiguration(
		const ONVIF::VideoSourceConfiguration& configuration, 
		bool forcePersistence) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/SetVideoSourceMode"
	//@ name=SetVideoSourceMode
	//@ replyName=SetVideoSourceModeResponse
	//@ $videoSourceToken={direction=in, name=VideoSourceToken}
	//@ $videoSourceModeToken={direction=in, name=VideoSourceModeToken}
	//@ $reboot={direction=out, name=Reboot}
	virtual void setVideoSourceMode(
		const std::string& videoSourceToken, 
		const std::string& videoSourceModeToken, 
		bool& reboot) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/StartMulticastStreaming"
	//@ name=StartMulticastStreaming
	//@ replyName=StartMulticastStreamingResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void startMulticastStreaming(const std::string& profileToken) = 0;

	//@ action="http://www.onvif.org/ver10/media/wsdl/StopMulticastStreaming"
	//@ name=StopMulticastStreaming
	//@ replyName=StopMulticastStreamingResponse
	//@ $profileToken={direction=in, name=ProfileToken}
	virtual void stopMulticastStreaming(const std::string& profileToken) = 0;
};


} } // ONVIF::Media


#endif // ONVIF_Media_MediaBinding_INCLUDED
