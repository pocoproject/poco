//
// OSDTextOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_OSDTextOptions_INCLUDED
#define TypeDeserializer_ONVIF_OSDTextOptions_INCLUDED


#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/OSDColorOptionsDeserializer.h"
#include "ONVIF/OSDColorOptionsSerializer.h"
#include "ONVIF/OSDTextOptions.h"
#include "ONVIF/OSDTextOptionsExtensionDeserializer.h"
#include "ONVIF/OSDTextOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::OSDTextOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::OSDTextOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::OSDTextOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"FontSizeRange"s,"DateFormat"s,"TimeFormat"s,"FontColor"s,"BackgroundColor"s,"Extension"s};
		bool ret = false;
		std::vector<std::string> gen_type;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], true, deser, gen_type);
		if (ret) value.setType(gen_type);
		Poco::SharedPtr<ONVIF::IntRange> gen_fontSizeRange;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IntRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_fontSizeRange);
		if (ret) value.setFontSizeRange(gen_fontSizeRange);
		std::vector<std::string> gen_dateFormat;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_dateFormat);
		if (ret) value.setDateFormat(gen_dateFormat);
		std::vector<std::string> gen_timeFormat;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[3], false, deser, gen_timeFormat);
		if (ret) value.setTimeFormat(gen_timeFormat);
		Poco::SharedPtr<ONVIF::OSDColorOptions> gen_fontColor;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDColorOptions>>::deserialize(REMOTING__NAMES[4], false, deser, gen_fontColor);
		if (ret) value.setFontColor(gen_fontColor);
		Poco::SharedPtr<ONVIF::OSDColorOptions> gen_backgroundColor;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDColorOptions>>::deserialize(REMOTING__NAMES[5], false, deser, gen_backgroundColor);
		if (ret) value.setBackgroundColor(gen_backgroundColor);
		Poco::SharedPtr<ONVIF::OSDTextOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDTextOptionsExtension>>::deserialize(REMOTING__NAMES[6], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_OSDTextOptions_INCLUDED

