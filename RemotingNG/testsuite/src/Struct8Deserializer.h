//
// Struct8Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_Struct8_INCLUDED
#define TypeDeserializer_Struct8_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Struct8>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Struct8& value)
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

	static void deserializeImpl(Deserializer& deser, Struct8& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"pocoOptional"s};
		TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], true, deser, value.pocoOptional);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Struct8_INCLUDED

