//
// Dot11SecurityConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Dot11SecurityConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_Dot11SecurityConfiguration_INCLUDED


#include "ONVIF/Dot11PSKSetDeserializer.h"
#include "ONVIF/Dot11PSKSetSerializer.h"
#include "ONVIF/Dot11SecurityConfiguration.h"
#include "ONVIF/Dot11SecurityConfigurationExtensionDeserializer.h"
#include "ONVIF/Dot11SecurityConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Dot11SecurityConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Dot11SecurityConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Dot11SecurityConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Algorithm"s,"PSK"s,"Dot1X"s,"Extension"s};
		bool ret = false;
		std::string gen_mode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		Poco::Optional<std::string> gen_algorithm;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_algorithm);
		if (ret) value.setAlgorithm(gen_algorithm);
		Poco::SharedPtr<ONVIF::Dot11PSKSet> gen_pSK;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Dot11PSKSet>>::deserialize(REMOTING__NAMES[2], false, deser, gen_pSK);
		if (ret) value.setPSK(gen_pSK);
		Poco::Optional<std::string> gen_dot1X;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[3], false, deser, gen_dot1X);
		if (ret) value.setDot1X(gen_dot1X);
		Poco::SharedPtr<ONVIF::Dot11SecurityConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Dot11SecurityConfigurationExtension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Dot11SecurityConfiguration_INCLUDED

