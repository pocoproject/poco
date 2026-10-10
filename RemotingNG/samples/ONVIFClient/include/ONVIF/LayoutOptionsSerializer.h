//
// LayoutOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_LayoutOptions_INCLUDED
#define TypeSerializer_ONVIF_LayoutOptions_INCLUDED


#include "ONVIF/LayoutOptions.h"
#include "ONVIF/LayoutOptionsExtensionDeserializer.h"
#include "ONVIF/LayoutOptionsExtensionSerializer.h"
#include "ONVIF/PaneLayoutOptionsDeserializer.h"
#include "ONVIF/PaneLayoutOptionsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::LayoutOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::LayoutOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::LayoutOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PaneLayoutOptions"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::PaneLayoutOptions>>::serialize(REMOTING__NAMES[0], value.getPaneLayoutOptions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::LayoutOptionsExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_LayoutOptions_INCLUDED

