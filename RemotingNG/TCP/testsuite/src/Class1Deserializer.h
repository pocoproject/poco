//
// Class1Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_Class1_INCLUDED
#define TypeDeserializer_Class1_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Class1>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Class1& value)
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

	static void deserializeImpl(Deserializer& deser, Class1& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"anEnum"s,"anInt"s,"aString"s};
		bool ret = false;
		int gen_anEnum;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_anEnum);
		if (ret) value.setAnEnum(static_cast<Enum1>(gen_anEnum));
		int gen_anInt;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_anInt);
		if (ret) value.setAnInt(gen_anInt);
		std::string gen_aString;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[2], true, deser, gen_aString);
		if (ret) value.setAString(gen_aString);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Class1_INCLUDED

