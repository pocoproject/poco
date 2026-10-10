//
// PTZPresetTourSpotSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZPresetTourSpot_INCLUDED
#define TypeSerializer_ONVIF_PTZPresetTourSpot_INCLUDED


#include "ONVIF/PTZPresetTourPresetDetailDeserializer.h"
#include "ONVIF/PTZPresetTourPresetDetailSerializer.h"
#include "ONVIF/PTZPresetTourSpot.h"
#include "ONVIF/PTZPresetTourSpotExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourSpotExtensionSerializer.h"
#include "ONVIF/PTZSpeedDeserializer.h"
#include "ONVIF/PTZSpeedSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZPresetTourSpot>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZPresetTourSpot& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZPresetTourSpot& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PresetDetail"s,"Speed"s,"StayTime"s,"Extension"s,""s};
		TypeSerializer<ONVIF::PTZPresetTourPresetDetail>::serialize(REMOTING__NAMES[0], value.getPresetDetail(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZSpeed>>::serialize(REMOTING__NAMES[1], value.getSpeed(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getStayTime(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZPresetTourSpotExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZPresetTourSpot_INCLUDED

