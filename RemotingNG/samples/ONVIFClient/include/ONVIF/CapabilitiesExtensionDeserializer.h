//
// CapabilitiesExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_CapabilitiesExtension_INCLUDED
#define TypeDeserializer_ONVIF_CapabilitiesExtension_INCLUDED


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
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::CapabilitiesExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::CapabilitiesExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::CapabilitiesExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DeviceIO"s,"Display"s,"Recording"s,"Search"s,"Replay"s,"Receiver"s,"AnalyticsDevice"s,"Extensions"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::DeviceIOCapabilities> gen_deviceIO;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DeviceIOCapabilities>>::deserialize(REMOTING__NAMES[0], false, deser, gen_deviceIO);
		if (ret) value.setDeviceIO(gen_deviceIO);
		Poco::SharedPtr<ONVIF::DisplayCapabilities> gen_display;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DisplayCapabilities>>::deserialize(REMOTING__NAMES[1], false, deser, gen_display);
		if (ret) value.setDisplay(gen_display);
		Poco::SharedPtr<ONVIF::RecordingCapabilities> gen_recording;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RecordingCapabilities>>::deserialize(REMOTING__NAMES[2], false, deser, gen_recording);
		if (ret) value.setRecording(gen_recording);
		Poco::SharedPtr<ONVIF::SearchCapabilities> gen_search;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SearchCapabilities>>::deserialize(REMOTING__NAMES[3], false, deser, gen_search);
		if (ret) value.setSearch(gen_search);
		Poco::SharedPtr<ONVIF::ReplayCapabilities> gen_replay;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ReplayCapabilities>>::deserialize(REMOTING__NAMES[4], false, deser, gen_replay);
		if (ret) value.setReplay(gen_replay);
		Poco::SharedPtr<ONVIF::ReceiverCapabilities> gen_receiver;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ReceiverCapabilities>>::deserialize(REMOTING__NAMES[5], false, deser, gen_receiver);
		if (ret) value.setReceiver(gen_receiver);
		Poco::SharedPtr<ONVIF::AnalyticsDeviceCapabilities> gen_analyticsDevice;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AnalyticsDeviceCapabilities>>::deserialize(REMOTING__NAMES[6], false, deser, gen_analyticsDevice);
		if (ret) value.setAnalyticsDevice(gen_analyticsDevice);
		Poco::SharedPtr<ONVIF::CapabilitiesExtension2> gen_extensions;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::CapabilitiesExtension2>>::deserialize(REMOTING__NAMES[7], false, deser, gen_extensions);
		if (ret) value.setExtensions(gen_extensions);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_CapabilitiesExtension_INCLUDED

