//
// ItemListSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ItemList_INCLUDED
#define TypeSerializer_ONVIF_ItemList_INCLUDED


#include "ONVIF/ElementItemDeserializer.h"
#include "ONVIF/ElementItemSerializer.h"
#include "ONVIF/ItemList.h"
#include "ONVIF/ItemListExtensionDeserializer.h"
#include "ONVIF/ItemListExtensionSerializer.h"
#include "ONVIF/SimpleItemDeserializer.h"
#include "ONVIF/SimpleItemSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ItemList>
{
public:
	static void serialize(const std::string& name, const ONVIF::ItemList& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ItemList& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SimpleItem"s,"ElementItem"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::SimpleItem>>::serialize(REMOTING__NAMES[0], value.getSimpleItem(), ser);
		TypeSerializer<std::vector<ONVIF::ElementItem>>::serialize(REMOTING__NAMES[1], value.getElementItem(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ItemListExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ItemList_INCLUDED

