//
// OSDConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_OSDConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_OSDConfigurationOptions_INCLUDED


#include "ONVIF/MaximumNumberOfOSDsDeserializer.h"
#include "ONVIF/MaximumNumberOfOSDsSerializer.h"
#include "ONVIF/OSDConfigurationOptions.h"
#include "ONVIF/OSDConfigurationOptionsExtensionDeserializer.h"
#include "ONVIF/OSDConfigurationOptionsExtensionSerializer.h"
#include "ONVIF/OSDImgOptionsDeserializer.h"
#include "ONVIF/OSDImgOptionsSerializer.h"
#include "ONVIF/OSDTextOptionsDeserializer.h"
#include "ONVIF/OSDTextOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::OSDConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::OSDConfigurationOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::OSDConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MaximumNumberOfOSDs"s,"Type"s,"PositionOption"s,"TextOption"s,"ImageOption"s,"Extension"s};
		bool ret = false;
		ONVIF::MaximumNumberOfOSDs gen_maximumNumberOfOSDs;
		ret = TypeDeserializer<ONVIF::MaximumNumberOfOSDs>::deserialize(REMOTING__NAMES[0], true, deser, gen_maximumNumberOfOSDs);
		if (ret) value.setMaximumNumberOfOSDs(gen_maximumNumberOfOSDs);
		std::vector<std::string> gen_type;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], true, deser, gen_type);
		if (ret) value.setType(gen_type);
		std::vector<std::string> gen_positionOption;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[2], true, deser, gen_positionOption);
		if (ret) value.setPositionOption(gen_positionOption);
		Poco::SharedPtr<ONVIF::OSDTextOptions> gen_textOption;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDTextOptions>>::deserialize(REMOTING__NAMES[3], false, deser, gen_textOption);
		if (ret) value.setTextOption(gen_textOption);
		Poco::SharedPtr<ONVIF::OSDImgOptions> gen_imageOption;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDImgOptions>>::deserialize(REMOTING__NAMES[4], false, deser, gen_imageOption);
		if (ret) value.setImageOption(gen_imageOption);
		Poco::SharedPtr<ONVIF::OSDConfigurationOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDConfigurationOptionsExtension>>::deserialize(REMOTING__NAMES[5], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_OSDConfigurationOptions_INCLUDED

