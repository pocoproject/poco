//
// ColorSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Color_INCLUDED
#define TypeSerializer_ONVIF_Color_INCLUDED


#include "ONVIF/Color.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Color>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"X"s,"Y"s,"Z"s,"Colorspace"s,"Likelihood"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[4]);
	}

	static void serialize(const std::string& name, const ONVIF::Color& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::Color& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"X"s,"Y"s,"Z"s,"Colorspace"s,"Likelihood"s};
		TypeSerializer<float>::serialize(REMOTING__NAMES[0], value.getX(), ser);
		TypeSerializer<float>::serialize(REMOTING__NAMES[1], value.getY(), ser);
		TypeSerializer<float>::serialize(REMOTING__NAMES[2], value.getZ(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[3], value.getColorspace(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[4], value.getLikelihood(), ser);
	}

	static void serializeImpl(const ONVIF::Color& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Color_INCLUDED

