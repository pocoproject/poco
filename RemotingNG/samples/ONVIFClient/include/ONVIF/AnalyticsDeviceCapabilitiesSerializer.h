//
// AnalyticsDeviceCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AnalyticsDeviceCapabilities_INCLUDED
#define TypeSerializer_ONVIF_AnalyticsDeviceCapabilities_INCLUDED


#include "ONVIF/AnalyticsDeviceCapabilities.h"
#include "ONVIF/AnalyticsDeviceExtensionDeserializer.h"
#include "ONVIF/AnalyticsDeviceExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AnalyticsDeviceCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::AnalyticsDeviceCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::AnalyticsDeviceCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"RuleSupport"s,"Extension"s,""s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getXAddr(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getRuleSupport(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AnalyticsDeviceExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AnalyticsDeviceCapabilities_INCLUDED

