//
// ProfileExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ProfileExtension_INCLUDED
#define TypeDeserializer_ONVIF_ProfileExtension_INCLUDED


#include "ONVIF/AudioDecoderConfigurationDeserializer.h"
#include "ONVIF/AudioDecoderConfigurationSerializer.h"
#include "ONVIF/AudioOutputConfigurationDeserializer.h"
#include "ONVIF/AudioOutputConfigurationSerializer.h"
#include "ONVIF/ProfileExtension.h"
#include "ONVIF/ProfileExtension2Deserializer.h"
#include "ONVIF/ProfileExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ProfileExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ProfileExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::ProfileExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AudioOutputConfiguration"s,"AudioDecoderConfiguration"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::AudioOutputConfiguration> gen_audioOutputConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AudioOutputConfiguration>>::deserialize(REMOTING__NAMES[0], false, deser, gen_audioOutputConfiguration);
		if (ret) value.setAudioOutputConfiguration(gen_audioOutputConfiguration);
		Poco::SharedPtr<ONVIF::AudioDecoderConfiguration> gen_audioDecoderConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AudioDecoderConfiguration>>::deserialize(REMOTING__NAMES[1], false, deser, gen_audioDecoderConfiguration);
		if (ret) value.setAudioDecoderConfiguration(gen_audioDecoderConfiguration);
		Poco::SharedPtr<ONVIF::ProfileExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ProfileExtension2>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ProfileExtension_INCLUDED

