//
// ObjectSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_Object_INCLUDED
#define TypeSerializer_Object_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Object>
{
public:
	static void serialize(const std::string& name, const Object& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Object& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"data"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.data, ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Object_INCLUDED

