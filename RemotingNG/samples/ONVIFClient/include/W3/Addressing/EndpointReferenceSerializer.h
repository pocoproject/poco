//
// EndpointReferenceSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_W3_Addressing_EndpointReference_INCLUDED
#define TypeSerializer_W3_Addressing_EndpointReference_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
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
class TypeSerializer<W3::Addressing::EndpointReference>
{
public:
	static void serialize(const std::string& name, const W3::Addressing::EndpointReference& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.w3.org/2005/08/addressing"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const W3::Addressing::EndpointReference& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Address"s,"ReferenceParameters"s,"Metadata"s,""s};
		TypeSerializer<W3::Addressing::AttributedURI>::serialize(REMOTING__NAMES[0], value.getAddress(), ser);
		TypeSerializer<Poco::SharedPtr<W3::Addressing::ReferenceParameters>>::serialize(REMOTING__NAMES[1], value.getReferenceParameters(), ser);
		TypeSerializer<Poco::SharedPtr<W3::Addressing::Metadata>>::serialize(REMOTING__NAMES[2], value.getMetadata(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_W3_Addressing_EndpointReference_INCLUDED

