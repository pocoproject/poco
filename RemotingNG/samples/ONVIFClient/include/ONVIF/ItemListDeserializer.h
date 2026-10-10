//
// ItemListDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ItemList_INCLUDED
#define TypeDeserializer_ONVIF_ItemList_INCLUDED


#include "ONVIF/ElementItemDeserializer.h"
#include "ONVIF/ElementItemSerializer.h"
#include "ONVIF/ItemList.h"
#include "ONVIF/ItemListExtensionDeserializer.h"
#include "ONVIF/ItemListExtensionSerializer.h"
#include "ONVIF/SimpleItemDeserializer.h"
#include "ONVIF/SimpleItemSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ItemList>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ItemList& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::ItemList& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SimpleItem"s,"ElementItem"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::SimpleItem> gen_simpleItem;
		ret = TypeDeserializer<std::vector<ONVIF::SimpleItem>>::deserialize(REMOTING__NAMES[0], false, deser, gen_simpleItem);
		if (ret) value.setSimpleItem(gen_simpleItem);
		std::vector<ONVIF::ElementItem> gen_elementItem;
		ret = TypeDeserializer<std::vector<ONVIF::ElementItem>>::deserialize(REMOTING__NAMES[1], false, deser, gen_elementItem);
		if (ret) value.setElementItem(gen_elementItem);
		Poco::SharedPtr<ONVIF::ItemListExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ItemListExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ItemList_INCLUDED

