//
// QueryExpressionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_OASIS_WSN_T1_QueryExpression_INCLUDED
#define TypeSerializer_OASIS_WSN_T1_QueryExpression_INCLUDED


#include "OASIS/WSN/T1/QueryExpression.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<OASIS::WSN::T1::QueryExpression>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dialect"s,"http://docs.oasis-open.org/wsn/t-1"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const OASIS::WSN::T1::QueryExpression& value, Serializer& ser)
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

	static void serializeAttributes(const OASIS::WSN::T1::QueryExpression& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Dialect"s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getDialect(), ser);
	}

	static void serializeImpl(const OASIS::WSN::T1::QueryExpression& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_OASIS_WSN_T1_QueryExpression_INCLUDED

