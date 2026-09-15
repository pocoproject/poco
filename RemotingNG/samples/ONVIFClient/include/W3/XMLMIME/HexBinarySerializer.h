//
// HexBinarySerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_W3_XMLMIME_HexBinary_INCLUDED
#define TypeSerializer_W3_XMLMIME_HexBinary_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "W3/XMLMIME/HexBinary.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<W3::XMLMIME::HexBinary>
{
public:
	static void serialize(const std::string& name, const W3::XMLMIME::HexBinary& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.w3.org/2005/05/xmlmime"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const W3::XMLMIME::HexBinary& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"value"s,""s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getValue(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_W3_XMLMIME_HexBinary_INCLUDED

