//
// OSDImgConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_OSDImgConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_OSDImgConfiguration_INCLUDED


#include "ONVIF/OSDImgConfiguration.h"
#include "ONVIF/OSDImgConfigurationExtensionDeserializer.h"
#include "ONVIF/OSDImgConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::OSDImgConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::OSDImgConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::OSDImgConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ImgPath"s,"Extension"s};
		bool ret = false;
		Poco::URI gen_imgPath;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_imgPath);
		if (ret) value.setImgPath(gen_imgPath);
		Poco::SharedPtr<ONVIF::OSDImgConfigurationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDImgConfigurationExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_OSDImgConfiguration_INCLUDED

