//
// SystemCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_SystemCapabilities_INCLUDED
#define TypeSerializer_ONVIF_SystemCapabilities_INCLUDED


#include "ONVIF/OnvifVersionDeserializer.h"
#include "ONVIF/OnvifVersionSerializer.h"
#include "ONVIF/SystemCapabilities.h"
#include "ONVIF/SystemCapabilitiesExtensionDeserializer.h"
#include "ONVIF/SystemCapabilitiesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::SystemCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::SystemCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::SystemCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DiscoveryResolve"s,"DiscoveryBye"s,"RemoteDiscovery"s,"SystemBackup"s,"SystemLogging"s,"FirmwareUpgrade"s,"SupportedVersions"s,"Extension"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getDiscoveryResolve(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getDiscoveryBye(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[2], value.getRemoteDiscovery(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[3], value.getSystemBackup(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[4], value.getSystemLogging(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[5], value.getFirmwareUpgrade(), ser);
		TypeSerializer<std::vector<ONVIF::OnvifVersion>>::serialize(REMOTING__NAMES[6], value.getSupportedVersions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SystemCapabilitiesExtension>>::serialize(REMOTING__NAMES[7], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_SystemCapabilities_INCLUDED

