//
// ProblemActionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_W3_Addressing_ProblemAction_INCLUDED
#define TypeDeserializer_W3_Addressing_ProblemAction_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "W3/Addressing/AttributedURIDeserializer.h"
#include "W3/Addressing/AttributedURISerializer.h"
#include "W3/Addressing/ProblemAction.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<W3::Addressing::ProblemAction>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, W3::Addressing::ProblemAction& value)
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

	static void deserializeImpl(Deserializer& deser, W3::Addressing::ProblemAction& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Action"s,"SoapAction"s};
		bool ret = false;
		Poco::SharedPtr<W3::Addressing::AttributedURI> gen_action;
		ret = TypeDeserializer<Poco::SharedPtr<W3::Addressing::AttributedURI>>::deserialize(REMOTING__NAMES[0], false, deser, gen_action);
		if (ret) value.setAction(gen_action);
		Poco::Optional<Poco::URI> gen_soapAction;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[1], false, deser, gen_soapAction);
		if (ret) value.setSoapAction(gen_soapAction);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_W3_Addressing_ProblemAction_INCLUDED

