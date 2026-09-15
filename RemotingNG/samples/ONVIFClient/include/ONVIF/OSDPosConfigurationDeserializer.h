//
// OSDPosConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_OSDPosConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_OSDPosConfiguration_INCLUDED


#include "ONVIF/OSDPosConfiguration.h"
#include "ONVIF/OSDPosConfigurationExtensionDeserializer.h"
#include "ONVIF/OSDPosConfigurationExtensionSerializer.h"
#include "ONVIF/VectorDeserializer.h"
#include "ONVIF/VectorSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::OSDPosConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::OSDPosConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::OSDPosConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"Pos"s,"Extension"s};
		bool ret = false;
		std::string gen_type;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_type);
		if (ret) value.setType(gen_type);
		Poco::SharedPtr<ONVIF::Vector> gen_pos;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Vector>>::deserialize(REMOTING__NAMES[1], false, deser, gen_pos);
		if (ret) value.setPos(gen_pos);
		Poco::SharedPtr<ONVIF::OSDPosConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDPosConfigurationExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_OSDPosConfiguration_INCLUDED

