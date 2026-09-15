//
// GetCurrentMessageDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_B2_GetCurrentMessage_INCLUDED
#define TypeDeserializer_OASIS_WSN_B2_GetCurrentMessage_INCLUDED


#include "OASIS/WSN/B2/GetCurrentMessage.h"
#include "OASIS/WSN/B2/TopicExpressionDeserializer.h"
#include "OASIS/WSN/B2/TopicExpressionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::B2::GetCurrentMessage>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::B2::GetCurrentMessage& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://docs.oasis-open.org/wsn/b-2"s);
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

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::B2::GetCurrentMessage& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Topic"s};
		bool ret = false;
		OASIS::WSN::B2::TopicExpression gen_topic;
		ret = TypeDeserializer<OASIS::WSN::B2::TopicExpression>::deserialize(REMOTING__NAMES[0], true, deser, gen_topic);
		if (ret) value.setTopic(gen_topic);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_B2_GetCurrentMessage_INCLUDED

