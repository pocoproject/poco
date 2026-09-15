//
// OSDTextOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_OSDTextOptions_INCLUDED
#define TypeSerializer_ONVIF_OSDTextOptions_INCLUDED


#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/OSDColorOptionsDeserializer.h"
#include "ONVIF/OSDColorOptionsSerializer.h"
#include "ONVIF/OSDTextOptions.h"
#include "ONVIF/OSDTextOptionsExtensionDeserializer.h"
#include "ONVIF/OSDTextOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::OSDTextOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::OSDTextOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::OSDTextOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"FontSizeRange"s,"DateFormat"s,"TimeFormat"s,"FontColor"s,"BackgroundColor"s,"Extension"s,""s};
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[0], value.getType(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IntRange>>::serialize(REMOTING__NAMES[1], value.getFontSizeRange(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[2], value.getDateFormat(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[3], value.getTimeFormat(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDColorOptions>>::serialize(REMOTING__NAMES[4], value.getFontColor(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDColorOptions>>::serialize(REMOTING__NAMES[5], value.getBackgroundColor(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDTextOptionsExtension>>::serialize(REMOTING__NAMES[6], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_OSDTextOptions_INCLUDED

