//
// Struct1Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_Struct1_INCLUDED
#define TypeSerializer_Struct1_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Struct1>
{
public:
	static void serialize(const std::string& name, const Struct1& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Struct1& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"aString"s,"anInt"s,"aDouble"s,"anEnum"s,"anEnum2"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.aString, ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[1], value.anInt, ser);
		TypeSerializer<double>::serialize(REMOTING__NAMES[2], value.aDouble, ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[3], static_cast<int>(value.anEnum), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[4], static_cast<int>(value.anEnum2), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Struct1_INCLUDED

