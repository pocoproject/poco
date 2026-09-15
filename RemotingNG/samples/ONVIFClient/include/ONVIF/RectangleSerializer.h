//
// RectangleSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Rectangle_INCLUDED
#define TypeSerializer_ONVIF_Rectangle_INCLUDED


#include "ONVIF/Rectangle.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Rectangle>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"bottom"s,"top"s,"right"s,"left"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[3]);
	}

	static void serialize(const std::string& name, const ONVIF::Rectangle& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::Rectangle& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"bottom"s,"top"s,"right"s,"left"s};
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[0], value.getBottom(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[1], value.getTop(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[2], value.getRight(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[3], value.getLeft(), ser);
	}

	static void serializeImpl(const ONVIF::Rectangle& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Rectangle_INCLUDED

