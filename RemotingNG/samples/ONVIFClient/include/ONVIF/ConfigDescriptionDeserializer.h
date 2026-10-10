//
// ConfigDescriptionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ConfigDescription_INCLUDED
#define TypeDeserializer_ONVIF_ConfigDescription_INCLUDED


#include "ONVIF/ConfigDescription.h"
#include "ONVIF/ConfigDescriptionExtensionDeserializer.h"
#include "ONVIF/ConfigDescriptionExtensionSerializer.h"
#include "ONVIF/ItemListDescriptionDeserializer.h"
#include "ONVIF/ItemListDescriptionSerializer.h"
#include "ONVIF/MessagesDeserializer.h"
#include "ONVIF/MessagesSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ConfigDescription>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ConfigDescription& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::ConfigDescription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"fixed"s,"maxInstances"s};
		bool ret = false;
		std::string gen_name;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_name);
		if (ret) value.setName(gen_name);
		Poco::Optional<bool> gen_fixed;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_fixed);
		if (ret) value.setFixed(gen_fixed);
		Poco::Optional<int> gen_maxInstances;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[2], false, deser, gen_maxInstances);
		if (ret) value.setMaxInstances(gen_maxInstances);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::ConfigDescription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Parameters"s,"Messages"s,"Extension"s};
		bool ret = false;
		ONVIF::ItemListDescription gen_parameters;
		ret = TypeDeserializer<ONVIF::ItemListDescription>::deserialize(REMOTING__NAMES[0], true, deser, gen_parameters);
		if (ret) value.setParameters(gen_parameters);
		std::vector<ONVIF::Messages> gen_messages;
		ret = TypeDeserializer<std::vector<ONVIF::Messages>>::deserialize(REMOTING__NAMES[1], false, deser, gen_messages);
		if (ret) value.setMessages(gen_messages);
		Poco::SharedPtr<ONVIF::ConfigDescriptionExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ConfigDescriptionExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"fixed"s,"maxInstances"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ConfigDescription_INCLUDED

