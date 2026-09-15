//
// MessageDescriptionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_MessageDescription_INCLUDED
#define TypeSerializer_ONVIF_MessageDescription_INCLUDED


#include "ONVIF/ItemListDescriptionDeserializer.h"
#include "ONVIF/ItemListDescriptionSerializer.h"
#include "ONVIF/MessageDescription.h"
#include "ONVIF/MessageDescriptionExtensionDeserializer.h"
#include "ONVIF/MessageDescriptionExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::MessageDescription>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IsProperty"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::MessageDescription& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::MessageDescription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IsProperty"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getIsProperty(), ser);
	}

	static void serializeImpl(const ONVIF::MessageDescription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Source"s,"Key"s,"Data"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::ItemListDescription>>::serialize(REMOTING__NAMES[0], value.getSource(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ItemListDescription>>::serialize(REMOTING__NAMES[1], value.getKey(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ItemListDescription>>::serialize(REMOTING__NAMES[2], value.getData(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MessageDescriptionExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_MessageDescription_INCLUDED

