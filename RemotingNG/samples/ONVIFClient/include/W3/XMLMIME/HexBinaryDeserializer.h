//
// HexBinaryDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_W3_XMLMIME_HexBinary_INCLUDED
#define TypeDeserializer_W3_XMLMIME_HexBinary_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "W3/XMLMIME/HexBinary.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<W3::XMLMIME::HexBinary>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, W3::XMLMIME::HexBinary& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.w3.org/2005/05/xmlmime"s);
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

	static void deserializeImpl(Deserializer& deser, W3::XMLMIME::HexBinary& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"value"s};
		bool ret = false;
		Poco::Optional<std::string> gen_value;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_value);
		if (ret) value.setValue(gen_value);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_W3_XMLMIME_HexBinary_INCLUDED

