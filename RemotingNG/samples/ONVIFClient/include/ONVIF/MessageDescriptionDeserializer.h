//
// MessageDescriptionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MessageDescription_INCLUDED
#define TypeDeserializer_ONVIF_MessageDescription_INCLUDED


#include "ONVIF/ItemListDescriptionDeserializer.h"
#include "ONVIF/ItemListDescriptionSerializer.h"
#include "ONVIF/MessageDescription.h"
#include "ONVIF/MessageDescriptionExtensionDeserializer.h"
#include "ONVIF/MessageDescriptionExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MessageDescription>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MessageDescription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::MessageDescription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IsProperty"s};
		bool ret = false;
		Poco::Optional<bool> gen_isProperty;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_isProperty);
		if (ret) value.setIsProperty(gen_isProperty);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::MessageDescription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Source"s,"Key"s,"Data"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::ItemListDescription> gen_source;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ItemListDescription>>::deserialize(REMOTING__NAMES[0], false, deser, gen_source);
		if (ret) value.setSource(gen_source);
		Poco::SharedPtr<ONVIF::ItemListDescription> gen_key;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ItemListDescription>>::deserialize(REMOTING__NAMES[1], false, deser, gen_key);
		if (ret) value.setKey(gen_key);
		Poco::SharedPtr<ONVIF::ItemListDescription> gen_data;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ItemListDescription>>::deserialize(REMOTING__NAMES[2], false, deser, gen_data);
		if (ret) value.setData(gen_data);
		Poco::SharedPtr<ONVIF::MessageDescriptionExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MessageDescriptionExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IsProperty"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MessageDescription_INCLUDED

