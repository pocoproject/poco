//
// OSDColorOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_OSDColorOptions_INCLUDED
#define TypeSerializer_ONVIF_OSDColorOptions_INCLUDED


#include "ONVIF/ColorOptionsDeserializer.h"
#include "ONVIF/ColorOptionsSerializer.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/OSDColorOptions.h"
#include "ONVIF/OSDColorOptionsExtensionDeserializer.h"
#include "ONVIF/OSDColorOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::OSDColorOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::OSDColorOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::OSDColorOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Color"s,"Transparent"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::ColorOptions>>::serialize(REMOTING__NAMES[0], value.getColor(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::IntRange>>::serialize(REMOTING__NAMES[1], value.getTransparent(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDColorOptionsExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_OSDColorOptions_INCLUDED

