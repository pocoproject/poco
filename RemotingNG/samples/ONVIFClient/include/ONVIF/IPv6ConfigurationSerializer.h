//
// IPv6ConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_IPv6Configuration_INCLUDED
#define TypeSerializer_ONVIF_IPv6Configuration_INCLUDED


#include "ONVIF/IPv6Configuration.h"
#include "ONVIF/IPv6ConfigurationExtensionDeserializer.h"
#include "ONVIF/IPv6ConfigurationExtensionSerializer.h"
#include "ONVIF/PrefixedIPv6AddressDeserializer.h"
#include "ONVIF/PrefixedIPv6AddressSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::IPv6Configuration>
{
public:
	static void serialize(const std::string& name, const ONVIF::IPv6Configuration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::IPv6Configuration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AcceptRouterAdvert"s,"DHCP"s,"Manual"s,"LinkLocal"s,"FromDHCP"s,"FromRA"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getAcceptRouterAdvert(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getDHCP(), ser);
		TypeSerializer<std::vector<ONVIF::PrefixedIPv6Address>>::serialize(REMOTING__NAMES[2], value.getManual(), ser);
		TypeSerializer<std::vector<ONVIF::PrefixedIPv6Address>>::serialize(REMOTING__NAMES[3], value.getLinkLocal(), ser);
		TypeSerializer<std::vector<ONVIF::PrefixedIPv6Address>>::serialize(REMOTING__NAMES[4], value.getFromDHCP(), ser);
		TypeSerializer<std::vector<ONVIF::PrefixedIPv6Address>>::serialize(REMOTING__NAMES[5], value.getFromRA(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IPv6ConfigurationExtension>>::serialize(REMOTING__NAMES[6], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_IPv6Configuration_INCLUDED

