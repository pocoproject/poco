//
// PresetTourSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PresetTour_INCLUDED
#define TypeSerializer_ONVIF_PresetTour_INCLUDED


#include "ONVIF/PTZPresetTourExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourExtensionSerializer.h"
#include "ONVIF/PTZPresetTourSpotDeserializer.h"
#include "ONVIF/PTZPresetTourSpotSerializer.h"
#include "ONVIF/PTZPresetTourStartingConditionDeserializer.h"
#include "ONVIF/PTZPresetTourStartingConditionSerializer.h"
#include "ONVIF/PTZPresetTourStatusDeserializer.h"
#include "ONVIF/PTZPresetTourStatusSerializer.h"
#include "ONVIF/PresetTour.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PresetTour>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::PresetTour& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::PresetTour& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getToken(), ser);
	}

	static void serializeImpl(const ONVIF::PresetTour& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"Status"s,"AutoStart"s,"StartingCondition"s,"TourSpot"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getName(), ser);
		TypeSerializer<ONVIF::PTZPresetTourStatus>::serialize(REMOTING__NAMES[1], value.getStatus(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[2], value.getAutoStart(), ser);
		TypeSerializer<ONVIF::PTZPresetTourStartingCondition>::serialize(REMOTING__NAMES[3], value.getStartingCondition(), ser);
		TypeSerializer<std::vector<ONVIF::PTZPresetTourSpot>>::serialize(REMOTING__NAMES[4], value.getTourSpot(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZPresetTourExtension>>::serialize(REMOTING__NAMES[5], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PresetTour_INCLUDED

