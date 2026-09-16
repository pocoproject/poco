//
// FilterDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_OASIS_WSN_B2_Filter_INCLUDED
#define TypeDeserializer_OASIS_WSN_B2_Filter_INCLUDED


#include "OASIS/WSN/B2/Filter.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<OASIS::WSN::B2::Filter>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, OASIS::WSN::B2::Filter& value)
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

	static void deserializeImpl(Deserializer& deser, OASIS::WSN::B2::Filter& value)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_OASIS_WSN_B2_Filter_INCLUDED

