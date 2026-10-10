//
// TopicDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_T1_Topic_INCLUDED
#define TypeDeserializer_OASIS_WSN_T1_Topic_INCLUDED


#include "OASIS/WSN/T1/DocumentationDeserializer.h"
#include "OASIS/WSN/T1/DocumentationSerializer.h"
#include "OASIS/WSN/T1/Topic.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::T1::Topic>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::T1::Topic& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://docs.oasis-open.org/wsn/t-1"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, OASIS::WSN::T1::Topic& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"parent"s};
		bool ret = false;
		Poco::Optional<std::string> gen_parent;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_parent);
		if (ret) value.setParent(gen_parent);
	}

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::T1::Topic& value)
	{
		using namespace std::string_literals;
		
		// OASIS::WSN::T1::ExtensibleDocumented
		{
		static const std::string REMOTING__NAMES__OASIS__WSN__T1__EXTENSIBLEDOCUMENTED[] = {"documentation"s};
		bool ret = false;
		Poco::SharedPtr<OASIS::WSN::T1::Documentation> gen_documentation;
		ret = TypeDeserializer<Poco::SharedPtr<OASIS::WSN::T1::Documentation>>::deserialize(REMOTING__NAMES__OASIS__WSN__T1__EXTENSIBLEDOCUMENTED[0], false, deser, gen_documentation);
		if (ret) value.setDocumentation(gen_documentation);
		}
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"parent"s,"http://docs.oasis-open.org/wsn/t-1"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_T1_Topic_INCLUDED

