//
// MulticastConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MulticastConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_MulticastConfiguration_INCLUDED


#include "ONVIF/IPAddressDeserializer.h"
#include "ONVIF/IPAddressSerializer.h"
#include "ONVIF/MulticastConfiguration.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MulticastConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MulticastConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::MulticastConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Address"s,"Port"s,"TTL"s,"AutoStart"s};
		bool ret = false;
		ONVIF::IPAddress gen_address;
		ret = TypeDeserializer<ONVIF::IPAddress>::deserialize(REMOTING__NAMES[0], true, deser, gen_address);
		if (ret) value.setAddress(gen_address);
		int gen_port;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_port);
		if (ret) value.setPort(gen_port);
		int gen_tTL;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_tTL);
		if (ret) value.setTTL(gen_tTL);
		bool gen_autoStart;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[3], true, deser, gen_autoStart);
		if (ret) value.setAutoStart(gen_autoStart);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MulticastConfiguration_INCLUDED

