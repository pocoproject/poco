//
// FocusStatus20Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_FocusStatus20_INCLUDED
#define TypeSerializer_ONVIF_FocusStatus20_INCLUDED


#include "ONVIF/FocusStatus20.h"
#include "ONVIF/FocusStatus20ExtensionDeserializer.h"
#include "ONVIF/FocusStatus20ExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::FocusStatus20>
{
public:
	static void serialize(const std::string& name, const ONVIF::FocusStatus20& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::FocusStatus20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Position"s,"MoveStatus"s,"Error"s,"Extension"s,""s};
		TypeSerializer<float>::serialize(REMOTING__NAMES[0], value.getPosition(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getMoveStatus(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getError(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FocusStatus20Extension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_FocusStatus20_INCLUDED

