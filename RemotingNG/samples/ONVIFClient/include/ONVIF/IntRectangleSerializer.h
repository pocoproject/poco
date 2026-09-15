//
// IntRectangleSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_IntRectangle_INCLUDED
#define TypeSerializer_ONVIF_IntRectangle_INCLUDED


#include "ONVIF/IntRectangle.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::IntRectangle>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"x"s,"y"s,"width"s,"height"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[3]);
	}

	static void serialize(const std::string& name, const ONVIF::IntRectangle& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::IntRectangle& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"x"s,"y"s,"width"s,"height"s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], value.getX(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[1], value.getY(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[2], value.getWidth(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[3], value.getHeight(), ser);
	}

	static void serializeImpl(const ONVIF::IntRectangle& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_IntRectangle_INCLUDED

