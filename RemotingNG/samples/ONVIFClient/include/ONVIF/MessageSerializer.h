//
// MessageSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Message_INCLUDED
#define TypeSerializer_ONVIF_Message_INCLUDED


#include "ONVIF/ItemListDeserializer.h"
#include "ONVIF/ItemListSerializer.h"
#include "ONVIF/Message.h"
#include "ONVIF/MessageExtensionDeserializer.h"
#include "ONVIF/MessageExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Message>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"UtcTime"s,"PropertyOperation"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1]);
	}

	static void serialize(const std::string& name, const ONVIF::Message& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::Message& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"UtcTime"s,"PropertyOperation"s};
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES[0], value.getUtcTime(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getPropertyOperation(), ser);
	}

	static void serializeImpl(const ONVIF::Message& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Source"s,"Key"s,"Data"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::ItemList>>::serialize(REMOTING__NAMES[0], value.getSource(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ItemList>>::serialize(REMOTING__NAMES[1], value.getKey(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ItemList>>::serialize(REMOTING__NAMES[2], value.getData(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MessageExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Message_INCLUDED

