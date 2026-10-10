//
// NetworkProtocolSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkProtocol_INCLUDED
#define TypeSerializer_ONVIF_NetworkProtocol_INCLUDED


#include "ONVIF/NetworkProtocol.h"
#include "ONVIF/NetworkProtocolDeserializer.h"
#include "ONVIF/NetworkProtocolExtensionDeserializer.h"
#include "ONVIF/NetworkProtocolExtensionSerializer.h"
#include "ONVIF/NetworkProtocolSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkProtocol>
{
public:
	static void serialize(const std::string& name, const ONVIF::NetworkProtocol& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NetworkProtocol& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"Enabled"s,"Port"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkProtocol>>::serialize(REMOTING__NAMES[0], value.getName(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getEnabled(), ser);
		TypeSerializer<std::vector<int>>::serialize(REMOTING__NAMES[2], value.getPort(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkProtocolExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkProtocol_INCLUDED

