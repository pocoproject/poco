//
// OSDImgOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_OSDImgOptions_INCLUDED
#define TypeDeserializer_ONVIF_OSDImgOptions_INCLUDED


#include "ONVIF/OSDImgOptions.h"
#include "ONVIF/OSDImgOptionsExtensionDeserializer.h"
#include "ONVIF/OSDImgOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::OSDImgOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::OSDImgOptions& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::OSDImgOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FormatsSupported"s,"MaxSize"s,"MaxWidth"s,"MaxHeight"s};
		bool ret = false;
		std::vector<std::vector<std::string>> gen_formatsSupported;
		ret = TypeDeserializer<std::vector<std::vector<std::string>>>::deserialize(REMOTING__NAMES[0], false, deser, gen_formatsSupported);
		if (ret) value.setFormatsSupported(gen_formatsSupported);
		Poco::Optional<int> gen_maxSize;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[1], false, deser, gen_maxSize);
		if (ret) value.setMaxSize(gen_maxSize);
		Poco::Optional<int> gen_maxWidth;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[2], false, deser, gen_maxWidth);
		if (ret) value.setMaxWidth(gen_maxWidth);
		Poco::Optional<int> gen_maxHeight;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[3], false, deser, gen_maxHeight);
		if (ret) value.setMaxHeight(gen_maxHeight);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::OSDImgOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ImagePath"s,"Extension"s};
		bool ret = false;
		std::vector<Poco::URI> gen_imagePath;
		ret = TypeDeserializer<std::vector<Poco::URI>>::deserialize(REMOTING__NAMES[0], true, deser, gen_imagePath);
		if (ret) value.setImagePath(gen_imagePath);
		Poco::SharedPtr<ONVIF::OSDImgOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDImgOptionsExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FormatsSupported"s,"MaxSize"s,"MaxWidth"s,"MaxHeight"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[3], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_OSDImgOptions_INCLUDED

