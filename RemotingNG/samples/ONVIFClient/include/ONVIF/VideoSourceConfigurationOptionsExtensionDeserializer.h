//
// VideoSourceConfigurationOptionsExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoSourceConfigurationOptionsExtension_INCLUDED
#define TypeDeserializer_ONVIF_VideoSourceConfigurationOptionsExtension_INCLUDED


#include "ONVIF/RotateOptionsDeserializer.h"
#include "ONVIF/RotateOptionsSerializer.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtension.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtension2Deserializer.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoSourceConfigurationOptionsExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoSourceConfigurationOptionsExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoSourceConfigurationOptionsExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Rotate"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::RotateOptions> gen_rotate;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RotateOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_rotate);
		if (ret) value.setRotate(gen_rotate);
		Poco::SharedPtr<ONVIF::VideoSourceConfigurationOptionsExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoSourceConfigurationOptionsExtension2>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoSourceConfigurationOptionsExtension_INCLUDED

