//
// PrefixedIPv6AddressDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PrefixedIPv6Address_INCLUDED
#define TypeDeserializer_ONVIF_PrefixedIPv6Address_INCLUDED


#include "ONVIF/PrefixedIPv6Address.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PrefixedIPv6Address>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PrefixedIPv6Address& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PrefixedIPv6Address& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Address"s,"PrefixLength"s};
		bool ret = false;
		std::string gen_address;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_address);
		if (ret) value.setAddress(gen_address);
		int gen_prefixLength;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_prefixLength);
		if (ret) value.setPrefixLength(gen_prefixLength);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PrefixedIPv6Address_INCLUDED

