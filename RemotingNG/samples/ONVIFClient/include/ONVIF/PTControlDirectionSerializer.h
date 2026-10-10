//
// PTControlDirectionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTControlDirection_INCLUDED
#define TypeSerializer_ONVIF_PTControlDirection_INCLUDED


#include "ONVIF/EFlipDeserializer.h"
#include "ONVIF/EFlipSerializer.h"
#include "ONVIF/PTControlDirection.h"
#include "ONVIF/PTControlDirectionExtensionDeserializer.h"
#include "ONVIF/PTControlDirectionExtensionSerializer.h"
#include "ONVIF/ReverseDeserializer.h"
#include "ONVIF/ReverseSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTControlDirection>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTControlDirection& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTControlDirection& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"EFlip"s,"Reverse"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::EFlip>>::serialize(REMOTING__NAMES[0], value.getEFlip(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Reverse>>::serialize(REMOTING__NAMES[1], value.getReverse(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTControlDirectionExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTControlDirection_INCLUDED

