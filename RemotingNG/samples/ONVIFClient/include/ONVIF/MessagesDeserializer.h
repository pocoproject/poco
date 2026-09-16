//
// MessagesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Messages_INCLUDED
#define TypeDeserializer_ONVIF_Messages_INCLUDED


#include "ONVIF/ItemListDescriptionDeserializer.h"
#include "ONVIF/ItemListDescriptionSerializer.h"
#include "ONVIF/MessageDescriptionExtensionDeserializer.h"
#include "ONVIF/MessageDescriptionExtensionSerializer.h"
#include "ONVIF/Messages.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Messages>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Messages& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::Messages& value)
	{
		// ONVIF::MessageDescription
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__MESSAGEDESCRIPTION[] = {"IsProperty"s};
		bool ret = false;
		Poco::Optional<bool> gen_isProperty;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES__ONVIF__MESSAGEDESCRIPTION[0], false, deser, gen_isProperty);
		if (ret) value.setIsProperty(gen_isProperty);
		}
		
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Messages& value)
	{
		using namespace std::string_literals;
		
		// ONVIF::MessageDescription
		{
		static const std::string REMOTING__NAMES__ONVIF__MESSAGEDESCRIPTION[] = {"Source"s,"Key"s,"Data"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::ItemListDescription> gen_source;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ItemListDescription>>::deserialize(REMOTING__NAMES__ONVIF__MESSAGEDESCRIPTION[0], false, deser, gen_source);
		if (ret) value.setSource(gen_source);
		Poco::SharedPtr<ONVIF::ItemListDescription> gen_key;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ItemListDescription>>::deserialize(REMOTING__NAMES__ONVIF__MESSAGEDESCRIPTION[1], false, deser, gen_key);
		if (ret) value.setKey(gen_key);
		Poco::SharedPtr<ONVIF::ItemListDescription> gen_data;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ItemListDescription>>::deserialize(REMOTING__NAMES__ONVIF__MESSAGEDESCRIPTION[2], false, deser, gen_data);
		if (ret) value.setData(gen_data);
		Poco::SharedPtr<ONVIF::MessageDescriptionExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MessageDescriptionExtension>>::deserialize(REMOTING__NAMES__ONVIF__MESSAGEDESCRIPTION[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
		}
		
		static const std::string REMOTING__NAMES[] = {"ParentTopic"s};
		bool ret = false;
		std::string gen_parentTopic;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_parentTopic);
		if (ret) value.setParentTopic(gen_parentTopic);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		// ONVIF::MessageDescription
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__MESSAGEDESCRIPTION[] = {"IsProperty"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES__ONVIF__MESSAGEDESCRIPTION[1], REMOTING__NAMES__ONVIF__MESSAGEDESCRIPTION[0], false);
		}
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Messages_INCLUDED

