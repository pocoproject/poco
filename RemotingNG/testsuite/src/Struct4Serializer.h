//
// Struct4Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_Struct4_INCLUDED
#define TypeSerializer_Struct4_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "Struct4Deserializer.h"
#include "Struct4Serializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Struct4>
{
public:
	static void serialize(const std::string& name, const Struct4& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Struct4& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"value"s,"vec"s,"ptr"s,""s,"http://www.appinf.com/types"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.value, ser);
		TypeSerializer<std::vector<Struct4>>::serialize(REMOTING__NAMES[1], value.vec, ser);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[4]);
		TypeSerializer<Poco::SharedPtr<Struct4>>::serialize(REMOTING__NAMES[2], value.ptr, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Struct4_INCLUDED

