//
// Struct6Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_Struct6_INCLUDED
#define TypeSerializer_Struct6_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Struct6>
{
public:
	static void serialize(const std::string& name, const Struct6& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Struct6& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"arr"s,""s};
		TypeSerializer<Poco::Array<int, 4>>::serialize(REMOTING__NAMES[0], value.arr, ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Struct6_INCLUDED

