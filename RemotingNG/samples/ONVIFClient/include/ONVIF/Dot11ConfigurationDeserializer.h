//
// Dot11ConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Dot11Configuration_INCLUDED
#define TypeDeserializer_ONVIF_Dot11Configuration_INCLUDED


#include "ONVIF/Dot11Configuration.h"
#include "ONVIF/Dot11SecurityConfigurationDeserializer.h"
#include "ONVIF/Dot11SecurityConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Dot11Configuration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Dot11Configuration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Dot11Configuration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SSID"s,"Mode"s,"Alias"s,"Priority"s,"Security"s};
		bool ret = false;
		std::string gen_sSID;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_sSID);
		if (ret) value.setSSID(gen_sSID);
		std::string gen_mode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		std::string gen_alias;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[2], true, deser, gen_alias);
		if (ret) value.setAlias(gen_alias);
		int gen_priority;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[3], true, deser, gen_priority);
		if (ret) value.setPriority(gen_priority);
		ONVIF::Dot11SecurityConfiguration gen_security;
		ret = TypeDeserializer<ONVIF::Dot11SecurityConfiguration>::deserialize(REMOTING__NAMES[4], true, deser, gen_security);
		if (ret) value.setSecurity(gen_security);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Dot11Configuration_INCLUDED

