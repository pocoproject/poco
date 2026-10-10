//
// NetworkHostSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkHost_INCLUDED
#define TypeSerializer_ONVIF_NetworkHost_INCLUDED


#include "ONVIF/NetworkHost.h"
#include "ONVIF/NetworkHostDeserializer.h"
#include "ONVIF/NetworkHostExtensionDeserializer.h"
#include "ONVIF/NetworkHostExtensionSerializer.h"
#include "ONVIF/NetworkHostSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkHost>
{
public:
	static void serialize(const std::string& name, const ONVIF::NetworkHost& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NetworkHost& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"IPv4Address"s,"IPv6Address"s,"DNSname"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkHost>>::serialize(REMOTING__NAMES[0], value.getType(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getIPv4Address(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getIPv6Address(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[3], value.getDNSname(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkHostExtension>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkHost_INCLUDED

