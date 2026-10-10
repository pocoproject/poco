//
// TopicSetSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_OASIS_WSN_T1_TopicSet_INCLUDED
#define TypeSerializer_OASIS_WSN_T1_TopicSet_INCLUDED


#include "OASIS/WSN/T1/DocumentationDeserializer.h"
#include "OASIS/WSN/T1/DocumentationSerializer.h"
#include "OASIS/WSN/T1/TopicSet.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<OASIS::WSN::T1::TopicSet>
{
public:
	static void serialize(const std::string& name, const OASIS::WSN::T1::TopicSet& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://docs.oasis-open.org/wsn/t-1"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const OASIS::WSN::T1::TopicSet& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// OASIS::WSN::T1::ExtensibleDocumented
		{
		static const std::string REMOTING__NAMES__OASIS__WSN__T1__EXTENSIBLEDOCUMENTED[] = {"documentation"s,""s};
		TypeSerializer<Poco::SharedPtr<OASIS::WSN::T1::Documentation>>::serialize(REMOTING__NAMES__OASIS__WSN__T1__EXTENSIBLEDOCUMENTED[0], value.getDocumentation(), ser);
		}
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_OASIS_WSN_T1_TopicSet_INCLUDED

