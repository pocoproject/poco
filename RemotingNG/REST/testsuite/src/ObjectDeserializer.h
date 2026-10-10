//
// ObjectDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_Object_INCLUDED
#define TypeDeserializer_Object_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Object>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Object& value)
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

	static void deserializeImpl(Deserializer& deser, Object& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"data"s};
		TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, value.data);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Object_INCLUDED

