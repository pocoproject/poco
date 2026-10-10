//
// Dot1XConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Dot1XConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_Dot1XConfiguration_INCLUDED


#include "ONVIF/Dot1XConfiguration.h"
#include "ONVIF/Dot1XConfigurationExtensionDeserializer.h"
#include "ONVIF/Dot1XConfigurationExtensionSerializer.h"
#include "ONVIF/EAPMethodConfigurationDeserializer.h"
#include "ONVIF/EAPMethodConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Dot1XConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Dot1XConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Dot1XConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dot1XConfigurationToken"s,"Identity"s,"AnonymousID"s,"EAPMethod"s,"CACertificateID"s,"EAPMethodConfiguration"s,"Extension"s};
		bool ret = false;
		std::string gen_dot1XConfigurationToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_dot1XConfigurationToken);
		if (ret) value.setDot1XConfigurationToken(gen_dot1XConfigurationToken);
		std::string gen_identity;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_identity);
		if (ret) value.setIdentity(gen_identity);
		Poco::Optional<std::string> gen_anonymousID;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_anonymousID);
		if (ret) value.setAnonymousID(gen_anonymousID);
		int gen_eAPMethod;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[3], true, deser, gen_eAPMethod);
		if (ret) value.setEAPMethod(gen_eAPMethod);
		std::vector<std::string> gen_cACertificateID;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[4], false, deser, gen_cACertificateID);
		if (ret) value.setCACertificateID(gen_cACertificateID);
		Poco::SharedPtr<ONVIF::EAPMethodConfiguration> gen_eAPMethodConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::EAPMethodConfiguration>>::deserialize(REMOTING__NAMES[5], false, deser, gen_eAPMethodConfiguration);
		if (ret) value.setEAPMethodConfiguration(gen_eAPMethodConfiguration);
		Poco::SharedPtr<ONVIF::Dot1XConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Dot1XConfigurationExtension>>::deserialize(REMOTING__NAMES[6], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Dot1XConfiguration_INCLUDED

