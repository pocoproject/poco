//
// DeviceCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_DeviceCapabilities_INCLUDED
#define TypeSerializer_ONVIF_DeviceCapabilities_INCLUDED


#include "ONVIF/DeviceCapabilities.h"
#include "ONVIF/DeviceCapabilitiesExtensionDeserializer.h"
#include "ONVIF/DeviceCapabilitiesExtensionSerializer.h"
#include "ONVIF/IOCapabilitiesDeserializer.h"
#include "ONVIF/IOCapabilitiesSerializer.h"
#include "ONVIF/NetworkCapabilitiesDeserializer.h"
#include "ONVIF/NetworkCapabilitiesSerializer.h"
#include "ONVIF/SecurityCapabilitiesDeserializer.h"
#include "ONVIF/SecurityCapabilitiesSerializer.h"
#include "ONVIF/SystemCapabilitiesDeserializer.h"
#include "ONVIF/SystemCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::DeviceCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::DeviceCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::DeviceCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"Network"s,"System"s,"IO"s,"Security"s,"Extension"s,""s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getXAddr(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NetworkCapabilities>>::serialize(REMOTING__NAMES[1], value.getNetwork(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SystemCapabilities>>::serialize(REMOTING__NAMES[2], value.getSystem(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IOCapabilities>>::serialize(REMOTING__NAMES[3], value.getIO(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SecurityCapabilities>>::serialize(REMOTING__NAMES[4], value.getSecurity(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::DeviceCapabilitiesExtension>>::serialize(REMOTING__NAMES[5], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_DeviceCapabilities_INCLUDED

