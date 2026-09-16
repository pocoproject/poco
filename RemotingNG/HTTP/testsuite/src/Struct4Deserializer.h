//
// Struct4Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_Struct4_INCLUDED
#define TypeDeserializer_Struct4_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Struct4Deserializer.h"
#include "Struct4Serializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Struct4>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Struct4& value)
	{
		using namespace std::string_literals;
		
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.deserializeStructEnd(name);
		}
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, Struct4& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"value"s,"vec"s,"ptr"s};
		TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, value.value);
		TypeDeserializer<std::vector<Struct4>>::deserialize(REMOTING__NAMES[1], true, deser, value.vec);
		TypeDeserializer<Poco::SharedPtr<Struct4>>::deserialize(REMOTING__NAMES[2], true, deser, value.ptr);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Struct4_INCLUDED

