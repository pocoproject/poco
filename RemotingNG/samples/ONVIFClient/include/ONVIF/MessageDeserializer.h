//
// MessageDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Message_INCLUDED
#define TypeDeserializer_ONVIF_Message_INCLUDED


#include "ONVIF/ItemListDeserializer.h"
#include "ONVIF/ItemListSerializer.h"
#include "ONVIF/Message.h"
#include "ONVIF/MessageExtensionDeserializer.h"
#include "ONVIF/MessageExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Message>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Message& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::Message& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"UtcTime"s,"PropertyOperation"s};
		bool ret = false;
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_utcTime;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[0], true, deser, gen_utcTime);
		if (ret) value.setUtcTime(gen_utcTime);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		Poco::Optional<std::string> gen_propertyOperation;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_propertyOperation);
		if (ret) value.setPropertyOperation(gen_propertyOperation);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Message& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Source"s,"Key"s,"Data"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::ItemList> gen_source;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ItemList>>::deserialize(REMOTING__NAMES[0], false, deser, gen_source);
		if (ret) value.setSource(gen_source);
		Poco::SharedPtr<ONVIF::ItemList> gen_key;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ItemList>>::deserialize(REMOTING__NAMES[1], false, deser, gen_key);
		if (ret) value.setKey(gen_key);
		Poco::SharedPtr<ONVIF::ItemList> gen_data;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ItemList>>::deserialize(REMOTING__NAMES[2], false, deser, gen_data);
		if (ret) value.setData(gen_data);
		Poco::SharedPtr<ONVIF::MessageExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MessageExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"UtcTime"s,"PropertyOperation"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Message_INCLUDED

