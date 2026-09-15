//
// VideoSourceConfigurationExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoSourceConfigurationExtension_INCLUDED
#define TypeDeserializer_ONVIF_VideoSourceConfigurationExtension_INCLUDED


#include "ONVIF/RotateDeserializer.h"
#include "ONVIF/RotateSerializer.h"
#include "ONVIF/VideoSourceConfigurationExtension.h"
#include "ONVIF/VideoSourceConfigurationExtension2Deserializer.h"
#include "ONVIF/VideoSourceConfigurationExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoSourceConfigurationExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoSourceConfigurationExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoSourceConfigurationExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Rotate"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::Rotate> gen_rotate;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Rotate>>::deserialize(REMOTING__NAMES[0], false, deser, gen_rotate);
		if (ret) value.setRotate(gen_rotate);
		Poco::SharedPtr<ONVIF::VideoSourceConfigurationExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoSourceConfigurationExtension2>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoSourceConfigurationExtension_INCLUDED

