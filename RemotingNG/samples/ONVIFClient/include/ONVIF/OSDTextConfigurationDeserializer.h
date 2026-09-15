//
// OSDTextConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_OSDTextConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_OSDTextConfiguration_INCLUDED


#include "ONVIF/OSDColorDeserializer.h"
#include "ONVIF/OSDColorSerializer.h"
#include "ONVIF/OSDTextConfiguration.h"
#include "ONVIF/OSDTextConfigurationExtensionDeserializer.h"
#include "ONVIF/OSDTextConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::OSDTextConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::OSDTextConfiguration& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::OSDTextConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IsPersistentText"s};
		bool ret = false;
		Poco::Optional<bool> gen_isPersistentText;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_isPersistentText);
		if (ret) value.setIsPersistentText(gen_isPersistentText);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::OSDTextConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"DateFormat"s,"TimeFormat"s,"FontSize"s,"FontColor"s,"BackgroundColor"s,"PlainText"s,"Extension"s};
		bool ret = false;
		std::string gen_type;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_type);
		if (ret) value.setType(gen_type);
		Poco::Optional<std::string> gen_dateFormat;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_dateFormat);
		if (ret) value.setDateFormat(gen_dateFormat);
		Poco::Optional<std::string> gen_timeFormat;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_timeFormat);
		if (ret) value.setTimeFormat(gen_timeFormat);
		Poco::Optional<int> gen_fontSize;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[3], false, deser, gen_fontSize);
		if (ret) value.setFontSize(gen_fontSize);
		Poco::SharedPtr<ONVIF::OSDColor> gen_fontColor;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDColor>>::deserialize(REMOTING__NAMES[4], false, deser, gen_fontColor);
		if (ret) value.setFontColor(gen_fontColor);
		Poco::SharedPtr<ONVIF::OSDColor> gen_backgroundColor;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDColor>>::deserialize(REMOTING__NAMES[5], false, deser, gen_backgroundColor);
		if (ret) value.setBackgroundColor(gen_backgroundColor);
		Poco::Optional<std::string> gen_plainText;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[6], false, deser, gen_plainText);
		if (ret) value.setPlainText(gen_plainText);
		Poco::SharedPtr<ONVIF::OSDTextConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDTextConfigurationExtension>>::deserialize(REMOTING__NAMES[7], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IsPersistentText"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_OSDTextConfiguration_INCLUDED

