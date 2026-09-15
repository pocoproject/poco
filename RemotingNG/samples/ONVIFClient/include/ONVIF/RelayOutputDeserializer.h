//
// RelayOutputDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RelayOutput_INCLUDED
#define TypeDeserializer_ONVIF_RelayOutput_INCLUDED


#include "ONVIF/RelayOutput.h"
#include "ONVIF/RelayOutputSettingsDeserializer.h"
#include "ONVIF/RelayOutputSettingsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RelayOutput>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RelayOutput& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::RelayOutput& value)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__ONVIF__DEVICEENTITY[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		}
		
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::RelayOutput& value)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"Properties"s};
		bool ret = false;
		ONVIF::RelayOutputSettings gen_properties;
		ret = TypeDeserializer<ONVIF::RelayOutputSettings>::deserialize(REMOTING__NAMES[0], true, deser, gen_properties);
		if (ret) value.setProperties(gen_properties);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES__ONVIF__DEVICEENTITY[1], REMOTING__NAMES__ONVIF__DEVICEENTITY[0], true);
		}
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RelayOutput_INCLUDED

