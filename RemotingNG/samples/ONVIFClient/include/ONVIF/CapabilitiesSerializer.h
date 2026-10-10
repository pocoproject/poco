//
// CapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Capabilities_INCLUDED
#define TypeSerializer_ONVIF_Capabilities_INCLUDED


#include "ONVIF/AnalyticsCapabilitiesDeserializer.h"
#include "ONVIF/AnalyticsCapabilitiesSerializer.h"
#include "ONVIF/Capabilities.h"
#include "ONVIF/CapabilitiesExtensionDeserializer.h"
#include "ONVIF/CapabilitiesExtensionSerializer.h"
#include "ONVIF/DeviceCapabilitiesDeserializer.h"
#include "ONVIF/DeviceCapabilitiesSerializer.h"
#include "ONVIF/EventCapabilitiesDeserializer.h"
#include "ONVIF/EventCapabilitiesSerializer.h"
#include "ONVIF/ImagingCapabilitiesDeserializer.h"
#include "ONVIF/ImagingCapabilitiesSerializer.h"
#include "ONVIF/MediaCapabilitiesDeserializer.h"
#include "ONVIF/MediaCapabilitiesSerializer.h"
#include "ONVIF/PTZCapabilitiesDeserializer.h"
#include "ONVIF/PTZCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Capabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::Capabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Capabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Analytics"s,"Device"s,"Events"s,"Imaging"s,"Media"s,"PTZ"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::AnalyticsCapabilities>>::serialize(REMOTING__NAMES[0], value.getAnalytics(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::DeviceCapabilities>>::serialize(REMOTING__NAMES[1], value.getDevice(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::EventCapabilities>>::serialize(REMOTING__NAMES[2], value.getEvents(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingCapabilities>>::serialize(REMOTING__NAMES[3], value.getImaging(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MediaCapabilities>>::serialize(REMOTING__NAMES[4], value.getMedia(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZCapabilities>>::serialize(REMOTING__NAMES[5], value.getPTZ(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::CapabilitiesExtension>>::serialize(REMOTING__NAMES[6], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Capabilities_INCLUDED

