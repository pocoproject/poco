//
// PolylineArraySerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PolylineArray_INCLUDED
#define TypeSerializer_ONVIF_PolylineArray_INCLUDED


#include "ONVIF/PolylineArray.h"
#include "ONVIF/PolylineArrayExtensionDeserializer.h"
#include "ONVIF/PolylineArrayExtensionSerializer.h"
#include "ONVIF/PolylineDeserializer.h"
#include "ONVIF/PolylineSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PolylineArray>
{
public:
	static void serialize(const std::string& name, const ONVIF::PolylineArray& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PolylineArray& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Segment"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::Polyline>>::serialize(REMOTING__NAMES[0], value.getSegment(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PolylineArrayExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PolylineArray_INCLUDED

