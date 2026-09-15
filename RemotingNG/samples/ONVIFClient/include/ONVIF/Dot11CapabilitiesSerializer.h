//
// Dot11CapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Dot11Capabilities_INCLUDED
#define TypeSerializer_ONVIF_Dot11Capabilities_INCLUDED


#include "ONVIF/Dot11Capabilities.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Dot11Capabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::Dot11Capabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Dot11Capabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TKIP"s,"ScanAvailableNetworks"s,"MultipleConfiguration"s,"AdHocStationMode"s,"WEP"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getTKIP(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getScanAvailableNetworks(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[2], value.getMultipleConfiguration(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[3], value.getAdHocStationMode(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[4], value.getWEP(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Dot11Capabilities_INCLUDED

