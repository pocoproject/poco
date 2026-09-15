//
// Struct1Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_Struct1_INCLUDED
#define TypeDeserializer_Struct1_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Struct1>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Struct1& value)
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

	static void deserializeImpl(Deserializer& deser, Struct1& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"aString"s,"anInt"s,"aDouble"s,"anEnum"s,"anEnum2"s};
		bool ret = false;
		TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, value.aString);
		TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, value.anInt);
		TypeDeserializer<double>::deserialize(REMOTING__NAMES[2], true, deser, value.aDouble);
		int genanEnum;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[3], true, deser, genanEnum);
		if (ret) value.anEnum = static_cast<Enum1>(genanEnum);
		int genanEnum2;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[4], true, deser, genanEnum2);
		if (ret) value.anEnum2 = static_cast<Struct1::Enum2>(genanEnum2);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Struct1_INCLUDED

