//
// NetworkCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkCapabilities_INCLUDED
#define TypeSerializer_ONVIF_NetworkCapabilities_INCLUDED


#include "ONVIF/NetworkCapabilities.h"
#include "ONVIF/NetworkCapabilitiesExtensionDeserializer.h"
#include "ONVIF/NetworkCapabilitiesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::NetworkCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NetworkCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IPFilter"s,"ZeroConfiguration"s,"IPVersion6"s,"DynDNS"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getIPFilter(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getZeroConfiguration(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getIPVersion6(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[3], value.getDynDNS(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkCapabilitiesExtension>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkCapabilities_INCLUDED

