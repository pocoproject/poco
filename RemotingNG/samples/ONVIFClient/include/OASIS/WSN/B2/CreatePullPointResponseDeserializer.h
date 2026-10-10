//
// CreatePullPointResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_B2_CreatePullPointResponse_INCLUDED
#define TypeDeserializer_OASIS_WSN_B2_CreatePullPointResponse_INCLUDED


#include "OASIS/WSN/B2/CreatePullPointResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "W3/Addressing/EndpointReferenceDeserializer.h"
#include "W3/Addressing/EndpointReferenceSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::B2::CreatePullPointResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::B2::CreatePullPointResponse& value)
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

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::B2::CreatePullPointResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PullPoint"s};
		bool ret = false;
		W3::Addressing::EndpointReference gen_pullPoint;
		ret = TypeDeserializer<W3::Addressing::EndpointReference>::deserialize(REMOTING__NAMES[0], true, deser, gen_pullPoint);
		if (ret) value.setPullPoint(gen_pullPoint);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_B2_CreatePullPointResponse_INCLUDED

