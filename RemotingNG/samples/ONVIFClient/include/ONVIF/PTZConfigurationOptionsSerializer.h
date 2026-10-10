//
// PTZConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_PTZConfigurationOptions_INCLUDED


#include "ONVIF/DurationRangeDeserializer.h"
#include "ONVIF/DurationRangeSerializer.h"
#include "ONVIF/PTControlDirectionOptionsDeserializer.h"
#include "ONVIF/PTControlDirectionOptionsSerializer.h"
#include "ONVIF/PTZConfigurationOptions.h"
#include "ONVIF/PTZConfigurationOptions2Deserializer.h"
#include "ONVIF/PTZConfigurationOptions2Serializer.h"
#include "ONVIF/PTZSpacesDeserializer.h"
#include "ONVIF/PTZSpacesSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZConfigurationOptions>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PTZRamps"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::PTZConfigurationOptions& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::PTZConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PTZRamps"s};
		TypeSerializer<std::vector<std::vector<int>>>::serialize(REMOTING__NAMES[0], value.getPTZRamps(), ser);
	}

	static void serializeImpl(const ONVIF::PTZConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Spaces"s,"PTZTimeout"s,"PTControlDirection"s,"Extension"s,""s};
		TypeSerializer<ONVIF::PTZSpaces>::serialize(REMOTING__NAMES[0], value.getSpaces(), ser);
		TypeSerializer<ONVIF::DurationRange>::serialize(REMOTING__NAMES[1], value.getPTZTimeout(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTControlDirectionOptions>>::serialize(REMOTING__NAMES[2], value.getPTControlDirection(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZConfigurationOptions2>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZConfigurationOptions_INCLUDED

