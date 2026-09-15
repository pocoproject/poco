//
// NetworkInterfaceConnectionSettingDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NetworkInterfaceConnectionSetting_INCLUDED
#define TypeDeserializer_ONVIF_NetworkInterfaceConnectionSetting_INCLUDED


#include "ONVIF/NetworkInterfaceConnectionSetting.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NetworkInterfaceConnectionSetting>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NetworkInterfaceConnectionSetting& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::NetworkInterfaceConnectionSetting& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AutoNegotiation"s,"Speed"s,"Duplex"s};
		bool ret = false;
		bool gen_autoNegotiation;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_autoNegotiation);
		if (ret) value.setAutoNegotiation(gen_autoNegotiation);
		int gen_speed;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_speed);
		if (ret) value.setSpeed(gen_speed);
		std::string gen_duplex;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[2], true, deser, gen_duplex);
		if (ret) value.setDuplex(gen_duplex);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NetworkInterfaceConnectionSetting_INCLUDED

