//
// IncludeSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_W3_XOP_Include_INCLUDED
#define TypeSerializer_W3_XOP_Include_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "W3/XOP/Include.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<W3::XOP::Include>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"href"s,"http://www.w3.org/2004/08/xop/include"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const W3::XOP::Include& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.w3.org/2004/08/xop/include"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const W3::XOP::Include& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"href"s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getHref(), ser);
	}

	static void serializeImpl(const W3::XOP::Include& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_W3_XOP_Include_INCLUDED

