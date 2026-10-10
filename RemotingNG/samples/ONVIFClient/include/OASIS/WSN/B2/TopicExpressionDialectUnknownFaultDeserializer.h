//
// TopicExpressionDialectUnknownFaultDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_B2_TopicExpressionDialectUnknownFault_INCLUDED
#define TypeDeserializer_OASIS_WSN_B2_TopicExpressionDialectUnknownFault_INCLUDED


#include "OASIS/WSN/B2/TopicExpressionDialectUnknownFault.h"
#include "OASIS/WSRF/BF2/DescriptionDeserializer.h"
#include "OASIS/WSRF/BF2/DescriptionSerializer.h"
#include "OASIS/WSRF/BF2/ErrorCodeDeserializer.h"
#include "OASIS/WSRF/BF2/ErrorCodeSerializer.h"
#include "OASIS/WSRF/BF2/FaultCauseDeserializer.h"
#include "OASIS/WSRF/BF2/FaultCauseSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "W3/Addressing/EndpointReferenceDeserializer.h"
#include "W3/Addressing/EndpointReferenceSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::B2::TopicExpressionDialectUnknownFault>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::B2::TopicExpressionDialectUnknownFault& value)
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

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::B2::TopicExpressionDialectUnknownFault& value)
	{
		using namespace std::string_literals;
		
		// OASIS::WSRF::BF2::BaseFault
		{
		static const std::string REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[] = {"Timestamp"s,"Originator"s,"ErrorCode"s,"Description"s,"FaultCause"s};
		bool ret = false;
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_timestamp;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[0], true, deser, gen_timestamp);
		if (ret) value.setTimestamp(gen_timestamp);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		Poco::SharedPtr<W3::Addressing::EndpointReference> gen_originator;
		ret = TypeDeserializer<Poco::SharedPtr<W3::Addressing::EndpointReference>>::deserialize(REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[1], false, deser, gen_originator);
		if (ret) value.setOriginator(gen_originator);
		Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode> gen_errorCode;
		ret = TypeDeserializer<Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>>::deserialize(REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[2], false, deser, gen_errorCode);
		if (ret) value.setErrorCode(gen_errorCode);
		std::vector<OASIS::WSRF::BF2::Description> gen_description;
		ret = TypeDeserializer<std::vector<OASIS::WSRF::BF2::Description>>::deserialize(REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[3], false, deser, gen_description);
		if (ret) value.setDescription(gen_description);
		Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause> gen_faultCause;
		ret = TypeDeserializer<Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>>::deserialize(REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[4], false, deser, gen_faultCause);
		if (ret) value.setFaultCause(gen_faultCause);
		}
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_B2_TopicExpressionDialectUnknownFault_INCLUDED

