//
// ExtensibleDocumentedDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_T1_ExtensibleDocumented_INCLUDED
#define TypeDeserializer_OASIS_WSN_T1_ExtensibleDocumented_INCLUDED


#include "OASIS/WSN/T1/DocumentationDeserializer.h"
#include "OASIS/WSN/T1/DocumentationSerializer.h"
#include "OASIS/WSN/T1/ExtensibleDocumented.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::T1::ExtensibleDocumented>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::T1::ExtensibleDocumented& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://docs.oasis-open.org/wsn/t-1"s);
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

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::T1::ExtensibleDocumented& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"documentation"s};
		bool ret = false;
		Poco::SharedPtr<OASIS::WSN::T1::Documentation> gen_documentation;
		ret = TypeDeserializer<Poco::SharedPtr<OASIS::WSN::T1::Documentation>>::deserialize(REMOTING__NAMES[0], false, deser, gen_documentation);
		if (ret) value.setDocumentation(gen_documentation);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_T1_ExtensibleDocumented_INCLUDED

