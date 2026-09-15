//
// CapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Capabilities_INCLUDED
#define TypeDeserializer_ONVIF_Capabilities_INCLUDED


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
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Capabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Capabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Capabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Analytics"s,"Device"s,"Events"s,"Imaging"s,"Media"s,"PTZ"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::AnalyticsCapabilities> gen_analytics;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AnalyticsCapabilities>>::deserialize(REMOTING__NAMES[0], false, deser, gen_analytics);
		if (ret) value.setAnalytics(gen_analytics);
		Poco::SharedPtr<ONVIF::DeviceCapabilities> gen_device;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DeviceCapabilities>>::deserialize(REMOTING__NAMES[1], false, deser, gen_device);
		if (ret) value.setDevice(gen_device);
		Poco::SharedPtr<ONVIF::EventCapabilities> gen_events;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::EventCapabilities>>::deserialize(REMOTING__NAMES[2], false, deser, gen_events);
		if (ret) value.setEvents(gen_events);
		Poco::SharedPtr<ONVIF::ImagingCapabilities> gen_imaging;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingCapabilities>>::deserialize(REMOTING__NAMES[3], false, deser, gen_imaging);
		if (ret) value.setImaging(gen_imaging);
		Poco::SharedPtr<ONVIF::MediaCapabilities> gen_media;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MediaCapabilities>>::deserialize(REMOTING__NAMES[4], false, deser, gen_media);
		if (ret) value.setMedia(gen_media);
		Poco::SharedPtr<ONVIF::PTZCapabilities> gen_pTZ;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZCapabilities>>::deserialize(REMOTING__NAMES[5], false, deser, gen_pTZ);
		if (ret) value.setPTZ(gen_pTZ);
		Poco::SharedPtr<ONVIF::CapabilitiesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::CapabilitiesExtension>>::deserialize(REMOTING__NAMES[6], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Capabilities_INCLUDED

