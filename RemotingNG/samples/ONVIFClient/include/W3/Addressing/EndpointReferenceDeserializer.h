//
// EndpointReferenceDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_W3_Addressing_EndpointReference_INCLUDED
#define TypeDeserializer_W3_Addressing_EndpointReference_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "W3/Addressing/AttributedURIDeserializer.h"
#include "W3/Addressing/AttributedURISerializer.h"
#include "W3/Addressing/EndpointReference.h"
#include "W3/Addressing/MetadataDeserializer.h"
#include "W3/Addressing/MetadataSerializer.h"
#include "W3/Addressing/ReferenceParametersDeserializer.h"
#include "W3/Addressing/ReferenceParametersSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<W3::Addressing::EndpointReference>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, W3::Addressing::EndpointReference& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.w3.org/2005/08/addressing"s);
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

	static void deserializeImpl(Deserializer& deser, W3::Addressing::EndpointReference& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Address"s,"ReferenceParameters"s,"Metadata"s};
		bool ret = false;
		W3::Addressing::AttributedURI gen_address;
		ret = TypeDeserializer<W3::Addressing::AttributedURI>::deserialize(REMOTING__NAMES[0], true, deser, gen_address);
		if (ret) value.setAddress(gen_address);
		Poco::SharedPtr<W3::Addressing::ReferenceParameters> gen_referenceParameters;
		ret = TypeDeserializer<Poco::SharedPtr<W3::Addressing::ReferenceParameters>>::deserialize(REMOTING__NAMES[1], false, deser, gen_referenceParameters);
		if (ret) value.setReferenceParameters(gen_referenceParameters);
		Poco::SharedPtr<W3::Addressing::Metadata> gen_metadata;
		ret = TypeDeserializer<Poco::SharedPtr<W3::Addressing::Metadata>>::deserialize(REMOTING__NAMES[2], false, deser, gen_metadata);
		if (ret) value.setMetadata(gen_metadata);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_W3_Addressing_EndpointReference_INCLUDED

