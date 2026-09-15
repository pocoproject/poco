//
// PTZPresetTourOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZPresetTourOptions_INCLUDED
#define TypeSerializer_ONVIF_PTZPresetTourOptions_INCLUDED


#include "ONVIF/PTZPresetTourOptions.h"
#include "ONVIF/PTZPresetTourSpotOptionsDeserializer.h"
#include "ONVIF/PTZPresetTourSpotOptionsSerializer.h"
#include "ONVIF/PTZPresetTourStartingConditionOptionsDeserializer.h"
#include "ONVIF/PTZPresetTourStartingConditionOptionsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZPresetTourOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZPresetTourOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZPresetTourOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AutoStart"s,"StartingCondition"s,"TourSpot"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getAutoStart(), ser);
		TypeSerializer<ONVIF::PTZPresetTourStartingConditionOptions>::serialize(REMOTING__NAMES[1], value.getStartingCondition(), ser);
		TypeSerializer<ONVIF::PTZPresetTourSpotOptions>::serialize(REMOTING__NAMES[2], value.getTourSpot(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZPresetTourOptions_INCLUDED

