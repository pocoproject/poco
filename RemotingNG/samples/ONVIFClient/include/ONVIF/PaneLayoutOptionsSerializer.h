//
// PaneLayoutOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PaneLayoutOptions_INCLUDED
#define TypeSerializer_ONVIF_PaneLayoutOptions_INCLUDED


#include "ONVIF/PaneLayoutOptions.h"
#include "ONVIF/PaneOptionExtensionDeserializer.h"
#include "ONVIF/PaneOptionExtensionSerializer.h"
#include "ONVIF/RectangleDeserializer.h"
#include "ONVIF/RectangleSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PaneLayoutOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::PaneLayoutOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PaneLayoutOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Area"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::Rectangle>>::serialize(REMOTING__NAMES[0], value.getArea(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PaneOptionExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PaneLayoutOptions_INCLUDED

