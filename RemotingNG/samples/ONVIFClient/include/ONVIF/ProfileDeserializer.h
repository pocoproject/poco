//
// ProfileDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Profile_INCLUDED
#define TypeDeserializer_ONVIF_Profile_INCLUDED


#include "ONVIF/AudioEncoderConfigurationDeserializer.h"
#include "ONVIF/AudioEncoderConfigurationSerializer.h"
#include "ONVIF/AudioSourceConfigurationDeserializer.h"
#include "ONVIF/AudioSourceConfigurationSerializer.h"
#include "ONVIF/MetadataConfigurationDeserializer.h"
#include "ONVIF/MetadataConfigurationSerializer.h"
#include "ONVIF/PTZConfigurationDeserializer.h"
#include "ONVIF/PTZConfigurationSerializer.h"
#include "ONVIF/Profile.h"
#include "ONVIF/ProfileExtensionDeserializer.h"
#include "ONVIF/ProfileExtensionSerializer.h"
#include "ONVIF/VideoAnalyticsConfigurationDeserializer.h"
#include "ONVIF/VideoAnalyticsConfigurationSerializer.h"
#include "ONVIF/VideoEncoderConfigurationDeserializer.h"
#include "ONVIF/VideoEncoderConfigurationSerializer.h"
#include "ONVIF/VideoSourceConfigurationDeserializer.h"
#include "ONVIF/VideoSourceConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Profile>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Profile& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::Profile& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"fixed"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		Poco::Optional<bool> gen_fixed;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_fixed);
		if (ret) value.setFixed(gen_fixed);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Profile& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"VideoSourceConfiguration"s,"AudioSourceConfiguration"s,"VideoEncoderConfiguration"s,"AudioEncoderConfiguration"s,"VideoAnalyticsConfiguration"s,"PTZConfiguration"s,"MetadataConfiguration"s,"Extension"s};
		bool ret = false;
		std::string gen_name;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_name);
		if (ret) value.setName(gen_name);
		Poco::SharedPtr<ONVIF::VideoSourceConfiguration> gen_videoSourceConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoSourceConfiguration>>::deserialize(REMOTING__NAMES[1], false, deser, gen_videoSourceConfiguration);
		if (ret) value.setVideoSourceConfiguration(gen_videoSourceConfiguration);
		Poco::SharedPtr<ONVIF::AudioSourceConfiguration> gen_audioSourceConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AudioSourceConfiguration>>::deserialize(REMOTING__NAMES[2], false, deser, gen_audioSourceConfiguration);
		if (ret) value.setAudioSourceConfiguration(gen_audioSourceConfiguration);
		Poco::SharedPtr<ONVIF::VideoEncoderConfiguration> gen_videoEncoderConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoEncoderConfiguration>>::deserialize(REMOTING__NAMES[3], false, deser, gen_videoEncoderConfiguration);
		if (ret) value.setVideoEncoderConfiguration(gen_videoEncoderConfiguration);
		Poco::SharedPtr<ONVIF::AudioEncoderConfiguration> gen_audioEncoderConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AudioEncoderConfiguration>>::deserialize(REMOTING__NAMES[4], false, deser, gen_audioEncoderConfiguration);
		if (ret) value.setAudioEncoderConfiguration(gen_audioEncoderConfiguration);
		Poco::SharedPtr<ONVIF::VideoAnalyticsConfiguration> gen_videoAnalyticsConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoAnalyticsConfiguration>>::deserialize(REMOTING__NAMES[5], false, deser, gen_videoAnalyticsConfiguration);
		if (ret) value.setVideoAnalyticsConfiguration(gen_videoAnalyticsConfiguration);
		Poco::SharedPtr<ONVIF::PTZConfiguration> gen_pTZConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZConfiguration>>::deserialize(REMOTING__NAMES[6], false, deser, gen_pTZConfiguration);
		if (ret) value.setPTZConfiguration(gen_pTZConfiguration);
		Poco::SharedPtr<ONVIF::MetadataConfiguration> gen_metadataConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MetadataConfiguration>>::deserialize(REMOTING__NAMES[7], false, deser, gen_metadataConfiguration);
		if (ret) value.setMetadataConfiguration(gen_metadataConfiguration);
		Poco::SharedPtr<ONVIF::ProfileExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ProfileExtension>>::deserialize(REMOTING__NAMES[8], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"fixed"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Profile_INCLUDED

