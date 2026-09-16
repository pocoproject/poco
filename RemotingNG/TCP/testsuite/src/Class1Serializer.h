//
// Class1Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_Class1_INCLUDED
#define TypeSerializer_Class1_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Class1>
{
public:
	static void serialize(const std::string& name, const Class1& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Class1& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"anEnum"s,"anInt"s,"aString"s,""s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], static_cast<int>(value.getAnEnum()), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[1], value.getAnInt(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], value.getAString(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Class1_INCLUDED

