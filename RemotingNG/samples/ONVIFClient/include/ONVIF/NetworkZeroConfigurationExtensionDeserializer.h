//
// NetworkZeroConfigurationExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NetworkZeroConfigurationExtension_INCLUDED
#define TypeDeserializer_ONVIF_NetworkZeroConfigurationExtension_INCLUDED


#include "ONVIF/NetworkZeroConfigurationDeserializer.h"
#include "ONVIF/NetworkZeroConfigurationExtension.h"
#include "ONVIF/NetworkZeroConfigurationExtension2Deserializer.h"
#include "ONVIF/NetworkZeroConfigurationExtension2Serializer.h"
#include "ONVIF/NetworkZeroConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NetworkZeroConfigurationExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NetworkZeroConfigurationExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::NetworkZeroConfigurationExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Additional"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::NetworkZeroConfiguration> gen_additional;
		ret = TypeDeserializer<std::vector<ONVIF::NetworkZeroConfiguration>>::deserialize(REMOTING__NAMES[0], false, deser, gen_additional);
		if (ret) value.setAdditional(gen_additional);
		Poco::SharedPtr<ONVIF::NetworkZeroConfigurationExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NetworkZeroConfigurationExtension2>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NetworkZeroConfigurationExtension_INCLUDED

