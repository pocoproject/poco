//
// ItemListDescriptionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ItemListDescription_INCLUDED
#define TypeSerializer_ONVIF_ItemListDescription_INCLUDED


#include "ONVIF/ElementItemDescriptionDeserializer.h"
#include "ONVIF/ElementItemDescriptionSerializer.h"
#include "ONVIF/ItemListDescription.h"
#include "ONVIF/ItemListDescriptionExtensionDeserializer.h"
#include "ONVIF/ItemListDescriptionExtensionSerializer.h"
#include "ONVIF/SimpleItemDescriptionDeserializer.h"
#include "ONVIF/SimpleItemDescriptionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ItemListDescription>
{
public:
	static void serialize(const std::string& name, const ONVIF::ItemListDescription& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ItemListDescription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SimpleItemDescription"s,"ElementItemDescription"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::SimpleItemDescription>>::serialize(REMOTING__NAMES[0], value.getSimpleItemDescription(), ser);
		TypeSerializer<std::vector<ONVIF::ElementItemDescription>>::serialize(REMOTING__NAMES[1], value.getElementItemDescription(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ItemListDescriptionExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ItemListDescription_INCLUDED

