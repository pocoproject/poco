//
// IntItemsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_IntItems_INCLUDED
#define TypeDeserializer_ONVIF_IntItems_INCLUDED


#include "ONVIF/IntItems.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::IntItems>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::IntItems& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::IntItems& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Items"s};
		bool ret = false;
		std::vector<int> gen_items;
		ret = TypeDeserializer<std::vector<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_items);
		if (ret) value.setItems(gen_items);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_IntItems_INCLUDED

