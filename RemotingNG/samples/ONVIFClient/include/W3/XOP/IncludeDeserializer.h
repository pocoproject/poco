//
// IncludeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_W3_XOP_Include_INCLUDED
#define TypeDeserializer_W3_XOP_Include_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "W3/XOP/Include.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<W3::XOP::Include>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, W3::XOP::Include& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.w3.org/2004/08/xop/include"s);
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

	static void deserializeAttributes(Deserializer& deser, W3::XOP::Include& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"href"s};
		bool ret = false;
		Poco::URI gen_href;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_href);
		if (ret) value.setHref(gen_href);
	}

	static void deserializeImpl(Deserializer& deser, W3::XOP::Include& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"href"s,"http://www.w3.org/2004/08/xop/include"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], true);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_W3_XOP_Include_INCLUDED

