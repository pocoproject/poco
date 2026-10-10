//
// IOCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_IOCapabilities_INCLUDED
#define TypeSerializer_ONVIF_IOCapabilities_INCLUDED


#include "ONVIF/IOCapabilities.h"
#include "ONVIF/IOCapabilitiesExtensionDeserializer.h"
#include "ONVIF/IOCapabilitiesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::IOCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::IOCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::IOCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"InputConnectors"s,"RelayOutputs"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[0], value.getInputConnectors(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[1], value.getRelayOutputs(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IOCapabilitiesExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_IOCapabilities_INCLUDED

