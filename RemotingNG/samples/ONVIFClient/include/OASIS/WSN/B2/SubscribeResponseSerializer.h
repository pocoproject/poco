//
// SubscribeResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_OASIS_WSN_B2_SubscribeResponse_INCLUDED
#define TypeSerializer_OASIS_WSN_B2_SubscribeResponse_INCLUDED


#include "OASIS/WSN/B2/SubscribeResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "W3/Addressing/EndpointReferenceDeserializer.h"
#include "W3/Addressing/EndpointReferenceSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<OASIS::WSN::B2::SubscribeResponse>
{
public:
	static void serialize(const std::string& name, const OASIS::WSN::B2::SubscribeResponse& value, Serializer& ser)
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

	static void serializeImpl(const OASIS::WSN::B2::SubscribeResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SubscriptionReference"s,"CurrentTime"s,"TerminationTime"s,""s};
		TypeSerializer<W3::Addressing::EndpointReference>::serialize(REMOTING__NAMES[0], value.getSubscriptionReference(), ser);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::Optional<Poco::DateTime>>::serialize(REMOTING__NAMES[1], value.getCurrentTime(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::Nullable<Poco::DateTime>>::serialize(REMOTING__NAMES[2], value.getTerminationTime(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_OASIS_WSN_B2_SubscribeResponse_INCLUDED

