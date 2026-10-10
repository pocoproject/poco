//
// NotificationProducerRPDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_B2_NotificationProducerRP_INCLUDED
#define TypeDeserializer_OASIS_WSN_B2_NotificationProducerRP_INCLUDED


#include "OASIS/WSN/B2/NotificationProducerRP.h"
#include "OASIS/WSN/B2/TopicExpressionDeserializer.h"
#include "OASIS/WSN/B2/TopicExpressionSerializer.h"
#include "OASIS/WSN/T1/TopicSetDeserializer.h"
#include "OASIS/WSN/T1/TopicSetSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::B2::NotificationProducerRP>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::B2::NotificationProducerRP& value)
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

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::B2::NotificationProducerRP& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TopicExpression"s,"FixedTopicSet"s,"TopicExpressionDialect"s,"TopicSet"s};
		bool ret = false;
		std::vector<OASIS::WSN::B2::TopicExpression> gen_topicExpression;
		ret = TypeDeserializer<std::vector<OASIS::WSN::B2::TopicExpression>>::deserialize(REMOTING__NAMES[0], false, deser, gen_topicExpression);
		if (ret) value.setTopicExpression(gen_topicExpression);
		Poco::Optional<bool> gen_fixedTopicSet;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_fixedTopicSet);
		if (ret) value.setFixedTopicSet(gen_fixedTopicSet);
		std::vector<Poco::URI> gen_topicExpressionDialect;
		ret = TypeDeserializer<std::vector<Poco::URI>>::deserialize(REMOTING__NAMES[2], false, deser, gen_topicExpressionDialect);
		if (ret) value.setTopicExpressionDialect(gen_topicExpressionDialect);
		Poco::SharedPtr<OASIS::WSN::T1::TopicSet> gen_topicSet;
		ret = TypeDeserializer<Poco::SharedPtr<OASIS::WSN::T1::TopicSet>>::deserialize(REMOTING__NAMES[3], false, deser, gen_topicSet);
		if (ret) value.setTopicSet(gen_topicSet);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_B2_NotificationProducerRP_INCLUDED

