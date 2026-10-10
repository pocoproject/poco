//
// VideoSourceConfigurationOptionsExtension2Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoSourceConfigurationOptionsExtension2_INCLUDED
#define TypeDeserializer_ONVIF_VideoSourceConfigurationOptionsExtension2_INCLUDED


#include "ONVIF/VideoSourceConfigurationOptionsExtension2.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoSourceConfigurationOptionsExtension2>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoSourceConfigurationOptionsExtension2& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoSourceConfigurationOptionsExtension2& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SceneOrientationMode"s};
		bool ret = false;
		std::vector<std::string> gen_sceneOrientationMode;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_sceneOrientationMode);
		if (ret) value.setSceneOrientationMode(gen_sceneOrientationMode);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoSourceConfigurationOptionsExtension2_INCLUDED

