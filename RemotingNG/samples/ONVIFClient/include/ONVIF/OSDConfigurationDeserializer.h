//
// OSDConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_OSDConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_OSDConfiguration_INCLUDED


#include "ONVIF/OSDConfiguration.h"
#include "ONVIF/OSDConfigurationExtensionDeserializer.h"
#include "ONVIF/OSDConfigurationExtensionSerializer.h"
#include "ONVIF/OSDImgConfigurationDeserializer.h"
#include "ONVIF/OSDImgConfigurationSerializer.h"
#include "ONVIF/OSDPosConfigurationDeserializer.h"
#include "ONVIF/OSDPosConfigurationSerializer.h"
#include "ONVIF/OSDReferenceDeserializer.h"
#include "ONVIF/OSDReferenceSerializer.h"
#include "ONVIF/OSDTextConfigurationDeserializer.h"
#include "ONVIF/OSDTextConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::OSDConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::OSDConfiguration& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::OSDConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::OSDConfiguration& value)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"VideoSourceConfigurationToken"s,"Type"s,"Position"s,"TextString"s,"Image"s,"Extension"s};
		bool ret = false;
		ONVIF::OSDReference gen_videoSourceConfigurationToken;
		ret = TypeDeserializer<ONVIF::OSDReference>::deserialize(REMOTING__NAMES[0], true, deser, gen_videoSourceConfigurationToken);
		if (ret) value.setVideoSourceConfigurationToken(gen_videoSourceConfigurationToken);
		std::string gen_type;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_type);
		if (ret) value.setType(gen_type);
		ONVIF::OSDPosConfiguration gen_position;
		ret = TypeDeserializer<ONVIF::OSDPosConfiguration>::deserialize(REMOTING__NAMES[2], true, deser, gen_position);
		if (ret) value.setPosition(gen_position);
		Poco::SharedPtr<ONVIF::OSDTextConfiguration> gen_textString;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDTextConfiguration>>::deserialize(REMOTING__NAMES[3], false, deser, gen_textString);
		if (ret) value.setTextString(gen_textString);
		Poco::SharedPtr<ONVIF::OSDImgConfiguration> gen_image;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDImgConfiguration>>::deserialize(REMOTING__NAMES[4], false, deser, gen_image);
		if (ret) value.setImage(gen_image);
		Poco::SharedPtr<ONVIF::OSDConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDConfigurationExtension>>::deserialize(REMOTING__NAMES[5], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
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


#endif // TypeDeserializer_ONVIF_OSDConfiguration_INCLUDED

