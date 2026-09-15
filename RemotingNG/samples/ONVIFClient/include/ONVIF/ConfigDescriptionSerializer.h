//
// ConfigDescriptionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ConfigDescription_INCLUDED
#define TypeSerializer_ONVIF_ConfigDescription_INCLUDED


#include "ONVIF/ConfigDescription.h"
#include "ONVIF/ConfigDescriptionExtensionDeserializer.h"
#include "ONVIF/ConfigDescriptionExtensionSerializer.h"
#include "ONVIF/ItemListDescriptionDeserializer.h"
#include "ONVIF/ItemListDescriptionSerializer.h"
#include "ONVIF/MessagesDeserializer.h"
#include "ONVIF/MessagesSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ConfigDescription>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"fixed"s,"maxInstances"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2]);
	}

	static void serialize(const std::string& name, const ONVIF::ConfigDescription& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::ConfigDescription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"fixed"s,"maxInstances"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getName(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getFixed(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[2], value.getMaxInstances(), ser);
	}

	static void serializeImpl(const ONVIF::ConfigDescription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Parameters"s,"Messages"s,"Extension"s,""s};
		TypeSerializer<ONVIF::ItemListDescription>::serialize(REMOTING__NAMES[0], value.getParameters(), ser);
		TypeSerializer<std::vector<ONVIF::Messages>>::serialize(REMOTING__NAMES[1], value.getMessages(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ConfigDescriptionExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ConfigDescription_INCLUDED

