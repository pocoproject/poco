//
// Dot11AvailableNetworksSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Dot11AvailableNetworks_INCLUDED
#define TypeSerializer_ONVIF_Dot11AvailableNetworks_INCLUDED


#include "ONVIF/Dot11AvailableNetworks.h"
#include "ONVIF/Dot11AvailableNetworksExtensionDeserializer.h"
#include "ONVIF/Dot11AvailableNetworksExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Dot11AvailableNetworks>
{
public:
	static void serialize(const std::string& name, const ONVIF::Dot11AvailableNetworks& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Dot11AvailableNetworks& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SSID"s,"BSSID"s,"AuthAndMangementSuite"s,"PairCipher"s,"GroupCipher"s,"SignalStrength"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getSSID(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getBSSID(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[2], value.getAuthAndMangementSuite(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[3], value.getPairCipher(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[4], value.getGroupCipher(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[5], value.getSignalStrength(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Dot11AvailableNetworksExtension>>::serialize(REMOTING__NAMES[6], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Dot11AvailableNetworks_INCLUDED

