//
// TopicNamespaceSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_OASIS_WSN_T1_TopicNamespace_INCLUDED
#define TypeSerializer_OASIS_WSN_T1_TopicNamespace_INCLUDED


#include "OASIS/WSN/T1/DocumentationDeserializer.h"
#include "OASIS/WSN/T1/DocumentationSerializer.h"
#include "OASIS/WSN/T1/TopicDeserializer.h"
#include "OASIS/WSN/T1/TopicNamespace.h"
#include "OASIS/WSN/T1/TopicSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<OASIS::WSN::T1::TopicNamespace>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"name"s,"targetNamespace"s,"final"s,"http://docs.oasis-open.org/wsn/t-1"s};
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2]);
	}

	static void serialize(const std::string& name, const OASIS::WSN::T1::TopicNamespace& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://docs.oasis-open.org/wsn/t-1"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const OASIS::WSN::T1::TopicNamespace& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"name"s,"targetNamespace"s,"final"s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getName(), ser);
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[1], value.getTargetNamespace(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getFinal(), ser);
	}

	static void serializeImpl(const OASIS::WSN::T1::TopicNamespace& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// OASIS::WSN::T1::ExtensibleDocumented
		{
		static const std::string REMOTING__NAMES__OASIS__WSN__T1__EXTENSIBLEDOCUMENTED[] = {"documentation"s,""s};
		TypeSerializer<Poco::SharedPtr<OASIS::WSN::T1::Documentation>>::serialize(REMOTING__NAMES__OASIS__WSN__T1__EXTENSIBLEDOCUMENTED[0], value.getDocumentation(), ser);
		}
		
		static const std::string REMOTING__NAMES[] = {"Topic"s,""s};
		TypeSerializer<std::vector<OASIS::WSN::T1::Topic>>::serialize(REMOTING__NAMES[0], value.getTopic(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_OASIS_WSN_T1_TopicNamespace_INCLUDED

