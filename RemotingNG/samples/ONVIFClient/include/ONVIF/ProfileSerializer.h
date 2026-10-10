//
// ProfileSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Profile_INCLUDED
#define TypeSerializer_ONVIF_Profile_INCLUDED


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
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Profile>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"fixed"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1]);
	}

	static void serialize(const std::string& name, const ONVIF::Profile& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::Profile& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"fixed"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getToken(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getFixed(), ser);
	}

	static void serializeImpl(const ONVIF::Profile& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"VideoSourceConfiguration"s,"AudioSourceConfiguration"s,"VideoEncoderConfiguration"s,"AudioEncoderConfiguration"s,"VideoAnalyticsConfiguration"s,"PTZConfiguration"s,"MetadataConfiguration"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getName(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoSourceConfiguration>>::serialize(REMOTING__NAMES[1], value.getVideoSourceConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioSourceConfiguration>>::serialize(REMOTING__NAMES[2], value.getAudioSourceConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoEncoderConfiguration>>::serialize(REMOTING__NAMES[3], value.getVideoEncoderConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioEncoderConfiguration>>::serialize(REMOTING__NAMES[4], value.getAudioEncoderConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoAnalyticsConfiguration>>::serialize(REMOTING__NAMES[5], value.getVideoAnalyticsConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZConfiguration>>::serialize(REMOTING__NAMES[6], value.getPTZConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MetadataConfiguration>>::serialize(REMOTING__NAMES[7], value.getMetadataConfiguration(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ProfileExtension>>::serialize(REMOTING__NAMES[8], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Profile_INCLUDED

