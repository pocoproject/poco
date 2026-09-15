//
// ErrorCodeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSRF_BF2_ErrorCode_INCLUDED
#define TypeDeserializer_OASIS_WSRF_BF2_ErrorCode_INCLUDED


#include "OASIS/WSRF/BF2/ErrorCode.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSRF::BF2::ErrorCode>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSRF::BF2::ErrorCode& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://docs.oasis-open.org/wsrf/bf-2"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, OASIS::WSRF::BF2::ErrorCode& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"value"s};
		bool ret = false;
		Poco::Optional<std::string> gen_value;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_value);
		if (ret) value.setValue(gen_value);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSRF_BF2_ErrorCode_INCLUDED

