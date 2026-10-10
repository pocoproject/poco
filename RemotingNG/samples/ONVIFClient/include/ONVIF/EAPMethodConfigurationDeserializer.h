//
// EAPMethodConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_EAPMethodConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_EAPMethodConfiguration_INCLUDED


#include "ONVIF/EAPMethodConfiguration.h"
#include "ONVIF/EapMethodExtensionDeserializer.h"
#include "ONVIF/EapMethodExtensionSerializer.h"
#include "ONVIF/TLSConfigurationDeserializer.h"
#include "ONVIF/TLSConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::EAPMethodConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::EAPMethodConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::EAPMethodConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TLSConfiguration"s,"Password"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::TLSConfiguration> gen_tLSConfiguration;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::TLSConfiguration>>::deserialize(REMOTING__NAMES[0], false, deser, gen_tLSConfiguration);
		if (ret) value.setTLSConfiguration(gen_tLSConfiguration);
		Poco::Optional<std::string> gen_password;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_password);
		if (ret) value.setPassword(gen_password);
		Poco::SharedPtr<ONVIF::EapMethodExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::EapMethodExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_EAPMethodConfiguration_INCLUDED

