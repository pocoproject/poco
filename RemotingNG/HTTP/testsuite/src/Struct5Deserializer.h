//
// Struct5Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_Struct5_INCLUDED
#define TypeDeserializer_Struct5_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Struct5>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Struct5& value)
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

	static void deserializeImpl(Deserializer& deser, Struct5& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"tv"s};
		TypeDeserializer<Poco::Int64>::deserialize(REMOTING__NAMES[0], true, deser, value.tv);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Struct5_INCLUDED

