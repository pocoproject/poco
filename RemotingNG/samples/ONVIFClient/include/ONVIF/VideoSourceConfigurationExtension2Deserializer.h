//
// VideoSourceConfigurationExtension2Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoSourceConfigurationExtension2_INCLUDED
#define TypeDeserializer_ONVIF_VideoSourceConfigurationExtension2_INCLUDED


#include "ONVIF/LensDescriptionDeserializer.h"
#include "ONVIF/LensDescriptionSerializer.h"
#include "ONVIF/SceneOrientationDeserializer.h"
#include "ONVIF/SceneOrientationSerializer.h"
#include "ONVIF/VideoSourceConfigurationExtension2.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoSourceConfigurationExtension2>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoSourceConfigurationExtension2& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoSourceConfigurationExtension2& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"LensDescription"s,"SceneOrientation"s};
		bool ret = false;
		std::vector<ONVIF::LensDescription> gen_lensDescription;
		ret = TypeDeserializer<std::vector<ONVIF::LensDescription>>::deserialize(REMOTING__NAMES[0], false, deser, gen_lensDescription);
		if (ret) value.setLensDescription(gen_lensDescription);
		Poco::SharedPtr<ONVIF::SceneOrientation> gen_sceneOrientation;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SceneOrientation>>::deserialize(REMOTING__NAMES[1], false, deser, gen_sceneOrientation);
		if (ret) value.setSceneOrientation(gen_sceneOrientation);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoSourceConfigurationExtension2_INCLUDED

