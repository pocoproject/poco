//
// PTZConfigurationExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZConfigurationExtension_INCLUDED
#define TypeDeserializer_ONVIF_PTZConfigurationExtension_INCLUDED


#include "ONVIF/PTControlDirectionDeserializer.h"
#include "ONVIF/PTControlDirectionSerializer.h"
#include "ONVIF/PTZConfigurationExtension.h"
#include "ONVIF/PTZConfigurationExtension2Deserializer.h"
#include "ONVIF/PTZConfigurationExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZConfigurationExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZConfigurationExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZConfigurationExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PTControlDirection"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::PTControlDirection> gen_pTControlDirection;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTControlDirection>>::deserialize(REMOTING__NAMES[0], false, deser, gen_pTControlDirection);
		if (ret) value.setPTControlDirection(gen_pTControlDirection);
		Poco::SharedPtr<ONVIF::PTZConfigurationExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZConfigurationExtension2>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZConfigurationExtension_INCLUDED

