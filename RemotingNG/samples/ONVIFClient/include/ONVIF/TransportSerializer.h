//
// TransportSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Transport_INCLUDED
#define TypeSerializer_ONVIF_Transport_INCLUDED


#include "ONVIF/Transport.h"
#include "ONVIF/TransportDeserializer.h"
#include "ONVIF/TransportSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Transport>
{
public:
	static void serialize(const std::string& name, const ONVIF::Transport& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Transport& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Protocol"s,"Tunnel"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getProtocol(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Transport>>::serialize(REMOTING__NAMES[1], value.getTunnel(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Transport_INCLUDED

