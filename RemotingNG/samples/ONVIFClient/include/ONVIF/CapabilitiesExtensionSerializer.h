//
// CapabilitiesExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_CapabilitiesExtension_INCLUDED
#define TypeSerializer_ONVIF_CapabilitiesExtension_INCLUDED


#include "ONVIF/AnalyticsDeviceCapabilitiesDeserializer.h"
#include "ONVIF/AnalyticsDeviceCapabilitiesSerializer.h"
#include "ONVIF/CapabilitiesExtension.h"
#include "ONVIF/CapabilitiesExtension2Deserializer.h"
#include "ONVIF/CapabilitiesExtension2Serializer.h"
#include "ONVIF/DeviceIOCapabilitiesDeserializer.h"
#include "ONVIF/DeviceIOCapabilitiesSerializer.h"
#include "ONVIF/DisplayCapabilitiesDeserializer.h"
#include "ONVIF/DisplayCapabilitiesSerializer.h"
#include "ONVIF/ReceiverCapabilitiesDeserializer.h"
#include "ONVIF/ReceiverCapabilitiesSerializer.h"
#include "ONVIF/RecordingCapabilitiesDeserializer.h"
#include "ONVIF/RecordingCapabilitiesSerializer.h"
#include "ONVIF/ReplayCapabilitiesDeserializer.h"
#include "ONVIF/ReplayCapabilitiesSerializer.h"
#include "ONVIF/SearchCapabilitiesDeserializer.h"
#include "ONVIF/SearchCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::CapabilitiesExtension>
{
public:
	static void serialize(const std::string& name, const ONVIF::CapabilitiesExtension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::CapabilitiesExtension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DeviceIO"s,"Display"s,"Recording"s,"Search"s,"Replay"s,"Receiver"s,"AnalyticsDevice"s,"Extensions"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::DeviceIOCapabilities>>::serialize(REMOTING__NAMES[0], value.getDeviceIO(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::DisplayCapabilities>>::serialize(REMOTING__NAMES[1], value.getDisplay(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::RecordingCapabilities>>::serialize(REMOTING__NAMES[2], value.getRecording(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SearchCapabilities>>::serialize(REMOTING__NAMES[3], value.getSearch(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ReplayCapabilities>>::serialize(REMOTING__NAMES[4], value.getReplay(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ReceiverCapabilities>>::serialize(REMOTING__NAMES[5], value.getReceiver(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AnalyticsDeviceCapabilities>>::serialize(REMOTING__NAMES[6], value.getAnalyticsDevice(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::CapabilitiesExtension2>>::serialize(REMOTING__NAMES[7], value.getExtensions(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_CapabilitiesExtension_INCLUDED

