//
// NotificationProducerRPSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_OASIS_WSN_B2_NotificationProducerRP_INCLUDED
#define TypeSerializer_OASIS_WSN_B2_NotificationProducerRP_INCLUDED


#include "OASIS/WSN/B2/NotificationProducerRP.h"
#include "OASIS/WSN/B2/TopicExpressionDeserializer.h"
#include "OASIS/WSN/B2/TopicExpressionSerializer.h"
#include "OASIS/WSN/T1/TopicSetDeserializer.h"
#include "OASIS/WSN/T1/TopicSetSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<OASIS::WSN::B2::NotificationProducerRP>
{
public:
	static void serialize(const std::string& name, const OASIS::WSN::B2::NotificationProducerRP& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://docs.oasis-open.org/wsn/b-2"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const OASIS::WSN::B2::NotificationProducerRP& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TopicExpression"s,"FixedTopicSet"s,"TopicExpressionDialect"s,"TopicSet"s,""s,"http://docs.oasis-open.org/wsn/t-1"s};
		TypeSerializer<std::vector<OASIS::WSN::B2::TopicExpression>>::serialize(REMOTING__NAMES[0], value.getTopicExpression(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getFixedTopicSet(), ser);
		TypeSerializer<std::vector<Poco::URI>>::serialize(REMOTING__NAMES[2], value.getTopicExpressionDialect(), ser);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[5]);
		TypeSerializer<Poco::SharedPtr<OASIS::WSN::T1::TopicSet>>::serialize(REMOTING__NAMES[3], value.getTopicSet(), ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_OASIS_WSN_B2_NotificationProducerRP_INCLUDED

