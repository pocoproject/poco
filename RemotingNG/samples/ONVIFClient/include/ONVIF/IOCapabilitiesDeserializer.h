//
// IOCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_IOCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_IOCapabilities_INCLUDED


#include "ONVIF/IOCapabilities.h"
#include "ONVIF/IOCapabilitiesExtensionDeserializer.h"
#include "ONVIF/IOCapabilitiesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::IOCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::IOCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::IOCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"InputConnectors"s,"RelayOutputs"s,"Extension"s};
		bool ret = false;
		Poco::Optional<int> gen_inputConnectors;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_inputConnectors);
		if (ret) value.setInputConnectors(gen_inputConnectors);
		Poco::Optional<int> gen_relayOutputs;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[1], false, deser, gen_relayOutputs);
		if (ret) value.setRelayOutputs(gen_relayOutputs);
		Poco::SharedPtr<ONVIF::IOCapabilitiesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IOCapabilitiesExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_IOCapabilities_INCLUDED

