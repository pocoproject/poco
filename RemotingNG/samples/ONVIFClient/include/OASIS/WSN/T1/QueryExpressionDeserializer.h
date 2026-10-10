//
// QueryExpressionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_T1_QueryExpression_INCLUDED
#define TypeDeserializer_OASIS_WSN_T1_QueryExpression_INCLUDED


#include "OASIS/WSN/T1/QueryExpression.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::T1::QueryExpression>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::T1::QueryExpression& value)
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

	static void deserializeAttributes(Deserializer& deser, OASIS::WSN::T1::QueryExpression& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dialect"s};
		bool ret = false;
		Poco::URI gen_dialect;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_dialect);
		if (ret) value.setDialect(gen_dialect);
	}

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::T1::QueryExpression& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dialect"s,"http://docs.oasis-open.org/wsn/t-1"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], true);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_T1_QueryExpression_INCLUDED

