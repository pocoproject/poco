//
// NotificationMessageHolderSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_OASIS_WSN_B2_NotificationMessageHolder_INCLUDED
#define TypeSerializer_OASIS_WSN_B2_NotificationMessageHolder_INCLUDED


#include "OASIS/WSN/B2/MessageDeserializer.h"
#include "OASIS/WSN/B2/MessageSerializer.h"
#include "OASIS/WSN/B2/NotificationMessageHolder.h"
#include "OASIS/WSN/B2/TopicExpressionDeserializer.h"
#include "OASIS/WSN/B2/TopicExpressionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "W3/Addressing/EndpointReferenceDeserializer.h"
#include "W3/Addressing/EndpointReferenceSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<OASIS::WSN::B2::NotificationMessageHolder>
{
public:
	static void serialize(const std::string& name, const OASIS::WSN::B2::NotificationMessageHolder& value, Serializer& ser)
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

	static void serializeImpl(const OASIS::WSN::B2::NotificationMessageHolder& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SubscriptionReference"s,"Topic"s,"ProducerReference"s,"Message"s,""s};
		TypeSerializer<Poco::SharedPtr<W3::Addressing::EndpointReference>>::serialize(REMOTING__NAMES[0], value.getSubscriptionReference(), ser);
		TypeSerializer<Poco::SharedPtr<OASIS::WSN::B2::TopicExpression>>::serialize(REMOTING__NAMES[1], value.getTopic(), ser);
		TypeSerializer<Poco::SharedPtr<W3::Addressing::EndpointReference>>::serialize(REMOTING__NAMES[2], value.getProducerReference(), ser);
		TypeSerializer<OASIS::WSN::B2::Message>::serialize(REMOTING__NAMES[3], value.getMessage(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_OASIS_WSN_B2_NotificationMessageHolder_INCLUDED

