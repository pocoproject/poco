//
// TransportDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Transport_INCLUDED
#define TypeDeserializer_ONVIF_Transport_INCLUDED


#include "ONVIF/Transport.h"
#include "ONVIF/TransportDeserializer.h"
#include "ONVIF/TransportSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Transport>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Transport& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Transport& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Protocol"s,"Tunnel"s};
		bool ret = false;
		std::string gen_protocol;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_protocol);
		if (ret) value.setProtocol(gen_protocol);
		Poco::SharedPtr<ONVIF::Transport> gen_tunnel;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Transport>>::deserialize(REMOTING__NAMES[1], false, deser, gen_tunnel);
		if (ret) value.setTunnel(gen_tunnel);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Transport_INCLUDED

