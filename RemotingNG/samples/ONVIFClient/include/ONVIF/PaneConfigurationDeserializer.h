//
// PaneConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PaneConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_PaneConfiguration_INCLUDED


#include "ONVIF/AudioEncoderConfigurationDeserializer.h"
#include "ONVIF/AudioEncoderConfigurationSerializer.h"
#include "ONVIF/PaneConfiguration.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PaneConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PaneConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PaneConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PaneName"s,"AudioOutputToken"s,"AudioSourceToken"s,"AudioEncoderConfiguration"s,"ReceiverToken"s,"Token"s};
		bool ret = false;
		Poco::Optional<std::string> gen_paneName;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_paneName);
		if (ret) value.setPaneName(gen_paneName);
		Poco::Optional<std::string> gen_audioOutputToken;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_audioOutputToken);
		if (ret) value.setAudioOutputToken(gen_audioOutputToken);
		Poco::Optional<std::string> gen_audioSourceToken;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_audioSourceToken);
		if (ret) value.setAudioSourceToken(gen_audioSourceToken);
		Poco::SharedPtr<ONVIF::AudioEncoderConfiguration> gen_audioEncoderConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AudioEncoderConfiguration>>::deserialize(REMOTING__NAMES[3], false, deser, gen_audioEncoderConfiguration);
		if (ret) value.setAudioEncoderConfiguration(gen_audioEncoderConfiguration);
		Poco::Optional<std::string> gen_receiverToken;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[4], false, deser, gen_receiverToken);
		if (ret) value.setReceiverToken(gen_receiverToken);
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[5], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PaneConfiguration_INCLUDED

