//
// PTZPresetTourSpotOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZPresetTourSpotOptions_INCLUDED
#define TypeSerializer_ONVIF_PTZPresetTourSpotOptions_INCLUDED


#include "ONVIF/DurationRangeDeserializer.h"
#include "ONVIF/DurationRangeSerializer.h"
#include "ONVIF/PTZPresetTourPresetDetailOptionsDeserializer.h"
#include "ONVIF/PTZPresetTourPresetDetailOptionsSerializer.h"
#include "ONVIF/PTZPresetTourSpotOptions.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZPresetTourSpotOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZPresetTourSpotOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZPresetTourSpotOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PresetDetail"s,"StayTime"s,""s};
		TypeSerializer<ONVIF::PTZPresetTourPresetDetailOptions>::serialize(REMOTING__NAMES[0], value.getPresetDetail(), ser);
		TypeSerializer<ONVIF::DurationRange>::serialize(REMOTING__NAMES[1], value.getStayTime(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZPresetTourSpotOptions_INCLUDED

