//
// UnableToDestroyPullPointFaultSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_OASIS_WSN_B2_UnableToDestroyPullPointFault_INCLUDED
#define TypeSerializer_OASIS_WSN_B2_UnableToDestroyPullPointFault_INCLUDED


#include "OASIS/WSN/B2/UnableToDestroyPullPointFault.h"
#include "OASIS/WSRF/BF2/DescriptionDeserializer.h"
#include "OASIS/WSRF/BF2/DescriptionSerializer.h"
#include "OASIS/WSRF/BF2/ErrorCodeDeserializer.h"
#include "OASIS/WSRF/BF2/ErrorCodeSerializer.h"
#include "OASIS/WSRF/BF2/FaultCauseDeserializer.h"
#include "OASIS/WSRF/BF2/FaultCauseSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "W3/Addressing/EndpointReferenceDeserializer.h"
#include "W3/Addressing/EndpointReferenceSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<OASIS::WSN::B2::UnableToDestroyPullPointFault>
{
public:
	static void serialize(const std::string& name, const OASIS::WSN::B2::UnableToDestroyPullPointFault& value, Serializer& ser)
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

	static void serializeImpl(const OASIS::WSN::B2::UnableToDestroyPullPointFault& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// OASIS::WSRF::BF2::BaseFault
		{
		static const std::string REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[] = {"Timestamp"s,"Originator"s,"ErrorCode"s,"Description"s,"FaultCause"s,""s};
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[0], value.getTimestamp(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
		TypeSerializer<Poco::SharedPtr<W3::Addressing::EndpointReference>>::serialize(REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[1], value.getOriginator(), ser);
		TypeSerializer<Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>>::serialize(REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[2], value.getErrorCode(), ser);
		TypeSerializer<std::vector<OASIS::WSRF::BF2::Description>>::serialize(REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[3], value.getDescription(), ser);
		TypeSerializer<Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>>::serialize(REMOTING__NAMES__OASIS__WSRF__BF2__BASEFAULT[4], value.getFaultCause(), ser);
		}
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_OASIS_WSN_B2_UnableToDestroyPullPointFault_INCLUDED

