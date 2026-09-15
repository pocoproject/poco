//
// SubscribeResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_B2_SubscribeResponse_INCLUDED
#define TypeDeserializer_OASIS_WSN_B2_SubscribeResponse_INCLUDED


#include "OASIS/WSN/B2/SubscribeResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "W3/Addressing/EndpointReferenceDeserializer.h"
#include "W3/Addressing/EndpointReferenceSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::B2::SubscribeResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::B2::SubscribeResponse& value)
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

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::B2::SubscribeResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SubscriptionReference"s,"CurrentTime"s,"TerminationTime"s};
		bool ret = false;
		W3::Addressing::EndpointReference gen_subscriptionReference;
		ret = TypeDeserializer<W3::Addressing::EndpointReference>::deserialize(REMOTING__NAMES[0], true, deser, gen_subscriptionReference);
		if (ret) value.setSubscriptionReference(gen_subscriptionReference);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::Optional<Poco::DateTime> gen_currentTime;
		ret = TypeDeserializer<Poco::Optional<Poco::DateTime>>::deserialize(REMOTING__NAMES[1], false, deser, gen_currentTime);
		if (ret) value.setCurrentTime(gen_currentTime);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::Nullable<Poco::DateTime> gen_terminationTime;
		ret = TypeDeserializer<Poco::Nullable<Poco::DateTime>>::deserialize(REMOTING__NAMES[2], false, deser, gen_terminationTime);
		if (ret) value.setTerminationTime(gen_terminationTime);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_B2_SubscribeResponse_INCLUDED

