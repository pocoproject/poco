//
// OSDTextConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_OSDTextConfiguration_INCLUDED
#define TypeSerializer_ONVIF_OSDTextConfiguration_INCLUDED


#include "ONVIF/OSDColorDeserializer.h"
#include "ONVIF/OSDColorSerializer.h"
#include "ONVIF/OSDTextConfiguration.h"
#include "ONVIF/OSDTextConfigurationExtensionDeserializer.h"
#include "ONVIF/OSDTextConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::OSDTextConfiguration>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IsPersistentText"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::OSDTextConfiguration& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::OSDTextConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IsPersistentText"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getIsPersistentText(), ser);
	}

	static void serializeImpl(const ONVIF::OSDTextConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"DateFormat"s,"TimeFormat"s,"FontSize"s,"FontColor"s,"BackgroundColor"s,"PlainText"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getType(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getDateFormat(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getTimeFormat(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[3], value.getFontSize(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDColor>>::serialize(REMOTING__NAMES[4], value.getFontColor(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDColor>>::serialize(REMOTING__NAMES[5], value.getBackgroundColor(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[6], value.getPlainText(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDTextConfigurationExtension>>::serialize(REMOTING__NAMES[7], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_OSDTextConfiguration_INCLUDED

