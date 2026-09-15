//
// NetworkProtocolDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NetworkProtocol_INCLUDED
#define TypeDeserializer_ONVIF_NetworkProtocol_INCLUDED


#include "ONVIF/NetworkProtocol.h"
#include "ONVIF/NetworkProtocolDeserializer.h"
#include "ONVIF/NetworkProtocolExtensionDeserializer.h"
#include "ONVIF/NetworkProtocolExtensionSerializer.h"
#include "ONVIF/NetworkProtocolSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NetworkProtocol>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NetworkProtocol& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::NetworkProtocol& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"Enabled"s,"Port"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::NetworkProtocol> gen_name;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkProtocol>>::deserialize(REMOTING__NAMES[0], true, deser, gen_name);
		if (ret) value.setName(gen_name);
		bool gen_enabled;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_enabled);
		if (ret) value.setEnabled(gen_enabled);
		std::vector<int> gen_port;
		ret = TypeDeserializer<std::vector<int>>::deserialize(REMOTING__NAMES[2], true, deser, gen_port);
		if (ret) value.setPort(gen_port);
		Poco::SharedPtr<ONVIF::NetworkProtocolExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkProtocolExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NetworkProtocol_INCLUDED

