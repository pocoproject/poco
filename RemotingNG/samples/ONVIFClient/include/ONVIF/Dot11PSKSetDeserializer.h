//
// Dot11PSKSetDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Dot11PSKSet_INCLUDED
#define TypeDeserializer_ONVIF_Dot11PSKSet_INCLUDED


#include "ONVIF/Dot11PSKSet.h"
#include "ONVIF/Dot11PSKSetExtensionDeserializer.h"
#include "ONVIF/Dot11PSKSetExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Dot11PSKSet>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Dot11PSKSet& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Dot11PSKSet& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Key"s,"Passphrase"s,"Extension"s};
		bool ret = false;
		Poco::Optional<std::string> gen_key;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_key);
		if (ret) value.setKey(gen_key);
		Poco::Optional<std::string> gen_passphrase;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_passphrase);
		if (ret) value.setPassphrase(gen_passphrase);
		Poco::SharedPtr<ONVIF::Dot11PSKSetExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Dot11PSKSetExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Dot11PSKSet_INCLUDED

