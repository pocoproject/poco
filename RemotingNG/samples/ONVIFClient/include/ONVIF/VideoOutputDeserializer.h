//
// VideoOutputDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoOutput_INCLUDED
#define TypeDeserializer_ONVIF_VideoOutput_INCLUDED


#include "ONVIF/LayoutDeserializer.h"
#include "ONVIF/LayoutSerializer.h"
#include "ONVIF/VideoOutput.h"
#include "ONVIF/VideoOutputExtensionDeserializer.h"
#include "ONVIF/VideoOutputExtensionSerializer.h"
#include "ONVIF/VideoResolutionDeserializer.h"
#include "ONVIF/VideoResolutionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoOutput>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoOutput& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::VideoOutput& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoOutput& value)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"Layout"s,"Resolution"s,"RefreshRate"s,"AspectRatio"s,"Extension"s};
		bool ret = false;
		ONVIF::Layout gen_layout;
		ret = TypeDeserializer<ONVIF::Layout>::deserialize(REMOTING__NAMES[0], true, deser, gen_layout);
		if (ret) value.setLayout(gen_layout);
		Poco::SharedPtr<ONVIF::VideoResolution> gen_resolution;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoResolution>>::deserialize(REMOTING__NAMES[1], false, deser, gen_resolution);
		if (ret) value.setResolution(gen_resolution);
		Poco::Optional<float> gen_refreshRate;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[2], false, deser, gen_refreshRate);
		if (ret) value.setRefreshRate(gen_refreshRate);
		Poco::Optional<float> gen_aspectRatio;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[3], false, deser, gen_aspectRatio);
		if (ret) value.setAspectRatio(gen_aspectRatio);
		Poco::SharedPtr<ONVIF::VideoOutputExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::VideoOutputExtension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
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


#endif // TypeDeserializer_ONVIF_VideoOutput_INCLUDED

