//
// PTControlDirectionOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTControlDirectionOptions_INCLUDED
#define TypeSerializer_ONVIF_PTControlDirectionOptions_INCLUDED


#include "ONVIF/EFlipOptionsDeserializer.h"
#include "ONVIF/EFlipOptionsSerializer.h"
#include "ONVIF/PTControlDirectionOptions.h"
#include "ONVIF/PTControlDirectionOptionsExtensionDeserializer.h"
#include "ONVIF/PTControlDirectionOptionsExtensionSerializer.h"
#include "ONVIF/ReverseOptionsDeserializer.h"
#include "ONVIF/ReverseOptionsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTControlDirectionOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTControlDirectionOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTControlDirectionOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"EFlip"s,"Reverse"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::EFlipOptions>>::serialize(REMOTING__NAMES[0], value.getEFlip(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ReverseOptions>>::serialize(REMOTING__NAMES[1], value.getReverse(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTControlDirectionOptionsExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTControlDirectionOptions_INCLUDED

