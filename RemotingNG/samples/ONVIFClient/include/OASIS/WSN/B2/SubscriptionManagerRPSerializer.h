//
// SubscriptionManagerRPSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_OASIS_WSN_B2_SubscriptionManagerRP_INCLUDED
#define TypeSerializer_OASIS_WSN_B2_SubscriptionManagerRP_INCLUDED


#include "OASIS/WSN/B2/FilterDeserializer.h"
#include "OASIS/WSN/B2/FilterSerializer.h"
#include "OASIS/WSN/B2/SubscriptionManagerRP.h"
#include "OASIS/WSN/B2/SubscriptionPolicyDeserializer.h"
#include "OASIS/WSN/B2/SubscriptionPolicySerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "W3/Addressing/EndpointReferenceDeserializer.h"
#include "W3/Addressing/EndpointReferenceSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<OASIS::WSN::B2::SubscriptionManagerRP>
{
public:
	static void serialize(const std::string& name, const OASIS::WSN::B2::SubscriptionManagerRP& value, Serializer& ser)
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

	static void serializeImpl(const OASIS::WSN::B2::SubscriptionManagerRP& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ConsumerReference"s,"Filter"s,"SubscriptionPolicy"s,"CreationTime"s,""s};
		TypeSerializer<W3::Addressing::EndpointReference>::serialize(REMOTING__NAMES[0], value.getConsumerReference(), ser);
		TypeSerializer<Poco::SharedPtr<OASIS::WSN::B2::Filter>>::serialize(REMOTING__NAMES[1], value.getFilter(), ser);
		TypeSerializer<Poco::SharedPtr<OASIS::WSN::B2::SubscriptionPolicy>>::serialize(REMOTING__NAMES[2], value.getSubscriptionPolicy(), ser);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::Optional<Poco::DateTime>>::serialize(REMOTING__NAMES[3], value.getCreationTime(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_OASIS_WSN_B2_SubscriptionManagerRP_INCLUDED

