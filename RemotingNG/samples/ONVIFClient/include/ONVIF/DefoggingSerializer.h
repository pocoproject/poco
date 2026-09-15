//
// DefoggingSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Defogging_INCLUDED
#define TypeSerializer_ONVIF_Defogging_INCLUDED


#include "ONVIF/Defogging.h"
#include "ONVIF/DefoggingExtensionDeserializer.h"
#include "ONVIF/DefoggingExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Defogging>
{
public:
	static void serialize(const std::string& name, const ONVIF::Defogging& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Defogging& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Level"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getMode(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[1], value.getLevel(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::DefoggingExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Defogging_INCLUDED

