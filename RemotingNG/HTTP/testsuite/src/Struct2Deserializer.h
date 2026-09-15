//
// Struct2Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_Struct2_INCLUDED
#define TypeDeserializer_Struct2_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Struct1Deserializer.h"
#include "Struct1Serializer.h"
#include "Tester.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Struct2>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Struct2& value)
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

	static void deserializeImpl(Deserializer& deser, Struct2& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"aVector"s,"anotherVector"s,"anURI"s,"anUUID"s,"aDateTime"s,"aLocalDateTime"s,"aTimestamp"s};
		TypeDeserializer<std::vector<Struct1>>::deserialize(REMOTING__NAMES[0], true, deser, value.aVector);
		TypeDeserializer<std::vector<Poco::SharedPtr<Struct1>>>::deserialize(REMOTING__NAMES[1], true, deser, value.anotherVector);
		TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[2], true, deser, value.anURI);
		TypeDeserializer<Poco::UUID>::deserialize(REMOTING__NAMES[3], true, deser, value.anUUID);
		TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[4], true, deser, value.aDateTime);
		TypeDeserializer<Poco::LocalDateTime>::deserialize(REMOTING__NAMES[5], true, deser, value.aLocalDateTime);
		TypeDeserializer<Poco::Timestamp>::deserialize(REMOTING__NAMES[6], true, deser, value.aTimestamp);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Struct2_INCLUDED

