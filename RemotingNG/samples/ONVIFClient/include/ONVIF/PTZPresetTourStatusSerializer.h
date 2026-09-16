//
// PTZPresetTourStatusSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZPresetTourStatus_INCLUDED
#define TypeSerializer_ONVIF_PTZPresetTourStatus_INCLUDED


#include "ONVIF/PTZPresetTourSpotDeserializer.h"
#include "ONVIF/PTZPresetTourSpotSerializer.h"
#include "ONVIF/PTZPresetTourStatus.h"
#include "ONVIF/PTZPresetTourStatusExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourStatusExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZPresetTourStatus>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZPresetTourStatus& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZPresetTourStatus& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"State"s,"CurrentTourSpot"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getState(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZPresetTourSpot>>::serialize(REMOTING__NAMES[1], value.getCurrentTourSpot(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZPresetTourStatusExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZPresetTourStatus_INCLUDED

